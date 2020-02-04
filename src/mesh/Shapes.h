/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#ifndef OPENGLHOUSE_SHAPES_H
#define OPENGLHOUSE_SHAPES_H

#include <vector>
#include "Vertex.h"

class Shapes
{
public:
    static void GenCube(std::vector<Vertex>& outVertices, std::vector<std::uint32_t>& outIndices, float size = 1.0f);
    static void GenSphere( std::vector<Vertex>& outVertices, std::vector<std::uint32_t>& outIndices, float radius, int stacks, int slices);
    static void GenCylinder(std::vector<Vertex>& outVertices, std::vector<std::uint32_t>& outIndices,
                            float height, float radius, int stacks, int slices, float sweep = 360.0f, bool capped = true);
};


#endif //OPENGLHOUSE_SHAPES_H
