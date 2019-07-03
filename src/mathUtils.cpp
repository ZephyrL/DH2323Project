#include "mathUtils.h"

#include <glm/glm.hpp>

using namespace glm;

vec3 CoordMin(const vec3 &v1, const vec3 &v2)
{
    return vec3(fmin(v1.x, v2.x),
                fmin(v1.y, v2.y),
                fmin(v1.z, v2.z));
}

vec3 CoordMax(const vec3 &v1, const vec3 &v2)
{
    return vec3(fmax(v1.x, v2.x),
                fmax(v1.y, v2.y),
                fmax(v1.z, v2.z));
}

vec3 Vabs(vec3 v)
{
    return vec3(
        fabs(v.x),
        fabs(v.y),
        fabs(v.z));
}
