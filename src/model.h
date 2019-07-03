#ifndef _MODEL_H_
#define _MODEL_H_

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;
using namespace glm;

#include "mesh.h"

// forward declaration
struct Vertex;
struct Texture;
class Mesh;

class Model
{
public:
    /*  Model Data */
    vector<Texture> textures_loaded; // stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<Mesh> meshes;
    string directory;
    bool gammaCorrection;

    /*  Functions   */
    // constructor, expects a filepath to a 3D model.
    Model(string const &path, bool gamma = false) : gammaCorrection(gamma)
    {
        loadModel(path);
    }

    // draws the model, and thus all its meshes
    // void Draw(Shader shader)
    // {
    //     for (unsigned int i = 0; i < meshes.size(); i++)
    //         meshes[i].Draw(shader);
    // }

private:
    void loadModel(string path);
    void processNode(string path, aiNode *node, const aiScene *scene);
    Mesh processMesh(string path, aiMesh *mesh, const aiScene *scene);
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                         string typeName);
    unsigned int TextureFromFile(const char *path, const string &directory, bool gamma);
};


#endif // !_MODEL_H_