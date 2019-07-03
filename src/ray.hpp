#ifndef _RAY_HPP_
#define _RAY_HPP_

#include <glm/glm.hpp>
using namespace glm;

class Ray
{
public:
    vec3 pos;
    vec3 dir;

    Ray(vec3 p = vec3(0, 0, 0), vec3 d = vec3(0, 0, 1)) : pos(p), dir(d) {}
};

#endif // !_RAY_HPP_