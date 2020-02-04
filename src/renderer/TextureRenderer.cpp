/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#include "TextureRenderer.h"
#include <algorithm>

#include "../shader/UniformBufferObj.h"

TextureRenderer::TextureRenderer() : m_ubo(0, sizeof(glm::mat4), 0, GL_DYNAMIC_DRAW)
{
    m_ubo.addVertUniformBlock(m_shader, "VertData");
}

void TextureRenderer::addObject(const RenderInfo* obj)
{
    m_objects.push_back(obj);
}

void TextureRenderer::removeObject(const RenderInfo* obj)
{
    auto iterator = std::find(m_objects.begin(), m_objects.end(), obj);
    if(iterator != m_objects.end()) m_objects.erase(iterator);
}

void TextureRenderer::render(const glm::mat4 &ProjViewMat)
{
    m_shader.bind();
    glActiveTexture(GL_TEXTURE0);
    for(const RenderInfo* obj : m_objects)
    {
        glBindTexture(GL_TEXTURE_2D, obj->texID);

        m_ubo.bufferVertData(&(ProjViewMat * obj->trans.getMatrix())[0][0]);

        glBindVertexArray(obj->vao);
        glDrawElements(GL_TRIANGLES, obj->mesh->indices.size(), GL_UNSIGNED_INT, nullptr);
    }
}