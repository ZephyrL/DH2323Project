#include "debug.h"

#include "parameters.h"
#include "cameracontroller.h"

#include <SDL.h>
#include <glm/glm.hpp>

#ifdef DEBUG
#include <iostream>
#include <glm/gtx/string_cast.hpp>
using namespace std;
#endif // DEBUG

#include "camera.h"

using namespace glm;

void CameraController::Initialize()
{
    SDL_GetMouseState(&mouseX, &mouseY);
}

void CameraController::Update()
{
    keyState = SDL_GetKeyState(0);

    vec3 camPos = refCam.GetPos();
#ifdef DEBUG
    // cout << glm::to_string(camPos) << endl;

#endif // DEBUG

    vec3 offset(0);

    if (keyState[SDLK_UP] || keyState[SDLK_w])
    {
        // up
        offset += vec3(0, 1, 0) * flexibility;
    }
    if (keyState[SDLK_DOWN] || keyState[SDLK_s])
    {
        // down
        offset -= vec3(0, 1, 0) * flexibility;
    }
    if (keyState[SDLK_LEFT] || keyState[SDLK_a])
    {
        // left
        offset -= vec3(1, 0, 0) * flexibility;
    }
    if (keyState[SDLK_RIGHT] || keyState[SDLK_d])
    {
        // right
        offset += vec3(1, 0, 0) * flexibility;
    }
    if (keyState[SDLK_q])
    {
        // foward
        offset -= vec3(0, 0, 1) * flexibility;
    }
    if (keyState[SDLK_e])
    {
        // backward
        offset += vec3(0, 0, 1) * flexibility;
    }

    if (offset != vec3(0))
    {
        refCam.SetPos(camPos + offset);
    }

    int curX, curY;
    SDL_GetMouseState(&curX, &curY);
    int dx = curX - mouseX;
    int dy = curY - mouseY;

    if (dx & dy == 0)
        return;
}