/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "OmniLight.h"

OmniLight::OmniLight(glm::vec3 pos, float farClipPlane, glm::vec4 diffuse, glm::vec4 specular, glm::vec2 shadowDimensions) :
        m_data({glm::vec4(pos, farClipPlane), diffuse, specular}),
        m_shadowDimensions(shadowDimensions)
{
    updateShadowPVs();
}

void OmniLight::updateShadowPVs()
{
    // Ensure no div by 0 errors
    if(m_shadowDimensions.y == 0) m_shadowDimensions.y = 1;

    /// Create projection view matrices for "looking" at each of the faces of the cubemap

    static constexpr std::array<glm::vec3, 6> atVecs =
            {
                glm::vec3( 1.0f,  0.0f,  0.0f),
                glm::vec3(-1.0f,  0.0f,  0.0f),
                glm::vec3( 0.0f,  1.0f,  0.0f),
                glm::vec3( 0.0f, -1.0f,  0.0f),
                glm::vec3( 0.0f,  0.0f,  1.0f),
                glm::vec3( 0.0f,  0.0f, -1.0f)
            };
    static constexpr std::array<glm::vec3, 6> upVecs
            {
                glm::vec3( 0.0f, -1.0f,  0.0f),
                glm::vec3( 0.0f, -1.0f,  0.0f),
                glm::vec3( 0.0f,  0.0f,  1.0f),
                glm::vec3( 0.0f,  0.0f, -1.0f),
                glm::vec3( 0.0f, -1.0f,  0.0f),
                glm::vec3( 0.0f, -1.0f,  0.0f)
            };

    glm::mat4 proj = glm::perspective(glm::radians(90.0f), m_shadowDimensions.x / m_shadowDimensions.y, 1.0f, m_data.pos.w);
    glm::vec3 pos = m_data.pos;

    for(int face = 0; face < 6; ++face)
    {
        m_shadowPVs[face] = proj * glm::lookAt(pos, pos + atVecs[face], upVecs[face]);
    }
}

void OmniLight::setShadowDimensions(glm::vec2 shadowDimensions)
{
    m_shadowDimensions = shadowDimensions;
    updateShadowPVs();
}
glm::vec2 OmniLight::getShadowDimensions() { return m_shadowDimensions; }

void OmniLight::setPos(glm::vec3 pos)
{
    m_data.pos.x = pos.x;
    m_data.pos.y = pos.y;
    m_data.pos.z = pos.z;
    updateShadowPVs();
}
glm::vec3 OmniLight::getPos() { return glm::vec3(m_data.pos); }

void OmniLight::setFarClipPlane(float farClipPlane) { m_data.pos.w = farClipPlane; }
float OmniLight::getFarClipPlane() { return m_data.pos.w; }

void OmniLight::setDiffuse(glm::vec4 diffuse) { m_data.diffuse = diffuse; }
glm::vec4 OmniLight::getDiffuse() { return m_data.diffuse; }

void OmniLight::setSpecular(glm::vec4 specular) { m_data.specular = specular; }
glm::vec4 OmniLight::getSpecular() { return m_data.specular; }

const OmniLight::omnilight_data& OmniLight::getData() const { return m_data; }

const std::array<glm::mat4, 6>& OmniLight::getShadowPVs() const { return m_shadowPVs; }