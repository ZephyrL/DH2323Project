#ifndef _SCENE_H_
#define _SCENE_H_

#include <string>
#include <iostream>

#include "light.hpp"
#include "model.h"
#include "mesh.h"

using namespace std;

class Model;

class Scene
{
private:
    /* data */
public:
    vector<Model> models;
    vector<Light> lights;

public:
    bool LoadModels(vector<string> &fileList);
    bool LoadLights(); // TODO: design light clas

public:
    Scene(/* args */){};
    ~Scene(){};
};

#endif // !_SCENE_H_