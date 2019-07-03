#include "debug.h"
#include "parameters.h"
#include "distancefield.h"

#include <vector>
#include <fstream>
#include <functional>
#include <glm/glm.hpp>

#include <iostream>
#include <glm/gtx/string_cast.hpp>

using namespace std;
using namespace glm;

#include "boundingbox.hpp"
#include "mesh.h"
#include "mathUtils.h"

ofstream &operator<<(ofstream &o, const DistanceFieldData &data)
{
    o << data.hashedName << endl
      << data.size.x << " " << data.size.y << " " << data.size.z << endl
      << data.resolution << endl;

    vec3 v = data.distanceFieldBBox.min;
    o << v.x << " " << v.y << " " << v.z << endl;

    v = data.distanceFieldBBox.max;
    o << v.x << " " << v.y << " " << v.z << endl;

    vec2 v2 = data.minMaxDistance;
    o << v2.x << " " << v2.y << endl;

    for (float f : data.uncompressedVolumeData)
    {
        o << f << endl;
    }

    for (vec3 v : data.positionData)
    {
        o << v.x << " " << v.y << " " << v.z << endl;
    }
    return o;
}

ifstream &operator>>(ifstream &i, DistanceFieldData &data)
{
    int x, y, z;
    float fx, fy, fz;
    float gx, gy, gz;

    i >> data.hashedName;

    i >> x >> y >> z;
    data.size = ivec3(x, y, z);

    i >> data.resolution;

    i >> fx >> fy >> fz >> gx >> gy >> gz;
    data.distanceFieldBBox = BBox(vec3(fx, fy, fz), vec3(gx, gy, gz));

    i >> fx >> fy;
    data.minMaxDistance = vec2(fx, fy);

    int size = data.size.x * data.size.y * data.size.z;
    data.uncompressedVolumeData = vector<float>(size);
    data.positionData = vector<vec3>(size);

    for (int idx = 0; idx < size; idx++)
    {
        i >> data.uncompressedVolumeData.at(idx);
    }

    for (int idx = 0; idx < size; idx++)
    {
        i >> fx >> fy >> fz;
        data.positionData.at(idx) = vec3(fx, fy, fz);
    }

    return i;
}

void DistanceFieldData::CheckExistDistanceFieldData()
{
    string str = this->mesh.name + std::to_string(this->resolution);
    hashedName = hash<string>{}(str);
#ifdef DEBUG
    cout << "DistanceField: hashedName " << hashedName << endl;
#endif // DEBUG

    string path = "./data/" + std::to_string(hashedName) + ".sdf";
    ifstream ifs(path);
    if (ifs)
    {
        cout << "DistanceField: Existing distance field data, loading. " << endl;
        ifs >> (*this);
        cout << "DistanceField: Data loaded." << endl;
        ifs.close();
#ifdef DEBUG
        cout << "DistanceField:: Volumn Size: " << glm::to_string(this->size) << endl;
#endif // DEBUG
    }
    else
    {
        ifs.close();
        CalculateDistanceField();
    }
}

void DistanceFieldData::CalculateDistanceField()
{
    // at least (?) voxels on each dimension
    int minVoxelPerDim = (int)trunc(DF_MIN_VOXEL_PER_DIM * resolution);

    // new bounding box : bit larger
    BBox originalBox = mesh.boundingBox;
    vec3 originalDiag = originalBox.GetHalfDiagonal();
    // new diagonal length, guarantee that the bounding box of distance field volume is LARGER than the bbox of mesh
    // how much larger? depends on the method to handle sdf query on bbox borders,
    // in this case it should be several-voxels-larger.
    vec3 newDiag = CoordMax(originalDiag * 0.2f, 4.f * originalBox.GetSize() / (float)minVoxelPerDim);
    distanceFieldBBox = BBox(originalBox.min - newDiag, originalBox.max + newDiag);

#ifdef DEBUG
    cout << "Original Box: \t" << glm::to_string(originalBox.min) << " \t " << glm::to_string(originalBox.max) << endl;
    cout << glm::to_string(originalBox.GetCenter()) << endl;
    cout << "New D F  Box: \t" << glm::to_string(distanceFieldBBox.min) << " \t " << glm::to_string(distanceFieldBBox.max) << endl;
    cout << glm::to_string(distanceFieldBBox.GetCenter()) << endl;
#endif // DEBUG

    // voxel should be like a cube, having at least certain amount on shortest axis
    vec3 dfBoxSize = distanceFieldBBox.GetSize();
    float voxelSideLength = fmin(fmin(dfBoxSize.x, dfBoxSize.y), dfBoxSize.z) / minVoxelPerDim;

    vec3 dfDimensions = dfBoxSize / voxelSideLength;
    size = ivec3((int)trunc(dfDimensions.x),
                 (int)trunc(dfDimensions.y),
                 (int)trunc(dfDimensions.z));

    // sample rays
    int thetaSteps = (int)std::trunc(sqrt(DF_NUM_SAMPLE_RAYS * 2));
    int phiSteps = thetaSteps / 2;

    vector<vec3> sampleRays;
    sampleRays.clear();

    vec3 sum(0);

    float phiOffset = PI / phiSteps * 0.5f;

    for (int phiStep = 0; phiStep < phiSteps; phiStep++)
    {
        float phi = -PI / 2.f + (PI * phiStep) / phiSteps + phiOffset;
        for (int thetaStep = 0; thetaStep < thetaSteps; thetaStep++)
        {
            float theta = (PI * 2.f * thetaStep) / thetaSteps;
            vec3 direction(cosf(theta), sinf(theta), sinf(phi));
            sum += normalize(direction);
            sampleRays.push_back(normalize(direction));
        }
    }

    int vectorSize = size.x * size.y * size.z;
    uncompressedVolumeData = vector<float>(vectorSize);
    positionData = vector<vec3>(vectorSize);
    cout << "DistanceField: Calculate distance field voxels" << endl;
    for (int zIndex = 0; zIndex < dfDimensions.z; zIndex++)
    {
        CalculateDistanceFieldLayer(sampleRays, zIndex);
        cout << (int)trunc((zIndex + 1) * 100.f / dfDimensions.z) << "% complete ... " << endl;
    }
    cout << "DistanceField: Calculation finished" << endl;

    PostDistanceFieldCalculation();
}

void DistanceFieldData::PostDistanceFieldCalculation()
{
    string path = "./data/" + std::to_string(hashedName) + ".sdf";
    ofstream ofs;
    ofs.open(path);

    if (!ofs.is_open())
    {
        cout << "DistanceField: Meet error saving SDF texture, won't save." << endl;
        cout << path << endl;
    }
    else
    {
        cout << "DistanceField: Texture saving." << endl;
        ofs << (*this);
        ofs.close();
        cout << "DistanceField: Texture saved." << endl;
    }
}

void DistanceFieldData::CalculateDistanceFieldLayer(
    vector<vec3> &sampleRays,
    int zIndex)
{
    const ivec3 &dimensions = size;
    vec3 boxSize = distanceFieldBBox.GetSize();
    vec3 voxelSize(boxSize.x / dimensions.x,
                   boxSize.y / dimensions.y,
                   boxSize.z / dimensions.z);

    float maxLength = glm::length(boxSize);

    for (int yIndex = 0; yIndex < dimensions.y; yIndex++)
    {
        for (int xIndex = 0; xIndex < dimensions.x; xIndex++)
        {
            // offset, refer to output buffer
            int index = zIndex * dimensions.y * dimensions.x +
                        yIndex * dimensions.x + xIndex;
            vec3 position = distanceFieldBBox.min + voxelSize * vec3(xIndex + 0.5f, yIndex + 0.5f, zIndex + 0.5f);

            // cout << glm::to_string(position) << " " ;

            int hit = 0;
            int hitBack = 0;
            float minDist = maxLength; // shortest distance to mesh among rays
            for (vec3 &dir : sampleRays)
            {
                ivec3 hitIndex(-1);
                float shortestDist = maxLength; // shortest distance to mesh of 1 sample ray
                vec3 hitNormal(0);

                if (RayMeshIntersection(position, dir, shortestDist, hitIndex, hitNormal))
                {
                    hit++;
                    if (dot(dir, hitNormal) > 0)
                    {
                        hitBack++;
                    }
                    if (shortestDist < minDist)
                        minDist = shortestDist;
                }
            }
            if (hitBack > 0.5f * DF_NUM_SAMPLE_RAYS || hitBack > 0.9f * hit)
            {
                minDist = -minDist;
            }

            uncompressedVolumeData.at(index) = minDist;
            positionData.at(index) = position;
            // cout << ((minDist > 0) ? 1 : 0) << " ";
        }
        // cout << endl;
    }
}

bool DistanceFieldData::RayMeshIntersection(const vec3 &ori, const vec3 &dir, float &minDist, ivec3 &hitIndex, vec3 &hitNormal)
{
    for (int idx = 0; idx < mesh.indices.size(); idx += 3)
    {
        unsigned int i0 = mesh.indices[idx];
        unsigned int i1 = mesh.indices[idx + 1];
        unsigned int i2 = mesh.indices[idx + 2];

        vec3 v0 = mesh.vertices[i0].Position;
        vec3 v1 = mesh.vertices[i1].Position;
        vec3 v2 = mesh.vertices[i2].Position;

        vec3 e1 = v1 - v0; // edge 1
        vec3 e2 = v2 - v0; // edge 2
        vec3 b = ori - v0; // Ax = b;
        vec3 d = dir;

        mat3 A(-d, e1, e2);

        // use Cramer's rule instead of inverse of matrix
        float f2 = -d.x * e1.y * e2.z - d.y * e1.z * e2.x - d.z * e1.x * e2.y - (-d.z * e1.y * e2.x - d.y * e1.x * e2.z - d.x * e1.z * e2.y);
        if (f2 != 0)
        {
            // float t = glm::determinant(mat3(b, e1, e2)) / f2;
            float t = b.x * e1.y * e2.z + b.y * e1.z * e2.x + b.z * e1.x * e2.y - (b.z * e1.y * e2.x + b.y * e1.x * e2.z + b.x * e1.z * e2.y);
            t /= f2;
            if (t >= ZERO && t < minDist)
            {
                // float u = glm::determinant(mat3(-d, b, e2)) / f2;
                float u = -d.x * b.y * e2.z - d.y * b.z * e2.x - d.z * b.x * e2.y - (-d.z * b.y * e2.x - d.y * b.x * e2.z - d.x * b.z * e2.y);
                u /= f2;
                if (u >= 0 && u <= 1.0f)
                {
                    // float v = glm::determinant(mat3(-d, e1, b)) / f2;
                    float v = -d.x * e1.y * b.z - d.y * e1.z * b.x - d.z * e1.x * b.y - (-d.z * e1.y * b.x - d.y * e1.x * b.z - d.x * e1.z * b.y);
                    v /= f2;
                    if (v >= 0 && u + v <= 1.0f)
                    {
                        minDist = t;
                        hitNormal = (1.f - u - v) * mesh.vertices[i0].Normal +
                                    u * mesh.vertices[i1].Normal +
                                    v * mesh.vertices[i2].Normal;
                        hitIndex = ivec3(i0, i1, i2);
                    }
                }
            }
        }
    }
    if (hitIndex.x != -1)
    {
        return true;
    }
    return false;
}