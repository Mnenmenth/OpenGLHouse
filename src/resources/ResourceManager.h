/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#ifndef OPENGLHOUSE_RESOURCEMANAGER_H
#define OPENGLHOUSE_RESOURCEMANAGER_H

#include "../NonCopyable.h"
#include <unordered_map>
#include "../shader/ShaderProgram.h"
#include "../mesh/Mesh.h"
#include "../renderer/RenderInfo.h"

//! Loading a new resource with a name param that already exists for that resource type will overwrite that entry
class ResourceManager : NonCopyable
{
public:
    static ResourceManager& Instance()
    {
        static ResourceManager instance;
        return instance;
    }

    ShaderProgram& NewShader(const char* name, const char* geomPath, const char* vertPath, const char* fragPath);
    ShaderProgram& GetShader(const char* name);

    std::uint32_t NewTex2D(const char* name, const char* filePath, std::uint32_t wrapType, std::uint32_t filterType, bool genMipmaps);
    std::uint32_t GetTex2D(const char* name);

    Mesh& NewMesh(const char* name, const char* filePath);
    Mesh& NewMesh(const char* name, std::vector<Vertex>& vertices, std::vector<std::uint32_t>& indices);
    Mesh& GetMesh(const char* name);

    RenderInfo& NewRenderInfo(const char* name);
    RenderInfo& GetRenderInfo(const char* name);
private:
    ResourceManager() = default;
    std::unordered_map<std::string, ShaderProgram> m_shaders;
    std::unordered_map<std::string, std::uint32_t> m_textures;
    std::unordered_map<std::string, Mesh> m_meshes;
    std::unordered_map<std::string, RenderInfo> m_renderInfos;
};


#endif //OPENGLHOUSE_RESOURCEMANAGER_H
