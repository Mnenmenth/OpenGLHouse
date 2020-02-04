/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#ifndef OPENGLHOUSE_POINTLIGHT_H
#define OPENGLHOUSE_POINTLIGHT_H

#include <glm/glm.hpp>
#include <cstddef>

namespace PointLight
{
    struct PointLight
    {
        // Light projection view matrix
        glm::mat4 lpv;
        glm::vec4 position;

        glm::vec4 diffuse;
        glm::vec4 specular;
    };
    static constexpr std::size_t datasize = (sizeof(PointLight) + sizeof(glm::vec4) - 1) & -sizeof(glm::vec4);
}

#endif //OPENGLHOUSE_POINTLIGHT_H
