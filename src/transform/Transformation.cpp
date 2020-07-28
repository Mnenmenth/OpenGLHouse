/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#include "Transformation.h"

Transformation::Transformation(Transformation* parent) : m_parent(parent), m_baseSize(1.0f)
{
    resetTransformations();
}

void Transformation::resetTransformations()
{
    m_globalScale           = glm::vec3(1.0f, 1.0f, 1.0f);
    m_globalOrientation     = glm::angleAxis(0.0f, glm::vec3(0.0f, 0.0f, 0.0f));
    m_globalPosition        = glm::vec3(0.0f, 0.0f, 0.0f);

    m_localScale            = glm::vec3(1.0f, 1.0f, 1.0f);
    m_localOrientation      = glm::angleAxis(0.0f, glm::vec3(0.0f, 0.0f, 0.0f));
    m_localPosition         = glm::vec3(0.0f, 0.0f, 0.0f);
}

/// Parent transformation object to be multiplied into final calculations
Transformation* Transformation::getParent() { return m_parent; }
void Transformation::setParent(Transformation *parent) { m_parent = parent; }

/// Relatively scaled base size
glm::vec3 Transformation::getSize() const { return m_baseSize * getScale(); }
const glm::vec3& Transformation::getBaseSize() const { return m_baseSize; }
void Transformation::setBaseSize(const glm::vec3 &baseSize) { m_baseSize = baseSize; }

/// Global

// Scale
void Transformation::scaleGlobal(const glm::vec3& scale)
{
    // Multiply in the new scale factor
    m_globalScale *= scale;
    // Scale the current position by the new scale factor
    m_globalPosition = m_globalPosition * scale;
}

void Transformation::setGlobalScale(const glm::vec3& scale)
{
    // Remove the current scale factors from global positionx
    m_globalPosition = m_globalPosition / m_globalScale;
    // Set the new scale value
    m_globalScale = scale;
    // Scale the position by the new scale value
    m_globalPosition = m_globalPosition * scale;
}

// Orientation/Rotation

void Transformation::rotateGlobal(const glm::quat& nOrient)
{
    // Apply rotation and normalize quat
    m_globalOrientation = m_globalOrientation * nOrient;
    m_globalOrientation = glm::normalize(m_globalOrientation);

    // Apply rotation to position
    m_globalPosition    = m_globalPosition * glm::inverse(nOrient);
}

void Transformation::rotateGlobal(float angle, const glm::vec3& axis)
{
    // Convert euler angles to quat (making sure axis vector is normalized) and then apply rotation
    rotateGlobal(glm::angleAxis(glm::radians(angle), glm::normalize(axis)));
}

void Transformation::setGlobalOrientation(const glm::quat& orientation)
{
    // Make sure the input orientation is normalized
    glm::quat nOrientation = glm::normalize(orientation);
    // Remove the current orientation from the position
    m_globalPosition = m_globalPosition * glm::normalize(glm::conjugate(m_globalOrientation) * nOrientation);
    // Set new orientation
    m_globalOrientation = nOrientation;
    // Apply new orientation to position
    m_globalPosition = m_globalPosition * nOrientation;
}

void Transformation::setGlobalOrientation(float angle, const glm::vec3& axis)
{
    // Convert euler angles to quat (making sure axis vector is normalized) and then set orientation
    setGlobalOrientation(glm::angleAxis(glm::radians(angle), glm::normalize(axis)));
}

// Translation/Position
void Transformation::translateGlobal(const glm::vec3& translation)
{
    // Add in the new translation
    m_globalPosition += translation;
}

void Transformation::setGlobalPosition(const glm::vec3& position)
{
    // Set the new position
    m_globalPosition = position;
}



const glm::vec3& Transformation::getGlobalScale()        const { return m_globalScale; }
const glm::quat& Transformation::getGlobalOrientation()  const { return m_globalOrientation; }
const glm::vec3& Transformation::getGlobalPosition()     const { return m_globalPosition; }

/// Local

// Scale
void Transformation::scaleLocal(const glm::vec3& scale)
{
    // Multiply in the new scale factor
    m_localScale *= scale;
}

void Transformation::setLocalScale(const glm::vec3& scale)
{
    // Set the new scale factor
    m_localScale = scale;
}

// Rotation/Orientation

void Transformation::rotateLocal(const glm::quat& orientation)
{
    // Apply rotation and normalize quat
    m_localOrientation = m_localOrientation * glm::normalize(orientation);
    m_localOrientation = glm::normalize(m_localOrientation);
}

void Transformation::rotateLocal(float angle, const glm::vec3& axis)
{
    // Convert euler angles to quat (making sure axis vector is normalized) and then apply rotation
    rotateLocal(glm::angleAxis(glm::radians(angle), glm::normalize(axis)));
}

void Transformation::setLocalOrientation(const glm::quat& orientation)
{
    // Set new orientation, making sure the new quat is normalized
    m_localOrientation = glm::normalize(orientation);
}

void Transformation::setLocalOrientation(float angle, const glm::vec3& axis)
{
    // Convert euler angles to quat (making sure axis vector is normalized) and then set new orientation
    setLocalOrientation(glm::angleAxis(glm::radians(angle), glm::normalize(axis)));
}

// Translation/Position

void Transformation::translateLocal(const glm::vec3& translation)
{
    // Apply new translation
    m_localPosition += translation;
}

void Transformation::setLocalPosition(const glm::vec3& position)
{
    // Set the new position
    m_localPosition = position;
}

const glm::vec3& Transformation::getLocalScale()         const { return m_localScale; }
const glm::quat& Transformation::getLocalOrientation()   const { return m_localOrientation; }
const glm::vec3& Transformation::getLocalPosition()      const { return m_localPosition; }

/// Combined transformations
glm::vec3 Transformation::getScale()         const { return m_globalScale * m_localScale; }
glm::quat Transformation::getOrientation()   const { return m_globalOrientation * m_localOrientation; }
glm::vec3 Transformation::getPosition()      const { return m_globalPosition + m_localPosition; }

glm::mat4 Transformation::getGlobalMatrix() const
{
    glm::mat4 Global(1.0f);
    Global = glm::translate(Global, m_globalPosition);
    Global = Global * glm::mat4_cast(m_globalOrientation);
    Global = glm::scale(Global, m_globalScale);

    return Global;
}

glm::mat4 Transformation::getLocalMatrix() const
{
    glm::mat4 Local(1.0f);
    Local = glm::translate(Local, m_localPosition);
    Local = Local * glm::mat4_cast(m_localOrientation);
    Local = glm::scale(Local, m_localScale);

    return Local;
}

glm::mat4 Transformation::getMatrix(bool includeParent) const
{
    if(includeParent && m_parent != nullptr)
        return m_parent->getMatrix() * getGlobalMatrix() * getLocalMatrix();

    return getGlobalMatrix() * getLocalMatrix();
}