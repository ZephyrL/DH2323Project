#ifndef _MESH_HPP_
#define _MESH_HPP_

#include <glm/glm.hpp>
#include <vector>
#include <string>

using namespace glm;
using namespace std;

#include "boundingbox.hpp"

// forward declaration
class DistanceFieldData;

struct Vertex
{
    glm::vec3 Position;  // position
    glm::vec3 Normal;    // normal
    glm::vec2 TexCoords; // texCoords
    glm::vec3 Tangent;   // tangent
    glm::vec3 Bitangent; // bitangent
};

struct Texture
{
    unsigned int id; // id of texture (globally)
    string type;     // formate of input file
    string path;     // path to texture
};

struct Transform
{
    float scale; // uniform scaling
    mat3 rotate;
    vec3 translate;

    Transform(float s = 1.0f, mat3 r = mat3(1.0f), vec3 t = vec3(1.0f)) : scale(s), rotate(r), translate(t) {}
};

class Mesh
{
public:
    /*  Mesh Data  */
    string name;    // path to the file of this mesh
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    // unsigned int VAO; // Deprecated, used for OpenGL

    Transform worldTransform; // can be extended to vector<> if instantialized
    BBox boundingBox;   // bounding box of mesh

    DistanceFieldData *distanceFieldData;   // mesh distance field data

    // constructor
    Mesh(string path, vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures,
         Transform transform = Transform())
    {
        this->name = path;
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        // now that we have all the required data, set the vertex buffers and its attribute pointers.
        // setupMesh(); // Deprecated, used to setup data in OpenGL pipeline
        this->worldTransform = transform;

        CheckNormal(); // in case some mesh doesn't have normals
        CalculateBoundingBox();
        GenerateDistanceField();
    }

    // render the mesh <= DRAW function here is only valid for pipelining render
    // void Draw(Shader shader)
    // {
    //     // Old demo draw function: https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/mesh.h
    // }

private:
    void CheckNormal();
    void CalculateBoundingBox();
    void GenerateDistanceField();
};

#endif // !_MESH_HPP_
