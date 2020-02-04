/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#include "LightRenderer.h"
#include <algorithm>
#include <cstddef>
#include <glm/gtc/type_ptr.hpp>

LightRenderer::LightRenderer() :
        m_ambientLight(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f)),
        m_globalUBO(0, sizeof(glm::mat4)*3, sizeof(glm::vec4), GL_DYNAMIC_DRAW),
        m_ambientUBO(0, 0, sizeof(glm::vec4), GL_STATIC_DRAW),
        m_omniUBO(0, 0, OmniLight::datasize, GL_DYNAMIC_DRAW),
        m_omniShadowUBO(OmniLight::shadowPVsSize, sizeof(glm::mat4)*3, sizeof(glm::vec4), GL_DYNAMIC_DRAW)
{
/// Global UBO Data
    // Ambient light
    m_globalUBO.addVertUniformBlock(m_ambientShader, "GlobalVertData");

    // Omni lights
    m_globalUBO.addVertUniformBlock(m_omniShader, "GlobalVertData");
    m_globalUBO.addFragUniformBlock(m_omniShader, "GlobalFragData");

/// Ambient shader UBO data

    m_ambientUBO.addFragUniformBlock(m_ambientShader, "FragData");
    m_ambientUBO.bufferFragData(&m_ambientLight[0]);

/// Omni shadow shader UBO data
    m_omniShadowUBO.addGeomUniformBlock(m_omniShadowShader, "GeomData");
    m_omniShadowUBO.addVertUniformBlock(m_omniShadowShader, "VertData");
    m_omniShadowUBO.addFragUniformBlock(m_omniShadowShader, "FragData");

/// Omni shadow shader FBO and shadow map

    glGenFramebuffers(1, &m_omniShadowFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_omniShadowFBO);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

/// Omni shader
    m_omniUBO.addFragUniformBlock(m_omniShader, "FragData");

/// Set active texture ids

    m_ambientShader.bind();
    m_ambientShader.setInt("diffusemap", 0);

    m_omniShadowShader.bind();
    m_omniShadowShader.setInt("diffusemap", 0);

    m_omniShader.bind();
    m_omniShader.setInt("diffusemap", 0);
    m_omniShader.setInt("specularmap", 1);
    m_omniShader.setInt("shadowmap", 2);

}

LightRenderer::~LightRenderer()
{

}

void LightRenderer::setAmbientLight(const glm::vec4& ambientLight)
{
    m_ambientLight = ambientLight;
    m_ambientUBO.bufferFragData(&m_ambientLight[0]);
}

void LightRenderer::addLight(OmniLight* light)
{
    std::uint32_t shadowMap;
    glGenTextures(1, &shadowMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap);

    for(std::uint32_t face = 0; face < 6; ++face)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_DEPTH_COMPONENT, m_shadowDimensions.x, m_shadowDimensions.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    light->setShadowDimensions(m_shadowDimensions);
    m_omniLights.emplace_back(light, shadowMap);
}

void LightRenderer::removeLight(const OmniLight* light)
{
    auto it = std::remove_if(
            m_omniLights.begin(),
            m_omniLights.end(),
            [light](std::pair<OmniLight*, std::uint32_t> pair)
            {
                if(pair.first == light)
                {
                    glDeleteTextures(1, &pair.second);
                    return true;
                }
                return false;
            }
    );

    m_omniLights.erase(it, m_omniLights.end());
}

void LightRenderer::addObject(const RenderInfo* obj)
{
    if(obj->opacity != RenderInfo::Translucent)
    {
        m_objects.push_back(obj);
    }
    else
    {
        m_translucentObjects.push_back(obj);
    }
}

void LightRenderer::removeObject(const RenderInfo* obj)
{
    if(obj->opacity != RenderInfo::Translucent)
    {
        auto iterator = std::find(m_objects.begin(), m_objects.end(), obj);
        if(iterator != m_objects.end()) m_objects.erase(iterator);
    }
    else
    {
        auto iterator = std::find(m_translucentObjects.begin(), m_translucentObjects.end(), obj);
        if(iterator != m_translucentObjects.end()) m_translucentObjects.erase(iterator);
    }
}

void LightRenderer::render(const glm::mat4& ProjViewMat, const glm::vec3& EyePos, const glm::vec4& Viewport, std::uint32_t framebufferID)
{
///
/// Shadow generation
///
    // Prepare viewport for shadows
    glViewport(0, 0, m_shadowDimensions.x, m_shadowDimensions.y);
    // Enable blending for multiple light sources
    glEnable(GL_BLEND);
    // Blending should be purely additive
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);
    // Don't discard fragments based on depth buffer of previous passes
    glDepthFunc(GL_LEQUAL);

    /// Omni pass
    omniShadowPass();

    // Reset framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
    glDepthFunc(GL_LESS);
    glDisable(GL_BLEND);

///
/// Light blending
///
    // Reset viewport
    glViewport(Viewport.x, Viewport.y, Viewport.z, Viewport.w);

    // Update global projection view mat and eye pos
    m_globalUBO.bufferVertData(0, sizeof(glm::mat4), &ProjViewMat[0][0]);
    m_globalUBO.bufferFragData(&glm::vec4(EyePos, 1.0f)[0]);

    /// Non translucent objects
    for(const RenderInfo* obj : m_objects)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, obj->texID);
        glActiveTexture(GL_TEXTURE1);
        // Use regular texture as specular map if there is none provided
        glBindTexture(GL_TEXTURE_2D, obj->specTexID == 0 ? obj->texID : obj->specTexID);

        // Update global model matrices
        glm::mat4 ModelMat = obj->trans.getMatrix();
        m_globalUBO.bufferVertData(sizeof(glm::mat4), sizeof(glm::mat4), &ModelMat[0][0]);
        m_globalUBO.bufferVertData(sizeof(glm::mat4)*2, sizeof(glm::mat4), &glm::transpose(glm::inverse(ModelMat))[0][0]);

        glBindVertexArray(obj->vao);

        /// Ambient lighting pass
        m_ambientShader.bind();
        renderObj(obj);

        /// Prepare opengl for additive lighting over multiple passes
        // Enable blending for multiple light sources
        glEnable(GL_BLEND);
        // Blending should be purely additive
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);
        // Don't discard fragments based on depth buffer of previous passes
        glDepthFunc(GL_LEQUAL);
        // Blend in all point light sources

        /// Omni light pass
        omniLightPass(obj);

        /// Reset depth func and blending for next object
        glDepthFunc(GL_LESS);
        glDisable(GL_BLEND);
    }

    /// Translucent objects
    // Enable blending for multiple light sources
    glEnable(GL_BLEND);
    // Don't discard fragments based on depth buffer of previous passes
    glDepthFunc(GL_LEQUAL);
    glBlendEquation(GL_FUNC_ADD);
    for(const RenderInfo* obj : m_translucentObjects)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, obj->texID);
        glActiveTexture(GL_TEXTURE1);
        // Use regular texture as specular map if there is none provided
        glBindTexture(GL_TEXTURE_2D, obj->specTexID == 0 ? obj->texID : obj->specTexID);

        // Update global model matrices
        glm::mat4 ModelMat = obj->trans.getMatrix();
        m_globalUBO.bufferVertData(sizeof(glm::mat4), sizeof(glm::mat4), &ModelMat[0][0]);
        m_globalUBO.bufferVertData(sizeof(glm::mat4)*2, sizeof(glm::mat4), &glm::transpose(glm::inverse(ModelMat))[0][0]);

        glBindVertexArray(obj->vao);

        /// Ambient lighting pass
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        m_ambientShader.bind();
        renderObj(obj);


        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        /// Omni light pass
        omniLightPass(obj);
    }
    /// Reset depth func and blending
    glDepthFunc(GL_LESS);
    glDisable(GL_BLEND);
}

void LightRenderer::renderObj(const RenderInfo *obj)
{
    if(obj->opacity != RenderInfo::Opacity::Opaque)
    {
        glDisable(GL_CULL_FACE);
        glDrawElements(GL_TRIANGLES, obj->mesh->indices.size(), GL_UNSIGNED_INT, nullptr);
        glEnable(GL_CULL_FACE);
    }
    else
    {
        glDrawElements(GL_TRIANGLES, obj->mesh->indices.size(), GL_UNSIGNED_INT, nullptr);
    }
}

void LightRenderer::omniShadowPass()
{
    m_omniShadowShader.bind();
    glBindFramebuffer(GL_FRAMEBUFFER, m_omniShadowFBO);

    glActiveTexture(GL_TEXTURE0);
    for(std::pair<OmniLight*, std::uint32_t>& pair : m_omniLights)
    {
        const OmniLight* light = pair.first;
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, pair.second, 0);
        glClear(GL_DEPTH_BUFFER_BIT);

        // Buffer shadow PVs
        m_omniShadowUBO.bufferGeomData(&light->getShadowPVs());

        // Buffer eye pos and far clipping plane
        m_omniShadowUBO.bufferFragData(&light->getData().pos[0]);

        for(const RenderInfo* obj : m_objects)
        {
            // Bind object's texture
            glBindTexture(GL_TEXTURE_2D, obj->texID);

            // Buffer object's model matrix
            m_omniShadowUBO.bufferVertData(&obj->trans.getMatrix()[0][0]);

            // Bind object's VAO and draw it
            glBindVertexArray(obj->vao);

            renderObj(obj);
        }

        //! Translucent objects shouldn't cast shadows for now until they're properly colored
//        for(const RenderInfo* obj : m_translucentObjects)
//        {
//            // Bind object's texture
//            glBindTexture(GL_TEXTURE_2D, obj->texID);
//
//            // Buffer object's model matrix
//            m_omniShadowUBO.bufferVertData(&obj->trans.getMatrix()[0][0]);
//
//            // Bind object's VAO and draw it
//            glBindVertexArray(obj->vao);
//
//            renderObj(obj);
//        }

    }
}

void LightRenderer::omniLightPass(const RenderInfo* obj)
{
    m_omniShader.bind();
    glActiveTexture(GL_TEXTURE2);
    for(std::pair<OmniLight*, std::uint32_t>& pair : m_omniLights)
    {
        const OmniLight* light = pair.first;

        // Buffer current light data
        m_omniUBO.bufferFragData(&light->getData());

        glBindTexture(GL_TEXTURE_CUBE_MAP, pair.second);

        renderObj(obj);
    }
}