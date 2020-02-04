/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#ifndef OPENGLHOUSE_TRANSFORMABLE_H
#define OPENGLHOUSE_TRANSFORMABLE_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include <utility>

class Transformation
{
public:
    explicit Transformation(Transformation* parent = nullptr);

    void resetTransformations();

/// Parent transformation object to be multiplied into final calculations
    Transformation* getParent();
    void setParent(Transformation* parent);

/// Relatively scaled base size
    glm::vec3 getSize() const;
    const glm::vec3& getBaseSize() const;
    void setBaseSize(const glm::vec3& baseSize);

    /// Global transformations
    void scaleGlobal(const glm::vec3& scale);
    void setGlobalScale(const glm::vec3& scale);

    void rotateGlobal(const glm::quat& orientation);
    void rotateGlobal(float angle, const glm::vec3& axis);
    void setGlobalOrientation(const glm::quat& orientation);
    void setGlobalOrientation(float angle, const glm::vec3& axis);

    void translateGlobal(const glm::vec3& translation);
    void setGlobalPosition(const glm::vec3& position);

    // Relative to world origin. Object may not actually be at this
        // scale/orientation/position depending on local transformations
    const glm::vec3& getGlobalScale() const;
    const glm::quat& getGlobalOrientation() const;
    const glm::vec3& getGlobalPosition() const;

/// Local transformations
    void scaleLocal(const glm::vec3& scale);
    void setLocalScale(const glm::vec3& scale);

    void rotateLocal(const glm::quat& orientation);
    void rotateLocal(float angle, const glm::vec3& axis);
    void setLocalOrientation(const glm::quat& orientation);
    void setLocalOrientation(float angle, const glm::vec3& axis);

    void translateLocal(const glm::vec3& translation);
    void setLocalPosition(const glm::vec3& position);

    // Relative to objects local origin. Object may not be actually be at this
        // scale/orientation/position depending on local transformations
    const glm::vec3& getLocalScale() const;
    const glm::quat& getLocalOrientation() const;
    const glm::vec3& getLocalPosition() const;

/// Combined global and local transformations
    glm::vec3 getScale() const;
    glm::quat getOrientation() const;
    glm::vec3 getPosition() const;

    // Global Transformation Matrix
    glm::mat4 getGlobalMatrix() const;
    // Local Transformation Matrix
    glm::mat4 getLocalMatrix() const;
    // Combined Global and Local Transformations Matrix
    glm::mat4 getMatrix(bool includeParent = true) const;

private:
    Transformation* m_parent;
    glm::vec3 m_baseSize;

/// Global transformations
    glm::vec3 m_globalScale;
    glm::quat m_globalOrientation;
    glm::vec3 m_globalPosition;

/// Local transformations
    glm::vec3 m_localScale;
    glm::quat m_localOrientation;
    glm::vec3 m_localPosition;
};


#endif //OPENGLHOUSE_TRANSFORMABLE_H
