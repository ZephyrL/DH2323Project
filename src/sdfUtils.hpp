#ifndef _SDFUTILS_HPP_
#define _SDFUTILS_HPP_
// Sphere
// s: radius
#include <glm/glm.hpp>
using namespace glm;

float lerp(float a, float b, float t)
{
    return (1 - t) * a + t * b;
}

float sdSphere(vec3 p, float s)
{
    return length(p) - s;
}

// Box
// b: size of box in x/y/z
// !!! b = (x,y,z) is halfDiagonal of box, whose true size is (2x, 2y, 2z);
float sdBox(vec3 p, vec3 b)
{
    vec3 d = abs(p) - b;
    return fmin(fmax(d.x, fmax(d.y, d.z)), 0.0) +
           length(glm::max(d, 0.0));
}

// t.x = larger radius, t.y = pipe radius
float sdTorus(vec3 p, vec2 t)
{
    vec2 q = vec2(length(vec2(p.x, p.z)) - t.x, p.y);
    return length(q) - t.y;
}

float sdCone(vec3 p, vec2 c)
{
    // c must be normalized
    float q = length(vec2(p));
    return dot(c, vec2(q, p.z));
}

float sdRoundedCylinder(vec3 p, float ra, float rb, float h)
{
    vec2 d = vec2(length(vec2(p.x, p.z)) - 2.0f * ra + rb, fabs(p.y) - h);
    return fmin(fmax(d.x, d.y), 0.0f) + length(max(d, 0.0f)) - rb;
}

float sdOctahedron(vec3 p, float s)
{
    p = abs(p);
    float m = p.x + p.y + p.z - s;
    vec3 q;
    if (3.0f * p.x < m)
        q = p;
    else if (3.0f * p.y < m)
        q = vec3(p.y, p.z, p.x);
    else if (3.0f * p.z < m)
        q = vec3(p.z, p.x, p.y);
    else
        return m * 0.57735027f;

    float k = clamp(0.5f * (q.z - q.y + s), 0.0f, s);
    return length(vec3(q.x, q.y - s + k, q.z - k));
}

// n.xyz = normal of plane , n.w = negative offset from origin,
// e.g. n = (0, 1, 0, 1), calculate the distance from any point to plane (y = -1)
float sdPlane(vec3 p, vec4 n)
{
    return dot(p, vec3(n)) - n.w;
}

float sdRoundBox(const vec3 &p, const vec3 &b, float r)
{

    vec3 q = abs(p) - b;
    return fmin(fmax(q.x, fmax(q.y, q.z)), 0.0) + length(glm::max(q, 0.0)) - r;
}
// BOOLEAN OPERATORS //

// Union
float opU(float d1, float d2)
{
    return fmin(d1, d2);
}

// Subtraction
float opS(float d1, float d2)
{
    return fmax(-d1, d2);
}

// Intersection
float opI(float d1, float d2)
{
    return fmax(d1, d2);
}

// Smooth operators
float opSU(float d1, float d2, float k)
{
    float h = clamp(0.5 + 0.5 * (d2 - d1) / k, 0.0, 1.0);
    return lerp(d2, d1, h) - k * h * (1.0 - h);
}

float opSS(float d1, float d2, float k)
{
    float h = clamp(0.5 - 0.5 * (d2 + d1) / k, 0.0, 1.0);
    return lerp(d2, -d1, h) + k * h * (1.0 - h);
}

float opSI(float d1, float d2, float k)
{
    float h = clamp(0.5 - 0.5 * (d2 - d1) / k, 0.0, 1.0);
    return lerp(d2, d1, h) + k * h * (1.0 - h);
}

// Mod Position Axis
float pMod1(float &p, float size)
{
    float halfsize = size * 0.5;
    float c = floor((p + halfsize) / size);
    p = fmod(p + halfsize, size) - halfsize;
    p = fmod(-p + halfsize, size) - halfsize;
    return c;
}

#endif // !_SDFUTILS_HPP_
