/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#include "Mesh.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <unordered_map>
#include <iostream>

void Mesh::LoadMesh(Mesh& out, const char *filePath)
{
    if(filePath == nullptr)
    {
        std::cout << "filePath in LoadMesh() must not be null" << std::endl;
    }

    out.vertices.clear();
    out.indices.clear();

    // Model loading modified from https://vulkan-tutorial.com/Loading_models
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    std::cout << "Loading mesh from file: " << filePath << std::endl;

    if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filePath))
    {
        if(!warn.empty() && err.empty())
        {
            std::cout << "\tWarning: " << warn << std::endl;
        }
        else if(!err.empty() && warn.empty())
        {
            std::cout << "\tError: " << err << std::endl;
        }
        else
        {
            std::cout << "\tWarning: " << warn << std::endl;
            std::cout << "\tError: " << err << std::endl;
        }
    }


    // Load vertex information, removing duplicate vertices
    std::unordered_map<Vertex, std::uint32_t> uniqueVertices = {};
    std::unordered_map<std::string, std::vector<std::uint32_t>> uniqueMaterials = {};

//    for(const auto& shape : shapes)
//    {
//        for(const auto& index : shape.mesh.indices)
//        {
//            Vertex vertex = {
//                    { // Vertex pos
//                            attrib.vertices[3 * index.vertex_index + 0],
//                            attrib.vertices[3 * index.vertex_index + 1],
//                            attrib.vertices[3 * index.vertex_index + 2]
//                    },
//                    { // Normal
//                            attrib.normals[3 * index.normal_index + 0],
//                            attrib.normals[3 * index.normal_index + 1],
//                            attrib.normals[3 * index.normal_index + 2]
//                    },
//                    { // UV coords with flipped Y
//                            attrib.texcoords[2 * index.texcoord_index + 0],
//                            1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
//                    }
//            };
//            //materials[0].name
//            if(uniqueVertices.count(vertex) == 0)
//            {
//                uniqueVertices[vertex] = static_cast<std::uint32_t>(out.vertices.size());
//                out.vertices.push_back(vertex);
//            }
//
//            out.indices.push_back(uniqueVertices[vertex]);
//        }
//    }

    for(const auto &shape : shapes)
    {
        std::size_t indexOffset = 0;

        for(std::size_t f = 0; f < shape.mesh.num_face_vertices.size(); ++f)
        {
            int faceVerts = shape.mesh.num_face_vertices[f];

            int materialID = shape.mesh.material_ids[f];

            for(std::size_t v = 0; v < faceVerts; ++v)
            {
                auto index = shape.mesh.indices[indexOffset + v];
                Vertex vertex = {
                        { // Vertex pos
                                attrib.vertices[3 * index.vertex_index + 0],
                                attrib.vertices[3 * index.vertex_index + 1],
                                attrib.vertices[3 * index.vertex_index + 2]
                        },
                        { // Normal
                                attrib.normals[3 * index.normal_index + 0],
                                attrib.normals[3 * index.normal_index + 1],
                                attrib.normals[3 * index.normal_index + 2]
                        },
                        { // UV coords with flipped Y
                                attrib.texcoords[2 * index.texcoord_index + 0],
                                1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                        }
                };

                if(uniqueVertices.count(vertex) == 0)
                {
                    uniqueVertices[vertex] = static_cast<std::uint32_t>(out.vertices.size());
                    out.vertices.push_back(vertex);
                }

                out.indices.push_back(uniqueVertices[vertex]);
            }

            indexOffset += faceVerts;

        }
    }

    std::cout << "\tMesh loaded successfully" << std::endl;
}