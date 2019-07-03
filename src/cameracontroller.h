#ifndef _CAMERA_CONTROLLER_H_
#define _CAMERA_CONTROLLER_H_

#include <glm/glm.hpp>
#include <SDL.h>

// forward declaration
class Camera;

class CameraController
{
private:
    // reference to camera it controls
    Camera &refCam;
    // key listener
    Uint8 *keyState;
    // mouse listener
    int mouseX, mouseY;
    // moving flexibility
    float flexibility;

private:
    void Initialize();

public:
    void Update();

public:
    CameraController(Camera &cam,
                     float flex = 0.05f) : refCam(cam), flexibility(flex)
    {
        Initialize();
    }
    ~CameraController() {}
};

#endif // !_CAMERA_CONTROLLER_H_