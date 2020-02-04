/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#ifndef OPENGLHOUSE_OMNILIGHT_H
#define OPENGLHOUSE_OMNILIGHT_H

#include <glm/glm.hpp>
#include <array>
#include <cstddef>

//! *FOR NOW*, pos.xyz is position and pos.w is far clipping plane.
class OmniLight
{
private:
    struct omnilight_data
    {
        // TODO: a vec3 and then a float for far clipping plane should also work instead of a vec4
        glm::vec4 pos;
        glm::vec4 diffuse;
        glm::vec4 specular;
    } m_data;
    glm::vec2 m_shadowDimensions;
    std::array<glm::mat4, 6> m_shadowPVs;
    void updateShadowPVs();

public:
    static constexpr std::size_t datasize = (sizeof(OmniLight::omnilight_data) + sizeof(glm::vec4) - 1) & -sizeof(glm::vec4);
    static constexpr std::size_t shadowPVsSize = sizeof(glm::mat4)*6;
    OmniLight(glm::vec3 pos, float farClipPlane, glm::vec4 diffuse, glm::vec4 specular, glm::vec2 shadowDimensions = {1, 1});

    void setShadowDimensions(glm::vec2 shadowDimensions);
    glm::vec2 getShadowDimensions();
    void setPos(glm::vec3 pos);
    glm::vec3 getPos();
    void setFarClipPlane(float farClipPlane);
    float getFarClipPlane();
    void setDiffuse(glm::vec4 diffuse);
    glm::vec4 getDiffuse();
    void setSpecular(glm::vec4 specular);
    glm::vec4 getSpecular();
    const OmniLight::omnilight_data& getData() const;
    const std::array<glm::mat4, 6>& getShadowPVs() const;
};


#endif //OPENGLHOUSE_OMNILIGHT_H
