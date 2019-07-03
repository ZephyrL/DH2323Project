#include "scene.h"

#include <string>
#include <vector>
#include <iostream>

#include "model.h"

using namespace std;

bool Scene::LoadModels(vector<string> &fileList)
{
    for (string &file : fileList)
    {
        Model temp = Model(file); // error detection within object constructor
        if (temp.meshes.size())
        {
            models.push_back(temp);
        }
        else
        {
            cerr << "Warining: File " << file << " cannot load, break loading." << endl;
            return false;
        }
    }
    return true;
}