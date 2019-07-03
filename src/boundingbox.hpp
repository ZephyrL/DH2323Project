#ifndef _BOUNDINGBOX_HPP_
#define _BOUNDINGBOX_HPP_

#include <glm/glm.hpp>
using namespace glm;

class BBox
{
public:
    vec3 min, max; // Min(x,y,z) and Max(x,y,z) of mesh vertices

public:
    inline vec3 GetCenter() const
    {
        return (min + max) / 2.0f;
    }
    inline vec3 GetHalfDiagonal() const
    {
        return (max - min) / 2.0f;
    }

    inline vec3 GetSize() const
    {
        return (max - min);
    }

    inline bool Include(const vec3 &p)
    {
        return (min.x < p.x && min.y < p.y && min.z < p.z) &&
               (max.x > p.x && max.y > p.y && max.z > p.z);
    }

public:
    BBox(vec3 min = vec3(0), vec3 max = vec3(0)) : min(min), max(max) {}
    ~BBox() {}
};

#endif // !_BOUNDINGBOX_HPP_
