/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#include <iostream>
#include "UniformBufferObj.h"
#include "../INCLUDE_GL.h"

// Used to align memory offset of uniform blocks
std::size_t UniformBufferObj::align(std::size_t size)
{
    static bool hasInit = false;
    static int offsetAlignment;
    if(!hasInit)
    {
        hasInit = true;
        glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &offsetAlignment);
    }
    return (size + offsetAlignment - 1) & -offsetAlignment;
}

std::uint32_t UniformBufferObj::BoundBuffer = 0;

UniformBufferObj::UniformBufferObj(std::size_t geomSize, std::size_t vertSize, std::size_t fragSize, std::uint32_t drawType)
{

    m_geomSize = align(geomSize);
    m_geomOffset = 0;

    m_vertSize = align(vertSize);
    m_vertOffset = m_geomSize;

    m_fragSize = fragSize;
    m_fragOffset = m_geomSize + m_vertSize;

    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
    glBufferData(GL_UNIFORM_BUFFER, getSize(), nullptr, drawType);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

UniformBufferObj::~UniformBufferObj()
{
    glDeleteBuffers(1, &m_ID);
}

std::size_t UniformBufferObj::getSize() { return m_geomSize + m_vertSize + m_fragSize; }

void UniformBufferObj::addUniformBlock(const ShaderProgram& shader, const char* name, std::uint32_t& binding, std::size_t offset, std::size_t size)
{
    if(binding == 0)
    {
        binding = UniformBufferObj::GetNewBinding();
    }
    // Get the index of the uniform block
    std::uint32_t index = glGetUniformBlockIndex(shader.getProgramID(), name);
    // Bind the uniform block to its respective binding point within the shader
    glUniformBlockBinding(shader.getProgramID(), index, binding);
    // Bind the uniform block to a data range within the buffer
    glBindBufferRange(GL_UNIFORM_BUFFER, binding, m_ID, offset, size);
}

void UniformBufferObj::bind()
{
    // Avoid unnecessary bind calls
    if(m_ID != BoundBuffer)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
        BoundBuffer = m_ID;
    }
}
void UniformBufferObj::unbind()
{
    // Don't mess with another UBO's state if this one isn't bound
    if(m_ID == BoundBuffer)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        BoundBuffer = 0;
    }
}

void UniformBufferObj::addGeomUniformBlock(const ShaderProgram &shader, const char *blockName)
{
    addUniformBlock(shader, blockName, m_geomBinding, m_geomOffset, m_geomSize);
}

void UniformBufferObj::addVertUniformBlock(const ShaderProgram &shader, const char *blockName)
{
    addUniformBlock(shader, blockName, m_vertBinding, m_vertOffset, m_vertSize);
}

void UniformBufferObj::addFragUniformBlock(const ShaderProgram &shader, const char *blockName)
{
    addUniformBlock(shader, blockName, m_fragBinding, m_fragOffset, m_fragSize);
}

void UniformBufferObj::bufferSubData(std::uint32_t offset, std::uint32_t size, const void *data)
{
    bind();
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}

void UniformBufferObj::bufferGeomData(const void *data)
{
    bufferSubData(m_geomOffset, m_geomSize, data);
}

void UniformBufferObj::bufferGeomData(std::uint32_t offset, std::uint32_t size, const void *data)
{
    bufferSubData(m_geomOffset+offset, size, data);
}

void UniformBufferObj::bufferVertData(const void *data)
{
    bufferSubData(m_vertOffset, m_vertSize, data);
}

void UniformBufferObj::bufferVertData(std::uint32_t offset, std::uint32_t size, const void *data)
{
    bufferSubData(m_vertOffset+offset, size, data);
}

void UniformBufferObj::bufferFragData(const void *data)
{
    bufferSubData(m_fragOffset, m_fragSize, data);
}

void UniformBufferObj::bufferFragData(std::uint32_t offset, std::uint32_t size, const void *data)
{
    bufferSubData(m_fragOffset+offset, size, data);
}