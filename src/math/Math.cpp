/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */
#include "Math.h"

glm::quat math::rotationBetween(glm::vec3 u, glm::vec3 v)
{
    float cosTheta = glm::dot(u, v);
    float sqrLen = glm::sqrt(length2(u) * length2(v));

    if(cosTheta / sqrLen == -1)
    {

        return glm::quat(0, glm::normalize(orthogonalVector(u)));
    }

    return glm::normalize(glm::quat(cosTheta + sqrLen, glm::cross(u, v)));
}

glm::vec3 math::orthogonalVector(glm::vec3 v)
{
    float x = glm::abs(v.x);
    float y = glm::abs(v.y);
    float z = glm::abs(v.z);

    glm::vec3 other = x < y ? (x < z ? X_AXIS : Z_AXIS) : (y < z ? Y_AXIS : Z_AXIS);
    return glm::cross(v, other);
}

float math::length2(glm::vec3 v)
{
    return v.x*v.x + v.y*v.y + v.z*v.z;
}

glm::quat math::mirror::reflectOrient(const class Camera & mirror, const class Camera & viewer)
{

    // Get the direction vector of mirror->viewer
    glm::vec3 viewDir = glm::normalize(mirror.getEyePos() - viewer.getEyePos());
    // Calculate the reflection angle and invert it
        // (It has to be inverted otherwise the direction is straight back at the viewer.
        // Possibly something I'm doing something wrong to cause that)
    glm::vec3 reflectDir = -1.0f * glm::reflect(viewDir, mirror.getWorldForward());
    // Invert Z component so mirror isn't face the wrong way
    reflectDir.z *= -1.0f;

    // Calculate the rotation between the mirror normal and the reflection direction
    return math::rotationBetween(mirror.getWorldForward(), reflectDir);
}

//bool math::boundingBoxIntersect(glm::vec3 origin, glm::vec3 direction, glm::vec3 aabbMin, glm::vec3 aabbMax, glm::mat4 Model, float & intersect_dist)
//{
//    float testMin = 0.0f;
//    float testMax = 100000.0f;
//    glm::vec3 obbWorldSpace(Model[3]);
//    glm::vec3 delta = obbWorldSpace - origin;
//
//    for(int i = 0; i < 3; ++i)
//    {
//        glm::vec3 axis(Model[i]);
//        float e = glm::dot(axis, delta);
//        float f = glm::dot(direction, axis);
//
//        if(glm::abs(f) > 0.001f)
//        {
//            float t1 = (e+aabbMin[i])/f;
//            float t2 = (e+aabbMax[i])/f;
//
//            if(t1 > t2)
//            {
//                float w = t1;
//                t1 = t2;
//                t2 = w;
//            }
//
//            if(t2 < testMax)
//            {
//                testMax = t2;
//            }
//
//            if(t1 > testMin)
//            {
//                testMin = t1;
//            }
//
//            if(testMax < testMin)
//            {
//                return false;
//            }
//        }
//        else
//        {
//            if(-e + aabbMin[i] > 0.0f || -e + aabbMax[i] < 0.0f)
//            {
//                return false;
//            }
//        }
//    }
//
//    intersect_dist = testMin;
//    return true;
//}

//void math::mouseToRay(const glm::vec2 &mousePos, const glm::vec2 &windowDimensions, const glm::mat4 &ViewMat,
//                      const glm::mat4 &ProjMat, glm::vec3 &origin, glm::vec3 &direction)
//{
//    glm::vec4 rayStart(
//            ((mousePos.x / static_cast<float>(windowDimensions.x) - 0.5f) * 2.0f),
//            ((mousePos.y / static_cast<float>(windowDimensions.y) - 0.5f) * 2.0f),
//            -1.0f,
//            1.0f
//            );
//    glm::vec4 rayEnd(
//            ((mousePos.x / static_cast<float>(windowDimensions.x) - 0.5f) * 2.0f),
//            ((mousePos.y / static_cast<float>(windowDimensions.y) - 0.5f) * 2.0f),
//            0.0f,
//            1.0f
//    );
//
//    glm::mat4 M = glm::inverse(ProjMat * ViewMat);
//    glm::vec4 rayStartWorld = M * rayStart;
//    rayStartWorld /= rayStartWorld.w;
//    glm::vec4 rayEndWorld = M * rayEnd;
//    rayEndWorld /= rayEndWorld.w;
//
//    glm::vec3 rayDirWorld(glm::normalize(rayEndWorld - rayStartWorld));
//
//    origin = glm::vec3(rayStartWorld);
//    direction = rayDirWorld;
//}

//https://github.com/PacktPublishing/OpenGL-Build-High-Performance-Graphics/blob/master/Module%201/Chapter02/Picking_SceneIntersection/Picking_SceneIntersection/main.cpp
//https://books.google.com/books?id=inc5DwAAQBAJ&pg=PA77&lpg=PA77&dq=glm::unProject+intersecting+rays&source=bl&ots=oJlw1ZhPYC&sig=ACfU3U2AjcZ89LmSzVha3B67l_iizmdbLQ&hl=en&sa=X&ved=2ahUKEwiavK6JwqTmAhVCu1kKHcPYB_EQ6AEwB3oECAoQAQ#v=onepage&q=glm%3A%3AunProject%20intersecting%20rays&f=false
glm::vec2 math::intersectBox(const math::Ray &ray, const math::Box &obb)
{
    glm::vec3 inv_dir = 1.0f / ray.direction;
    glm::vec3 tMin = (obb.min - ray.origin) * inv_dir;
    glm::vec3 tMax = (obb.max - ray.origin) * inv_dir;
    glm::vec3 t1 = glm::min(tMin, tMax);
    glm::vec3 t2 = glm::max(tMin, tMax);
    float tNear = glm::max(glm::max(t1.x, t1.y), t1.z);
    float tFar = glm::min(glm::min(t2.x, t2.y), t2.z);
    return glm::vec2(tNear, tFar);
}

math::Ray math::mouseRay(const glm::vec2 &mousePos, const glm::vec2 &windowDimensions, const Camera &camera, const glm::mat4 &projMat)
{
    glm::vec3 win = glm::vec3(mousePos.x, windowDimensions.x - mousePos.y, 0.0f);
    glm::vec4 viewport = glm::vec4(0.0f, 0.0f, windowDimensions.x, windowDimensions.y);
    glm::mat4 viewMat = camera.getViewMatrix();
    glm::vec3 start = glm::unProject(win, viewMat, projMat, viewport);
    win.z = 1.0f;
    glm::vec3 end = glm::unProject(win, viewMat, projMat, viewport);

    return {camera.getWorldEyePos(), glm::normalize(end-start)};
}