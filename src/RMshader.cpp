#include "parameters.h"
#include "debug.h"
#include "RMShader.h"

#include <glm/glm.hpp>
#include <vector>

#include <iostream>
#include <glm/gtx/string_cast.hpp>

using namespace std;
using namespace glm;

#include "scene.h"
#include "ray.hpp"
#include "sdfUtils.hpp"
#include "distancefield.h"
#include "boundingbox.hpp"

RMShader::RMShader(const Scene &s) : scene(s)
{
    const Mesh &mesh = scene.models[0].meshes[0];
    df = mesh.distanceFieldData;
    meshBox = new BBox(mesh.boundingBox);
    dfBox = new BBox(df->distanceFieldBBox);
    boxSize = dfBox->GetSize();
    dimensions = df->size;
    voxelSize = boxSize / vec3(dimensions);
}

vec3 RMShader::getNormal(const vec3 &p)
{
    const vec2 offset = vec2(0.02f, 0.0f);
    vec3 normal = vec3(
        sdf(p + vec3(offset.x, offset.y, offset.y)) - sdf(p - vec3(offset.x, offset.y, offset.y)),
        sdf(p + vec3(offset.y, offset.x, offset.y)) - sdf(p - vec3(offset.y, offset.x, offset.y)),
        sdf(p + vec3(offset.y, offset.y, offset.x)) - sdf(p - vec3(offset.y, offset.y, offset.x)));
    return normalize(normal);
}

float RMShader::sdf(const vec3 &p)
{
    float result;

    float objectResult;

    float ground = sdPlane(p, groundPos);

    float rBox = sdRoundBox(p - rdBoxPos, rdBoxSize, rdBoxR);

    float torus = sdTorus(p - torusPos, torusRad);

    float rCylinder = sdRoundedCylinder(p - cyPos, cyRa, cyRb, cyH);

    float octahedron = sdOctahedron(p - octPos, octSize);

    float rBox2 = sdRoundBox(p - combPos, rdBoxSize2, rdBoxR);
    float sphere2 = sdSphere(p - combPos, sphereR2);
    float sub = opSS(sphere2, rBox2, subK);

    objectResult = ground;
#ifdef PREZ
    objectResult = opU(objectResult, rBox);
    objectResult = opU(objectResult, torus);
    objectResult = opU(objectResult, rCylinder);
    objectResult = opU(objectResult, octahedron);
    objectResult = opU(objectResult, sub);
#endif // PREZ

    if (dfBox->Include(p))
    {
        // cout << "In box" << endl;
        vec3 pos = p - dfBox->min;
        vec3 halfVoxel = voxelSize * .5f;
        if (pos.x <= halfVoxel.x || pos.y <= halfVoxel.y || pos.z <= halfVoxel.z ||
            pos.x >= boxSize.x - halfVoxel.x || pos.y >= boxSize.y - halfVoxel.y || pos.z >= boxSize.z - halfVoxel.z)
        {
            // position is at border of the bounding box
            int index = (int)trunc(pos.x / voxelSize.x) +
                        (int)trunc(pos.y / voxelSize.y) * dimensions.x +
                        (int)trunc(pos.z / voxelSize.z) * dimensions.x * dimensions.y;
            result = df->uncompressedVolumeData[index];
        }
        else
        {
            // position is surrounded by 8 sample points
            pos -= halfVoxel; // pos > (0,0,0);
            int x = (int)floor(pos.x / voxelSize.x);
            int y = (int)floor(pos.y / voxelSize.x);
            int z = (int)floor(pos.z / voxelSize.x);

            if (x == dimensions.x - 1 || y == dimensions.y - 1 || z == dimensions.z - 1)
            {
                cout << "Warning: overbound" << endl;
            }

            int i0 = x + y * dimensions.x + z * dimensions.x * dimensions.y;
            int i1 = (x + 1) + y * dimensions.x + z * dimensions.x * dimensions.y;
            int i2 = (x + 1) + (y + 1) * dimensions.x + z * dimensions.x * dimensions.y;
            int i3 = x + (y + 1) * dimensions.x + z * dimensions.x * dimensions.y;
            int i4 = x + y * dimensions.x + (z + 1) * dimensions.x * dimensions.y;
            int i5 = (x + 1) + y * dimensions.x + (z + 1) * dimensions.x * dimensions.y;
            int i6 = (x + 1) + (y + 1) * dimensions.x + (z + 1) * dimensions.x * dimensions.y;
            int i7 = x + (y + 1) * dimensions.x + (z + 1) * dimensions.x * dimensions.y;

            float v0 = df->uncompressedVolumeData[i0];
            float v1 = df->uncompressedVolumeData[i1];
            float v2 = df->uncompressedVolumeData[i2];
            float v3 = df->uncompressedVolumeData[i3];
            float v4 = df->uncompressedVolumeData[i4];
            float v5 = df->uncompressedVolumeData[i5];
            float v6 = df->uncompressedVolumeData[i6];
            float v7 = df->uncompressedVolumeData[i7];

            vec3 p0 = df->positionData[i0];
            vec3 p1 = df->positionData[i1];
            vec3 p2 = df->positionData[i2];
            vec3 p3 = df->positionData[i3];
            vec3 p4 = df->positionData[i4];
            vec3 p5 = df->positionData[i5];
            vec3 p6 = df->positionData[i6];
            vec3 p7 = df->positionData[i7];

            float ax = (p.x - p0.x) / (p1.x - p0.x);
            float ay = (p.y - p0.y) / (p3.y - p0.y);
            float az = (p.z - p0.z) / (p4.z - p0.z);

            float bx = 1.f - ax;
            float by = 1.f - ay;
            float bz = 1.f - az;

            result = bx * by * bz * v0 +
                     ax * by * bz * v1 +
                     ax * ay * bz * v2 +
                     bx * ay * bz * v3 +
                     bx * by * az * v4 +
                     ax * by * az * v5 +
                     ax * ay * az * v6 +
                     bx * ay * az * v7;
        }
    }
    else
    {
        // cout << "mesh: " << glm::to_string(meshBox->GetSize()) << endl;
        // cout << "df:   " << glm::to_string(dfBox->GetSize()) << endl;
        result = sdBox(p - meshBox->GetCenter(), meshBox->GetHalfDiagonal());
    }

    return opU(objectResult, result);
}

vec3 RMShader::sdfHitColor(const vec3 &p, float d)
{
    // objects
    float ground = sdPlane(p, groundPos);
    if (ground <= d)
        return groundColor;
#ifdef PREZ
    float rBox = sdRoundBox(p - rdBoxPos, rdBoxSize, rdBoxR);
    if (rBox <= d)
        return rBoxColor;

    float torus = sdTorus(p - torusPos, torusRad);
    if (torus <= d)
        return torusColor;

    float rCylinder = sdRoundedCylinder(p - cyPos, cyRa, cyRb, cyH);
    if (rCylinder <= d)
        return cyColor;

    float octahedron = sdOctahedron(p - octPos, octSize);
    if (octahedron <= d)
        return octColor;

    float rBox2 = sdRoundBox(p - combPos, rdBoxSize2, rdBoxR);
    float sphere2 = sdSphere(p - combPos, sphereR2);
    float comb = opSS(sphere2, rBox2, subK);
    if (comb <= d)
        return combColor;
#endif // PREZ
    return bunnyColor;
}

float RMShader::softShadow(vec3 ro, vec3 rd, float mint, float maxt, float k)
{
    float result = 1.0f;
    for (float t = mint; t < maxt;)
    {
        float h = sdf(ro + rd * t);
        if (h < RM_ACCURACY)
        {
            // hit: directly cast a shadow
            return 0.0;
        }
        // the closer is the light to something, the darker is the soft shadow
        result = fmin(result, k * h / t);
        t += h;
    }
    return result;
}

float RMShader::ambientOcclusion(vec3 p, vec3 n)
{
    float step = RM_AO_STEP_SIZE;
    float ao = 0.0f;
    float dist;
    for (int i = 1; i <= RM_AO_MAX_ITER; i++)
    {
        dist = step * i;
        // closer steps contributes more to AO
        // object at shorter distance contributes more, too
        ao += fmax((dist - sdf(p + n * dist)) / dist, 0.0f);
    }
    return (1.0f - ao * RM_AO_INTENSITY);
}

vec3 RMShader::shading(const vec3 &p, const vec3 &n, vec3 color = DEFAULT_OBJECT_COLOR)
{
    vec3 result;
    // direction light
    vec3 light = (vec3(0.8f, 0.75f, 0.9f) * dot(-RM_LIGHT_DIR, n) * 0.5f + 0.5f) * 1.0f;
    // shadow
    float shadow = softShadow(p, -RM_LIGHT_DIR, RM_SHADOW_DISTANCE.x, RM_SHADOW_DISTANCE.y, RM_SHADOW_PENUMBRA) * 0.5f + 0.5f;
    shadow = fmax(0.0, pow(shadow, 1.0f));
    // AO
    float ao = ambientOcclusion(p, n);
    // float ao = 1.0f;

    result = color * light * shadow * ao;
    return result;
}

vec3 RMShader::render(const Ray &ray)
{
    vec4 result = vec4(1.f);

    const int maxIter = RM_MAX_ITER; // max step of marching
    float t = 0;                     // distance travelled

    for (int i = 0; i < maxIter; i++)
    {
        if (t > RM_MAX_DEPTH || t >= RM_MAX_DEPTH)
        {
            // not hit
            result = vec4(ray.dir, 0);
            break;
        }

        vec3 p = ray.pos + ray.dir * t;
        // check hit
        float d = sdf(p);
        if (d <= RM_ACCURACY)
        {
            // hit
            vec3 color = sdfHitColor(p, d) / 255.f;
            vec3 n = getNormal(p);
            vec3 s = shading(p, n, color);
            // vec3 s = vec3(t / (float)RM_MAX_DIST);
            result = vec4(s, 1);
            break;
        }

        t += d;
    }

    return vec3(result);
}