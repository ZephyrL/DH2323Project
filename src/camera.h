#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <glm/glm.hpp>
#include <vector>

using namespace glm;
using namespace std;

#include "ray.hpp"

// forward declaration
class Ray;
class RMShader;
class Screen;
class Scene;

#ifdef DEBUG
#include <glm/gtx/string_cast.hpp>
#endif // DEBUG

// shaders
class RMShader;
class CameraController;

const float DEFAULT_FOV = 45.f;

// class of camera  // TODO: camera controller <- set*() functions
class Camera
{

private:
    // lookAt() parameters
    vec3 pos;
    vec3 head;
    vec3 target;
    // frustum
    bool ortho; // true = ortho, false = persp
    float minDepth, maxDepth;
    // field of view
    float fov;
    // settings of image plane of camera, aka. screen
    int imgWidth, imgHeight;
    int numOfRays;

    bool enableControl;
    CameraController *controller;

public:
    vector<Ray> rays;

private:
    void SetRays();

public:
    void Draw(Scene &, Screen &);

    void SetImagePlane(Screen &s);

    void SetControl(bool);

    void SetPos(vec3 p)
    {
        pos = p;
        SetRays();
    }
    void SetHead(vec3 h)
    {
        head = h;
        SetRays();
    }
    void SetTarget(vec3 f)
    {
        target = f;
        SetRays();
    }

    vec3 GetPos() const
    {
        return pos;
    }

    vec3 GetHead() const
    {
        return head;
    }

    vec3 GetTarget() const
    {
        return target;
    }

public:
    Camera(vec3 p = vec3(0, 0, 1.f),
           vec3 h = vec3(0, 1.f, 0),
           vec3 t = vec3(0, 0, 0),
           bool o = false,
           float minD = 0.01f,
           float maxD = 100.f,
           float fov = DEFAULT_FOV,
           bool c = false)
        : pos(p), head(h), target(t),
          ortho(o), minDepth(minD), maxDepth(maxD), fov(fov),
          enableControl(c), controller(nullptr)
    {
    }

    ~Camera() {}
};

#endif // !_CAMERA_H_