/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#include "Skybox.h"
#include "../mesh/Shapes.h"
#include "../resources/ResourceManager.h"
#include <stbi_image.h>
#include <cstddef>

// Cubemap texture creation modified from https://learnopengl.com/Advanced-OpenGL/Cubemaps
Skybox::Skybox(const std::array<const char*, 6>& faceTextures) : m_ubo(0, sizeof(glm::mat4), 0, GL_DYNAMIC_DRAW)
{

    m_ubo.addVertUniformBlock(m_shader, "VertData");

    glGenTextures(1, &m_cubemapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapTexture);

    int width, height, channels;
    GLuint format;
    for(std::size_t face = 0; face < faceTextures.size(); face++)
    {
        unsigned char* data = stbi_load(faceTextures[face], &width, &height, &channels, 0);
        if(data)
        {
            format = (channels == 3) ? GL_RGB : GL_RGBA;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Failed to load skybox texture: " << face << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    std::vector<Vertex> vertices;
    std::vector<std::uint32_t> indices;
    Shapes::GenCube(vertices, indices);
    m_skybox = ResourceManager::Instance().NewRenderInfo("skybox");
    m_skybox.mesh = &ResourceManager::Instance().NewMesh("skybox", vertices, indices);
    RenderInfo::GenVAO(m_skybox);
}

Skybox::~Skybox()
{
    RenderInfo::FreeMemory(m_skybox);
    glDeleteTextures(1, &m_cubemapTexture);
}

void Skybox::render(const glm::mat4& ProjMat, const glm::mat4& ViewMat)
{
    glDepthFunc(GL_LEQUAL);
    glCullFace(GL_FRONT);

    m_shader.bind();

    m_ubo.bufferVertData(&(ProjMat * glm::mat4(glm::mat3(ViewMat)))[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapTexture);

    glBindVertexArray(m_skybox.vao);
    glDrawElements(GL_TRIANGLES, m_skybox.mesh->indices.size(), GL_UNSIGNED_INT, nullptr);

    glDepthFunc(GL_LESS);
    glCullFace(GL_BACK);
}