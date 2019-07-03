#ifndef _DISTANCE_FIELD_H_
#define _DISTANCE_FIELD_H_

#include "parameters.h"

#include <vector>
#include <glm/glm.hpp>
#include <fstream>

using namespace std;
using namespace glm;

#include "boundingbox.hpp"

// forward declarations
class Mesh;
class BBox;

class DistanceFieldData
{
private:
    /* data */
private:
    void CheckExistDistanceFieldData();

    void CalculateDistanceField();

    void CalculateDistanceFieldLayer(
        vector<vec3> &sampleRays,
        int zIndex);

    void PostDistanceFieldCalculation();

    bool RayMeshIntersection(
        const vec3 &ori,
        const vec3 &dir,
        float &minDist,
        ivec3 &hitIndex,
        vec3 &hitNormal);


public:
    const class Mesh &mesh; // referece to the mesh it represents

    size_t hashedName;  // a sequence of number as hash(name)

    ivec3 size; // Dimensions of grid: (x, y, z)

    float resolution;   // resolution of DF on generation

    BBox distanceFieldBBox; // bounding box of DF, bit larger

    vec2 minMaxDistance;    // reserved data field
    // distance field data // might be compressed
    vector<float> uncompressedVolumeData;
    // keep this part to reduce time of position retrival during SDF query
    vector<vec3> positionData;  

public:
    DistanceFieldData(const class Mesh &mesh, float resolution = DF_DEFAULT_RESOLUTION)
        : mesh(mesh), resolution(resolution)
    {
        // hash the name of mesh, check if there is cooked data.
        // if the distance field of this mesh has already been generate, load it
        // otherwise CalculateDistanceField() is called sequentially.
        CheckExistDistanceFieldData();
    };
    ~DistanceFieldData(){};

    friend ofstream& operator<<(ofstream& o, const DistanceFieldData& data);
    friend ifstream& operator>>(ifstream& i, DistanceFieldData& data);
};

#endif // !_DISTANCE_FIELD_HPP_
