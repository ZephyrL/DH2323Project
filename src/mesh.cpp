#include "mesh.h"

#include <glm/glm.hpp>
#include <vector>

using namespace std;
using namespace glm;

#include "distancefield.h"

void Mesh::CheckNormal()
{
    if (this->vertices.size() == 0)
        return;
    if (this->vertices[0].Normal != vec3(0, 0, 0))
        return;
    else
    {
        // calculate normals
        for (int idx = 0; idx < indices.size(); idx += 3)
        {
            unsigned int i0 = indices[idx];
            unsigned int i1 = indices[idx + 1];
            unsigned int i2 = indices[idx + 2];

            Vertex& v0 = vertices[i0];
            Vertex& v1 = vertices[i1];
            Vertex& v2 = vertices[i2];

            vec3 e1 = v1.Position - v0.Position;
            vec3 e2 = v2.Position - v0.Position;

            // normal of current plane
            vec3 normal = normalize(cross(e1, e2));
            v0.Normal = v1.Normal = v2.Normal = normal;
            // if the mesh has no normals, it's likely it neither have tangents 
            // and bitangents, we then use bitangent.x as a counter,
            // which counts the number of reference of each vertex, so that the normals are smooth.
            // v0.Normal = (v0.Normal * v0.Bitangent.x + normal) / (v0.Bitangent.x + 1);
            // v1.Normal = (v1.Normal * v1.Bitangent.x + normal) / (v1.Bitangent.x + 1);
            // v2.Normal = (v2.Normal * v2.Bitangent.x + normal) / (v2.Bitangent.x + 1);
            // v0.Bitangent = vec3(v0.Bitangent.x + 1, 0 ,0);  
            // v1.Bitangent = vec3(v1.Bitangent.x + 1, 0 ,0); 
            // v2.Bitangent = vec3(v2.Bitangent.x + 1, 0 ,0); 
        }
    }
}

void Mesh::CalculateBoundingBox()
{
    if (vertices.size() <= 0)
    {
        // error occurred
        return;
    }

    float minX, minY, minZ;
    float maxX, maxY, maxZ;

    maxX = minX = vertices[0].Position.x;
    maxY = minY = vertices[0].Position.y;
    maxZ = minZ = vertices[0].Position.z;

    for (auto &v : vertices)
    {
        if (v.Position.x > maxX)
            maxX = v.Position.x;
        if (v.Position.x < minX)
            minX = v.Position.x;
        if (v.Position.y > maxY)
            maxY = v.Position.y;
        if (v.Position.y < minY)
            minY = v.Position.y;
        if (v.Position.z > maxZ)
            maxZ = v.Position.z;
        if (v.Position.z < minZ)
            minZ = v.Position.z;
    }

    this->boundingBox.min = vec3(minX, minY, minZ);
    this->boundingBox.max = vec3(maxX, maxY, maxZ);
}

void Mesh::GenerateDistanceField()
{
    this->distanceFieldData = new DistanceFieldData(*this);
}