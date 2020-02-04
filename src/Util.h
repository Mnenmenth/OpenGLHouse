/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */
#ifndef OPENGLHOUSE_UTIL_H
#define OPENGLHOUSE_UTIL_H

#include <glm/glm.hpp>
#define printVec3(vec) std::cout << "(x->" << vec.x << ", y->" << vec.y << ", z->" << vec.z << ")"
#define convBlenderVec_m(vec3) {vec3.x, vec3.z, -vec3.y}

namespace util
{
    /// Rotate a vector from blender world space to opengl world space
    glm::vec3 convBlenderVec(const glm::vec3 &v)
    {
        return convBlenderVec_m(v);
    }
}
#endif //OPENGLHOUSE_UTIL_H
