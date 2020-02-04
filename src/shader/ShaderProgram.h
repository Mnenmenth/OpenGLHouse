/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  *
  */

#ifndef OPENGLHOUSE_SHADERPROGRAM_H
#define OPENGLHOUSE_SHADERPROGRAM_H


#include "../NonCopyable.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/mat4x4.hpp>
#include <cstddef>

class ShaderProgram : public NonCopyable
{
public:
    //! Pass nullptr for any unused shaders
    ShaderProgram(const char* geomPath, const char* vertexPath, const char* fragmentPath);
    ~ShaderProgram();
    std::uint32_t getProgramID() const;
    void bind() const;
    void setInt(const char* name, int value) const;
    void setUInt(const char* name, std::uint32_t value) const;
    void setFloat(const char* name, float value) const;
    void setVec3f(const char* name, const glm::vec3& value) const;
    void setVec4f(const char* name, const glm::vec4& value) const;
    void setMatrix4f(const char* name, const glm::mat4& matrix) const;

private:
    std::uint32_t m_ID;
    enum ShaderType {
        PROGRAM,
        VERTEX,
        FRAGMENT,
        GEOMETRY
    };
    static const char* ShaderTypeNames[];
    static void CompileStatus(std::uint32_t ID, ShaderType type);
    /// Keep track of the currently bound shader program
    static std::uint32_t BoundProgram;
};


#endif //OPENGLHOUSE_SHADERPROGRAM_H
