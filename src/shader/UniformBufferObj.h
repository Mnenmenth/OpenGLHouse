/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#ifndef OPENGLHOUSE_UBO_H
#define OPENGLHOUSE_UBO_H

#include "ShaderProgram.h"
#include "../NonCopyable.h"
#include <cstdint>
#include <cstddef>

/// Class for managing a Uniform Buffer Object in GPU memory
class UniformBufferObj : public NonCopyable
{
public:
    ///
    /// \param geomSize Size of block in geometry shader. 0 = No geometry block
    /// \param vertSize Size of block in vertex shader. 0 = No vertex block
    /// \param fragSize Size of block in fragment shader. 0 = No fragment block
    /// \param drawType One of: GL_STATIC_DRAW, GL_STREAM_DRAW, GL_DYNAMIC_DRAW
    UniformBufferObj(std::size_t geomSize, std::size_t vertSize, std::size_t fragSize, std::uint32_t drawType);
    ~UniformBufferObj();

    /// Bind and unbind current buffer
        //! These are currently here in case they will become useful at some point,
        //! but at the moment they are largely unnecessary since bufferSubData binds
        //! current buffer if it is not already bound
    void bind();
    void unbind();

    /// Links the UBO to the block in the shader program
    void addGeomUniformBlock(const ShaderProgram& shader, const char* blockName);
    void addVertUniformBlock(const ShaderProgram& shader, const char* blockName);
    void addFragUniformBlock(const ShaderProgram& shader, const char* blockName);

    /// Buffer data into the UBO
    void bufferSubData(std::uint32_t offset, std::uint32_t size, const void* data);
    //! bufferXData offset is offset from X's base offset, not 0
    void bufferGeomData(const void* data);
    void bufferGeomData(std::uint32_t offset, std::uint32_t size, const void* data);
    void bufferVertData(const void* data);
    void bufferVertData(std::uint32_t offset, std::uint32_t size, const void* data);
    void bufferFragData(const void* data);
    void bufferFragData(std::uint32_t offset, std::uint32_t size, const void* data);
private:
    /// Static counter for UBO bindings
    static std::uint32_t GetNewBinding()
    {
        static std::uint32_t Incrementer = 0;
        return Incrementer++;
    }

    /// Use to make sure the offset alignment/size of buffer ranges is multiple of GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT
    static std::size_t align(std::size_t size);

    /// Keep track of the currently bound UBO
    static std::uint32_t BoundBuffer;

    /// Buffer related data
    std::uint32_t m_ID = 0;

    std::uint32_t m_geomBinding = 0;
    std::size_t m_geomSize = 0;
    std::size_t m_geomOffset = 0;

    std::uint32_t m_vertBinding = 0;
    std::size_t m_vertSize = 0;
    std::size_t m_vertOffset = 0;

    std::uint32_t m_fragBinding = 0;
    std::size_t m_fragSize = 0;
    std::size_t m_fragOffset = 0;

    std::size_t getSize();
    void addUniformBlock(const ShaderProgram&, const char* name, std::uint32_t& binding, std::size_t offset, std::size_t size);

};

#endif //OPENGLHOUSE_UBO_H
