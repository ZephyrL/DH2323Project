#ifndef _PARAMETERS_H_
#define _PARAMETERS_H_

#include <glm/glm.hpp>
#include <vector>
#include <string>
using namespace std;
using namespace glm;

// distancefield.cpp
const int DF_MIN_VOXEL_PER_DIM = 8;     // lower bound: minimum distance field voxels on the shortest axis
const int DF_NUM_SAMPLE_RAYS = 1200;    // number of sample rays when sample distance field of mesh
const float DF_DEFAULT_RESOLUTION = 1.0f;   // resolution of distance field volumn, the higher the better, but time consuming

// RMShader.cpp
const int RM_MAX_ITER = 200;        // ray marching max iteration
const float RM_MAX_DEPTH = 5.f;     // max scene depth (used to cooperate with depth field)
const float RM_ACCURACY = 1e-3;     // threshold, distance smaller than this value will regard as a hit
const float RM_MAX_DIST = 5.f;      // max distance a ray can travel

const vec3 RM_LIGHT_DIR = normalize(vec3(-1));      // global light direction (directional light)

const vec2 RM_SHADOW_DISTANCE = vec2(0.01f, 100.f); // range of the length of shadow ray 
const float RM_SHADOW_PENUMBRA = 50.f;              // shadow penumbra

const float RM_AO_STEP_SIZE = 0.2f;                // ambient occlusion step size
const int RM_AO_MAX_ITER = 3;                       // ambient occlusion max step (the larger of this value, the more shaodow will be accumulated)
const float RM_AO_INTENSITY = 0.22f;                // ambient occlusion intensity

const vec3 DEFAULT_OBJECT_COLOR = vec3(104 / 255.f, 168 / 255.f, 192 / 255.f);  // incase any object in the scene not being assigned a color

// RMshader.cpp
// objects
const vec4 groundPos = vec4(0, 1, 0, 0.015f);       // ground
const vec3 groundColor = vec3(150, 150, 150);

const vec3 rdBoxPos = vec3(-0.3f, 0.1f, 0.f);       // round box
const vec3 rdBoxSize = vec3(0.03f, 0.02f, 0.025f);  // size (x, y, z)
const float rdBoxR = 0.007f;                        // round corner radius
const vec3 rBoxColor = vec3(255, 255, 51);

const vec3 torusPos = vec3(0.2f, 0.1f, 0.f);        // torus
const vec2 torusRad = vec2(0.05f, 0.015f);          // torus large radius
const vec3 torusColor = vec3(255, 153, 51);         // torus pipe radius

const vec3 cyPos = vec3(0.2f, 0.1f, -0.3f);         // round cylinder
const float cyRa = 0.03f;                           // cylinder radius
const float cyRb = 0.01f;                           // round corner radius
const float cyH = 0.03f;                            // cylinder height
const vec3 cyColor = vec3(51, 255, 255);

const vec3 octPos = vec3(-0.3f, 0.1f, -0.3f);       // octahedron
const float octSize = 0.06f;                        // side length
const vec3 octColor = vec3(51, 51, 255);

const vec3 combPos = vec3(-0.05f, 0.1f, -0.3f);     // a combined geometry
const vec3 rdBoxSize2 = vec3(0.025f);               // part 1: a round box
const float sphereR2 = 0.04f;                       // part 2: a sphere
const float subK = 0.005f;                          // geometry = SmoothSubtraction(box - sphere)
const vec3 combColor = vec3(153, 51, 255);

const vec3 bunnyColor = vec3(255, 102, 178);

// main.cpp
const vec3 CAM_POS = vec3(0.25f, 0.65f, 0.55f);     // camera pos
const vec3 CAM_TARGET = vec3(-0.03f, 0.1f, -0.1f);  // camera target
// const vec3 CAM_TARGET = vec3(0.f);
const int SCREEN_WIDTH = 500;                       // screen width
const int SCREEN_HEIGHT = 500;                      // screen height

#endif // !_PARAMETERS_H_
