#ifndef _MATHUTILS_H_
#define _MATHUTILS_H_

#define PI (3.1415926535897932f)

#define MIN_COMPARE_INIT (1e9)
#define MAX_COMPARE_INIT (-1e9)

#define ZERO (1e-4)

#define EULERS_NUMBER (2.71828182845904523536f)
#define GOLDEN_RATIO (1.6180339887498948482045868343656381f)

#include <glm/glm.hpp>
using namespace glm;

vec3 CoordMin(const vec3 &v1, const vec3 &v2);
vec3 CoordMax(const vec3 &v1, const vec3 &v2);

vec3 Vabs(vec3 v);

#endif // !_MATHUTILS_H_