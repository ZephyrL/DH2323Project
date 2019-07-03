#include "debug.h"

#include "camera.h"

#include <glm/glm.hpp>
using namespace glm;

#include "scene.h"
#include "screen.h"
#include "cameracontroller.h"
#include "ray.hpp"
#include "RMShader.h"
#include "mathUtils.h"

void Camera::SetImagePlane(Screen &s)
{
    imgWidth = s.width;
    imgHeight = s.height;
    numOfRays = imgWidth * imgHeight;
    rays = vector<Ray>(numOfRays);
    SetRays();
}

void Camera::SetControl(bool b)
{
    this->enableControl = b;
    if ( b && (!controller))
        controller = new CameraController(*this);
}

void Camera::SetRays()
{
    // right = x+
    // up = y+
    // forward = z-

    vec3 forward = normalize(pos - target);       // forward
    vec3 right = normalize(cross(head, forward)); // right
    vec3 up = normalize(cross(forward, right));   // up

    // Camera to world matrix, calculated via right-hand coord

    float upOff = fov * 0.5 * PI / 180.f;
    float rightOff = upOff * imgWidth / (float)imgHeight;

    vec3 TL = -forward + up * upOff - right * rightOff;
    vec3 TR = -forward + up * upOff + right * rightOff;
    vec3 BR = -forward - up * upOff + right * rightOff;
    vec3 BL = -forward - up * upOff - right * rightOff;

    // set rays in a sequential style // TODO: parallelism
    for (int y = 0; y < imgHeight; y++)
    {
        for (int x = 0; x < imgWidth; x++)
        {
            float a = x / (float)(imgWidth - 1);
            float b = y / (float)(imgHeight - 1);
            vec3 dir = normalize((1 - a) * (1 - b) * TL + (1 - b) * a * TR + (1 - a) * b * BL + a * b * BR);
            rays.at(y * imgWidth + x) = Ray(this->pos, dir);
        }
    }
}

void Camera::Draw(Scene &scene, Screen &scr)
{
#ifdef TIMING
    scr.fps();
#endif

    if (this->enableControl)
    {
        controller->Update();
    }

    auto &screen = scr.screen;

    RMShader rmShader(scene);
    rmShader.configure();

    if (SDL_MUSTLOCK(screen))
        SDL_LockSurface(screen);

    for (int y = 0; y < imgHeight; y++)
    {
        for (int x = 0; x < imgWidth; x++)
        {
            int rayIndex = y * imgWidth + x;
            Ray &ray = rays[rayIndex];

            vec3 color = rmShader.render(ray);

            scr.put(x, y, Vabs(color));
        }
    }

    if (SDL_MUSTLOCK(screen))
        SDL_UnlockSurface(screen);

    SDL_UpdateRect(screen, 0, 0, 0, 0);
}
