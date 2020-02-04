/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */
#ifndef OPENGLHOUSE_MATH_H
#define OPENGLHOUSE_MATH_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "../camera/Camera.h"

//! rotationBetween(u,v) and orthoVector(v) adapted from pseudocode from
//! https://stackoverflow.com/a/11741520/4752083

namespace math
{
    static constexpr glm::vec3 X_AXIS(1.0f, 0.0f, 0.0f);
    static constexpr glm::vec3 Y_AXIS(0.0f, 1.0f, 0.0f);
    static constexpr glm::vec3 Z_AXIS(0.0f, 0.0f, 1.0f);

    /// Calculates the quaternion rotation between two vectors
        // Calculates quaternion that is twice the required rotation
        // and find the halfway between that and 0
    glm::quat rotationBetween(glm::vec3 u, glm::vec3 v);

    /// Calculates a orthogonal vector to v
        // aka creates a vector at right angles to v
    glm::vec3 orthogonalVector(glm::vec3 v);

    /// Squared length of vector
    float length2(glm::vec3 v);

    namespace mirror
    {
        /// Calculate the viewing orientation for a mirror reflection
        glm::quat reflectOrient(const Camera& mirror, const Camera& viewer);
    }

    /// Ray casting object detection
    struct Ray
    {
        glm::vec3 origin = glm::vec3(0.0f);
        glm::vec3 direction = glm::vec3(0.0f);
    };
    struct Box
    {
        glm::vec3 min = glm::vec3(0.0f);
        glm::vec3 max = glm::vec3(0.0f);
    };

//    bool boundingBoxIntersect(glm::vec3 origin, glm::vec3 direction, glm::vec3 aabbMin, glm::vec3 aabbMax, glm::mat4 Model, float &intersect_dist);
    glm::vec2 intersectBox(const Ray& ray, const Box& obb);
    /// Get 3D ray cast from mouse position
    math::Ray mouseRay(const glm::vec2 &mousePos, const glm::vec2 &windowDimensions, const Camera &camera, const glm::mat4 &projMat);
}


#endif //OPENGLHOUSE_MATH_H
