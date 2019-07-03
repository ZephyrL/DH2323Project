#include "debug.h"

#include "parameters.h"

#ifdef DEBUG
#include <glm/gtx/string_cast.hpp> // for test
#endif

#ifdef SCREENSHOT
#include <ctime>
#endif // SCREENSHOT

#include <string>

using namespace std;
using namespace glm;

#include "scene.h"
#include "screen.h"
#include "camera.h"
#include "ray.hpp"

vector<string> fileList = {
    "./data/bun_zipper_res3.ply"};

int main()
{
    Scene scene;
    scene.LoadModels(fileList);

    if (scene.models[0].meshes[0].vertices[0].Normal != vec3(0, 0, 0))
    {
        // cout << glm::to_string(scene.models[0].meshes[0].vertices[0].Normal) << endl;
    }
    else
    {
        cout << "No normal" << endl;
    }

#ifdef DEBUG
    for (auto &m : scene.models)
    {
        for (auto &mesh : m.meshes)
        {
            cout << "Bounxing box: " << endl;
            cout << glm::to_string(mesh.boundingBox.min) << endl
                 << glm::to_string(mesh.boundingBox.max) << endl;
        }
    }
#endif // DEBUG output bbox of scene

    Screen screen(SCREEN_WIDTH, SCREEN_HEIGHT);
    Camera cam(CAM_POS);
    cam.SetTarget(CAM_TARGET);
    // cam.setHead(vec3(0, -1, 0));
    cam.SetImagePlane(screen);

#ifdef DEBUG
    cout << "Num of Rays: " << cam.rays.size() << endl;
    vec3 sum(0, 0, 0);
    for (auto &ray : cam.rays)
    {
        sum += ray.dir;
    }
    cout << "Average direction : " << glm::to_string(normalize(sum)) << endl;
#endif // DEBUG test camera ray generation

    cam.SetControl(true);

    while (screen.NotQuit())
    {
        cam.Draw(scene, screen);
    }

#ifdef SCREENSHOT
    time_t rawTime;
    time(&rawTime);
    struct tm *timeInfo = localtime(&rawTime);
    char buffer[256];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeInfo);
    string str(buffer);
    str = "./shots/" + str + ".bmp";
    screen.save(str);
#endif // SHOT

    return 0;
}