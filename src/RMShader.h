#ifndef _RMSHADER_H_
#define _RMSHADER_H_

#include <glm/glm.hpp>
using namespace glm;

// forward declaration
class Scene;
class Ray;
class BBox;
class DistanceFieldData;

class RMShader
{
private:
    /* data */
public:
    vec3 render(const Ray &ray);
    void configure(){};

private:
    const Scene &scene;

    // temporary, ptrs and vars
    DistanceFieldData *df;
    BBox *meshBox;
    BBox *dfBox;
    vec3 boxSize;
    ivec3 dimensions;
    vec3 voxelSize;

private:
    // sdf functions
    vec3 getNormal(const vec3 &p);
    float sdf(const vec3 &p);
    vec3 sdfHitColor(const vec3 &p, float d);

    // shading function
    float softShadow(vec3 ro, vec3 rd, float mint, float maxt, float k);
    float ambientOcclusion(vec3 p, vec3 n);
    vec3 shading(const vec3 &p, const vec3 &n, vec3 objectColor);

public:
    RMShader(const Scene &s);
    ~RMShader(){};
};

#endif // !_RMSHADER_H_
