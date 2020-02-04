/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#ifndef OPENGLHOUSE_MESH_H
#define OPENGLHOUSE_MESH_H

#include <vector>
#include "Vertex.h"
#include "Material.h"

struct Mesh
{
    std::vector<Vertex> vertices;
    std::vector<std::uint32_t> indices;
    std::vector<Material> materials;

    static void LoadMesh(Mesh& out, const char* filePath);
};

#endif //OPENGLHOUSE_MESH_H
