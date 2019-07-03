#ifndef _LIGHT_HPP_
#define _LIGHT_HPP_

#include <glm/glm.hpp>
using namespace glm;

class Light
{
    enum LightType
    {
        POINT_LIGHT,
        DIR_LIGHT,
        PLANE_LIGHT,
        SKY_LIGHT,
        LIGHT_TYPE_MAX
    };

private:
    /* data */
    LightType type;
    vec3 position;  // Point light
    vec3 direction; // Directional light
    // Plane light
    // Sky light

public:
    Light(/* args */){};
    ~Light(){};
};

#endif // !_LIGHT_HPP_
