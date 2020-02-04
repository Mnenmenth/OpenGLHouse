/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#ifndef OPENGLHOUSE_LIGHTRENDERER_H
#define OPENGLHOUSE_LIGHTRENDERER_H


#include "RenderInfo.h"
#include "../lighting/PointLight.h"
#include "../shader/ShaderProgram.h"
#include "../shader/UniformBufferObj.h"
#include "../NonCopyable.h"
#include "../lighting/OmniLight.h"

class LightRenderer : public NonCopyable
{
public:
    LightRenderer();
    ~LightRenderer();

    void setAmbientLight(const glm::vec4& ambientLight);

    void addLight(OmniLight*);
    void removeLight(const OmniLight*);

    void addObject(const RenderInfo*);
    void removeObject(const RenderInfo*);

    void render(const glm::mat4& ProjViewMat, const glm::vec3& EyePos, const glm::vec4& Viewport, std::uint32_t framebufferID = 0);

private:
    ShaderProgram m_omniShadowShader = {"shaders/omniShadowMap.gs", "shaders/omniShadowMap.vs", "shaders/omniShadowMap.fs"};
    ShaderProgram m_omniShader = {nullptr, "shaders/omniLight.vs", "shaders/omniLight.fs"};
    ShaderProgram m_ambientShader = {nullptr, "shaders/ambientlight.vs", "shaders/ambientlight.fs"};
    UniformBufferObj m_globalUBO, m_ambientUBO;
    UniformBufferObj m_omniUBO, m_omniShadowUBO;
    std::uint32_t m_omniShadowFBO = 0;
    glm::vec2 m_shadowDimensions = {1024.0f, 1024.0f};

    glm::vec4 m_ambientLight;
    // Omni light and its shadow map ID
    std::vector<std::pair<OmniLight*, std::uint32_t>> m_omniLights;
    std::vector<const RenderInfo*> m_objects;
    std::vector<const RenderInfo*> m_translucentObjects;

    void renderObj(const RenderInfo* obj);

    void omniShadowPass();
    void omniLightPass(const RenderInfo* obj);

};


#endif //OPENGLHOUSE_LIGHTRENDERER_H
