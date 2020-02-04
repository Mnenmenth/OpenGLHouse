/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#ifndef OPENGLHOUSE_VERTEX_H
#define OPENGLHOUSE_VERTEX_H

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <cstddef>

struct Vertex
{
    // Vertex position
    glm::vec3 v;
    // Normal vector
    glm::vec3 n;
    // UV Coordinate
    glm::vec2 t;

    bool operator==(const Vertex& v2) const
    {
        return v == v2.v && n == v2.n && t == v2.t;
    }
};

namespace std
{
    template<> struct hash<Vertex>
    {
        std::size_t operator()(Vertex const& vertex) const
        {
            return ((hash<glm::vec3>()(vertex.v) ^
                     (hash<glm::vec3>()(vertex.n) << 1)) >> 1) ^
                   (hash<glm::vec2>()(vertex.t) << 1);
        }
    };
}

#endif //OPENGLHOUSE_VERTEX_H
