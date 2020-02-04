/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  *
  * Modified from https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader_s.h
  */

#include "ShaderProgram.h"
#include "../INCLUDE_GL.h"

#define glUniform(type, value) glUniform ## type(glGetUniformLocation(m_ID, name), value)
#define glUniformVec(type, value) glUniform ## type(glGetUniformLocation(m_ID, name), 1, value)
#define glUniformMatrix(type, value) glUniformMatrix ## type(glGetUniformLocation(m_ID, name), 1, GL_FALSE, value)

const char* ShaderProgram::ShaderTypeNames[] = {"Program", "Vertex", "Fragment", "Geometry"};

std::uint32_t ShaderProgram::BoundProgram = 0;

ShaderProgram::ShaderProgram(const char* geomPath, const char *vertexPath, const char *fragmentPath) {

    std::cout << "Creating shader program..." << std::endl;

    std::string geomSource;
    std::string vertexSource;
    std::string fragmentSource;

    std::ifstream geomFile;
    std::ifstream vertexFile;
    std::ifstream fragmentFile;

    geomFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        // Read contents of geom shader file
        if(geomPath != nullptr)
        {
            std::cout << "\tLoading geometry shader - " << geomPath << std::endl;
            geomFile.open(geomPath);
            std::stringstream geomStream;
            geomStream << geomFile.rdbuf();
            geomFile.close();
            geomSource = geomStream.str();
        }

        std::cout << "\tLoading vertex shader - " << vertexPath << std::endl;
        // Read contents of vertex shader file
        vertexFile.open(vertexPath);
        std::stringstream vertexStream;
        vertexStream << vertexFile.rdbuf();
        vertexFile.close();
        vertexSource = vertexStream.str();

        std::cout << "\tLoading fragment shader - " << fragmentPath << std::endl;
        // Read contents of fragment shader file
        fragmentFile.open(fragmentPath);
        std::stringstream fragmentSteam;
        fragmentSteam << fragmentFile.rdbuf();
        fragmentFile.close();
        fragmentSource = fragmentSteam.str();

    } catch (std::ifstream::failure &e) {
        std::cout << "\tUnable to read shader file(s): " << e.what() << std::endl;
    }

    std::uint32_t geom;
    if(geomPath != nullptr)
    {
        const char* gSource = geomSource.c_str();
        geom = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geom, 1, &gSource, nullptr);
        glCompileShader(geom);
        CompileStatus(geom, GEOMETRY);
    }

    std::uint32_t vertex;
    const char* vSource = vertexSource.c_str();
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vSource, nullptr);
    glCompileShader(vertex);
    CompileStatus(vertex, VERTEX);

    std::uint32_t fragment;
    const char* fSource = fragmentSource.c_str();
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fSource, nullptr);
    glCompileShader(fragment);
    CompileStatus(fragment, FRAGMENT);

    m_ID = glCreateProgram();
    glAttachShader(m_ID, vertex);
    glAttachShader(m_ID, fragment);
    if(geomPath != nullptr) glAttachShader(m_ID, geom);
    glLinkProgram(m_ID);
    CompileStatus(m_ID, PROGRAM);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if(geomPath != nullptr) glDeleteShader(geom);
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(m_ID);
}

std::uint32_t ShaderProgram::getProgramID() const
{
    return m_ID;
}

void ShaderProgram::bind() const
{
    if(BoundProgram != m_ID)
    {
        glUseProgram(m_ID);
        BoundProgram = m_ID;
    }
}

void ShaderProgram::setInt(const char *name, int value) const
{
    glUniform(1i, value);
}

void ShaderProgram::setUInt(const char *name, std::uint32_t value) const
{
    glUniform(1ui, value);
}

void ShaderProgram::setFloat(const char *name, float value) const
{
    glUniform(1f, value);
}

void ShaderProgram::setVec3f(const char *name, const glm::vec3 &value) const
{
    glUniformVec(3fv, &value[0]);
}

void ShaderProgram::setVec4f(const char *name, const glm::vec4 &value) const
{
    glUniformVec(4fv, &value[0]);
}

void ShaderProgram::setMatrix4f(const char *name, const glm::mat4& value) const
{
    glUniformMatrix(4fv, &value[0][0]);
}


void ShaderProgram::CompileStatus(std::uint32_t ID, ShaderType type) {
    int success;
    //int logLength;
    //glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &logLength);
    char infoLog[1024];
    std::cout << "\t";
    if(type == PROGRAM) { // Program Link Status
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if(success) {
            std::cout << ShaderTypeNames[type] << " linked successfully" << std::endl;
        } else {
            glGetProgramInfoLog(ID, 1024, nullptr, infoLog);
            std::cout << ShaderTypeNames[type] << " linking error: \n\t" << infoLog << std::endl;
        }
    } else { // Shader Compile Status
        glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
        if(success) {
            std::cout << ShaderTypeNames[type] << " shader compiled successfully" << std::endl;
        } else {
            glGetShaderInfoLog(ID, 1024, nullptr, infoLog);
            std::cout << ShaderTypeNames[type] << " shader compilation failed: \n\t" << infoLog << std::endl;
        }
    }
}