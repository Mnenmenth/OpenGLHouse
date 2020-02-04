/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#include "Camera.h"

// Calculate world right and forward based on parameters
Camera::Camera(glm::vec3 EyePos, glm::vec3 LookingAt, glm::vec3 WorldUp) :
        Camera(
                EyePos,
                LookingAt,
                WorldUp,
                LookingAt-EyePos,
                glm::cross(LookingAt-EyePos, WorldUp)
                )
{}

// Just use what is given
Camera::Camera(glm::vec3 EyePos, glm::vec3 LookingAt, glm::vec3 WorldUp, glm::vec3 WorldForward, glm::vec3 WorldRight) :
        m_EyePos(EyePos),
        m_LookingAt(LookingAt),
        m_WorldUp(glm::normalize(WorldUp)),
        m_WorldForward(glm::normalize(WorldForward)),
        m_WorldRight(glm::normalize(WorldRight)),
        m_Orientation(glm::angleAxis(0.0f, glm::normalize(WorldUp))),
        m_Translation(0.0f, 0.0f, 0.0f),
        m_Zoom(0.0f),
        m_ZoomMin(0.01f),
        m_ZoomMax(100.0f)
{}

/// Sets the translation of the world around the camera
void Camera::setWorldTranslation(glm::vec3 translation)
{
    m_Translation = translation;
}

/// Move World
void Camera::moveWorld(glm::vec3 delta)
{
    moveWorldLat(delta.x);
    moveWorldVert(delta.y);
    moveWorldLong(delta.z);
}

void Camera::moveWorldLat(float delta)
{
    m_Translation += (delta * m_WorldRight);
}


void Camera::moveWorldVert(float delta)
{
    m_Translation += (delta * m_WorldUp);
}

void Camera::moveWorldLong(float delta)
{
    m_Translation += (delta * m_WorldForward);
}


/// Rotate World
void Camera::rotateWorldYaw(float theta)
{
    m_Orientation = glm::normalize(m_Orientation * glm::angleAxis(glm::radians(theta), m_WorldUp));
}

void Camera::rotateWorldPitch(float theta)
{
    m_Orientation = glm::normalize(m_Orientation * glm::angleAxis(glm::radians(theta), m_WorldRight));
}

/// Position within world
void Camera::setPosition(glm::vec3 pos)
{
    m_Translation = -1.0f * pos;
}

/// Move Camera
void Camera::move(glm::vec3 delta)
{
    moveLat(delta.x);
    moveVert(delta.y);
    moveLong(delta.z);
}

void Camera::moveLat(float delta)
{
    m_Translation += (delta * m_WorldRight) * m_Orientation;
}

void Camera::moveVert(float delta)
{
    m_Translation += (delta * m_WorldUp) * m_Orientation;
}

void Camera::moveLong(float delta)
{
    m_Translation += (delta * m_WorldForward) * m_Orientation;
}

void Camera::moveLocked(glm::vec3 delta)
{
    moveLatLocked(delta.x);
    moveVertLocked(delta.y);
    moveLongLocked(delta.z);
}

void Camera::moveLatLocked(float delta)
{
    glm::vec3 deltaLat = (delta * m_WorldRight) * m_Orientation;
    deltaLat.y = 0.0f;
    m_Translation += deltaLat;
}

void Camera::moveVertLocked(float delta)
{
    moveWorldVert(-delta);
}

void Camera::moveLongLocked(float delta)
{
    glm::vec3 deltaLong = (delta * m_WorldForward) * m_Orientation;
    deltaLong.y = 0.0f;
    m_Translation += deltaLong;
}

/// Rotate Camera
void Camera::rotate(glm::quat orientation)
{
    m_Orientation = glm::normalize(orientation * glm::normalize(orientation));
}

void Camera::rotateYaw(float theta)
{
    m_Orientation = glm::normalize(m_Orientation * glm::angleAxis(glm::radians(theta), m_WorldUp * m_Orientation));
}

void Camera::rotatePitch(float theta)
{
    m_Orientation = glm::normalize(m_Orientation * glm::angleAxis(glm::radians(theta), m_WorldRight * m_Orientation));
}

void Camera::rotateRoll(float theta)
{
    m_Orientation = glm::normalize(m_Orientation * glm::angleAxis(glm::radians(theta), m_WorldForward * m_Orientation));
}

void Camera::setOrientation(glm::quat newOrient) {
    m_Orientation = glm::normalize(newOrient);
}

void Camera::resetOrientation() {
    m_Orientation = glm::normalize(m_Orientation * glm::inverse(m_Orientation));
}

/// Zoom
void Camera::zoom(float delta)
{
    m_EyePos += delta * m_WorldForward;
    m_Zoom += delta;
}

void Camera::setZoom(float newZoom)
{
    resetZoom();
    m_EyePos += newZoom * m_WorldForward;
    m_Zoom = newZoom;
}

void Camera::setMinZoom(float zoomMin)
{
    m_ZoomMin = zoomMin;
    if(m_Zoom < m_ZoomMin) m_Zoom = m_ZoomMin;
}

void Camera::setMaxZoom(float zoomMax)
{
    m_ZoomMax = zoomMax;
    if(m_Zoom > m_ZoomMax) m_Zoom = m_ZoomMax;
}

void Camera::resetZoom()
{
    m_EyePos -= m_Zoom * m_WorldForward;
    m_Zoom = 0.0f;
}

/// Getters
glm::vec3 Camera::getBaseEyePos()           const { return m_EyePos; }
glm::vec3 Camera::getWorldEyePos()          const { return (((m_EyePos-(m_Zoom*m_WorldForward*2.0f)) * m_Orientation) + m_Translation); } //{ return (m_EyePos * m_Orientation) + m_Translation; }
glm::vec3 Camera::getLookingAt()            const { return m_LookingAt; }
glm::vec3 Camera::getWorldUp()              const { return m_WorldUp; }
glm::vec3 Camera::getWorldForward()         const { return m_WorldForward; }
glm::vec3 Camera::getWorldRight()           const { return m_WorldRight; }
glm::vec3 Camera::getWorldTranslation()     const { return m_Translation; }

glm::vec3 Camera::getEyePos()               const { return -1.0f * getWorldEyePos(); }
glm::vec3 Camera::getPosition()             const { return -1.0f * m_Translation; }
glm::quat Camera::getOrientation()          const { return m_Orientation; }
float Camera::getZoom()                     const { return m_Zoom; }

glm::mat4 Camera::getViewMatrix() const
{
    glm::mat4 view = glm::lookAt(m_EyePos, m_LookingAt, m_WorldUp);
    view = view * glm::mat4_cast(m_Orientation);
    view = glm::translate(view, m_Translation);
    return view;
}