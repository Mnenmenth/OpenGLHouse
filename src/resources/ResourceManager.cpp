/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#include "ResourceManager.h"
#include "../texture/Texture.h"

ShaderProgram& ResourceManager::NewShader(const char *name, const char *geomPath, const char *vertPath, const char *fragPath)
{
    auto result = m_shaders.emplace(std::piecewise_construct,
                                    std::forward_as_tuple(name),
                                    std::forward_as_tuple(geomPath, vertPath, fragPath)
    );
    if(!result.second)
    {
        std::cout << "Shader key '" << name << "' already exists. Existing value will be returned" << std::endl;
    }
    return result.first->second;
}

ShaderProgram& ResourceManager::GetShader(const char *name)
{
    try
    {
        return m_shaders.at(name);
    }
    catch (std::out_of_range &e)
    {
        std::cout << "Shader '" << name << "' doesn't exist!" << std::endl;
        return m_shaders.at("MISSING");
    }
}

std::uint32_t ResourceManager::NewTex2D(const char *name, const char *filePath, std::uint32_t wrapType,
                                        std::uint32_t filterType, bool genMipmaps)
{
    auto result = m_textures.insert({name, Texture::NewTex2D(filePath, wrapType, filterType, genMipmaps)});
    if(!result.second)
    {
        std::cout << "Tex2D key '" << name << "' already exists. Existing value will be returned" << std::endl;
    }
    return result.first->second;
}

std::uint32_t ResourceManager::GetTex2D(const char *name)
{
    try
    {
        return m_textures.at(name);
    }
    catch (std::out_of_range &e)
    {
        std::cout << "Texture '" << name << "' doesn't exist!" << std::endl;
        return m_textures.at("MISSING");
    }
}

Mesh& ResourceManager::NewMesh(const char *name, const char *filePath)
{
    auto result = m_meshes.emplace
            (
                    std::piecewise_construct,
                    std::forward_as_tuple(name),
                    std::forward_as_tuple()
            );
    Mesh &mesh = result.first->second;

    if(!result.second)
    {
        std::cout << "Mesh key '" << name << "' already exists. Existing value will be returned" << std::endl;
        return mesh;
    }

    Mesh::LoadMesh(mesh, filePath);
    return mesh;
}

Mesh& ResourceManager::NewMesh(const char *name, std::vector<Vertex>& vertices, std::vector<std::uint32_t>& indices)
{
    auto result = m_meshes.emplace
            (
                    std::piecewise_construct,
                    std::forward_as_tuple(name),
                    std::forward_as_tuple()
            );
    Mesh &mesh = result.first->second;

    if(!result.second)
    {
        std::cout << "Mesh key '" << name << "' already exists. Existing value will be returned" << std::endl;
        return mesh;
    }

    mesh.vertices = vertices;
    mesh.indices = indices;
    return mesh;
}

Mesh& ResourceManager::GetMesh(const char *name)
{
    try
    {
        return m_meshes.at(name);
    }
    catch (std::out_of_range &e)
    {
        std::cout << "Mesh '" << name << "' doesn't exist!" << std::endl;
        return m_meshes.at("MISSING");
    }}

RenderInfo &ResourceManager::NewRenderInfo(const char *name)
{
    auto result = m_renderInfos.emplace
            (
                    std::piecewise_construct,
                    std::forward_as_tuple(name),
                    std::forward_as_tuple()
            );

    if(!result.second)
    {
        std::cout << "RenderInfo key '" << name << "' already exists. Existing value will be returned" << std::endl;
    }

    return result.first->second;
}

RenderInfo &ResourceManager::GetRenderInfo(const char *name)
{
    try
    {
        return m_renderInfos.at(name);
    }
    catch (std::out_of_range &e)
    {
        std::cout << "RenderInfo '" << name << "' doesn't exist!" << std::endl;
        return m_renderInfos.at("MISSING");
    }
}