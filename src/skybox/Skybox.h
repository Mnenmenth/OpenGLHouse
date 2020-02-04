/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#ifndef OPENGLHOUSE_SKYBOX_H
#define OPENGLHOUSE_SKYBOX_H

#include <array>
#include "../shader/ShaderProgram.h"
#include "../renderer/RenderInfo.h"
#include "../shader/UniformBufferObj.h"
#include "../NonCopyable.h"

class Skybox : public NonCopyable
{
public:
    explicit Skybox(const std::array<const char*, 6>& faceTextures);
    ~Skybox();
    void render(const glm::mat4& ProjMat, const glm::mat4& ViewMat);
private:
    ShaderProgram m_shader = {nullptr, "shaders/skybox.vs", "shaders/skybox.fs"};
    std::uint32_t m_cubemapTexture;
    UniformBufferObj m_ubo;
    RenderInfo m_skybox = {};
};


#endif //OPENGLHOUSE_SKYBOX_H
