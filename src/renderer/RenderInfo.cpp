/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#include "RenderInfo.h"
#include "../texture/Texture.h"

void RenderInfo::GenVAO(RenderInfo& info)
{
    glGenVertexArrays(1, &info.vao);
    glGenBuffers(1, &info.vbo);
    glGenBuffers(1, &info.ebo);

    glBindVertexArray(info.vao);
    glBindBuffer(GL_ARRAY_BUFFER, info.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(info.mesh->vertices[0]) * info.mesh->vertices.size(), info.mesh->vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, info.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(info.mesh->indices[0]) * info.mesh->indices.size(), info.mesh->indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, v)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, n)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, t)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void RenderInfo::FreeMemory(RenderInfo& info, bool deleteTexture)
{
    glDeleteVertexArrays(1, &info.vao);
    glDeleteBuffers(1, &info.vbo);
    glDeleteBuffers(1, &info.ebo);
    if(deleteTexture)
    {
        Texture::FreeMemory(info.texID);
        Texture::FreeMemory(info.specTexID);
    }

    info.vao = info.vbo = info.ebo = info.texID = 0;
}