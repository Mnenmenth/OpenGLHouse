/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#ifndef OPENGLHOUSE_TEXTURERENDERER_H
#define OPENGLHOUSE_TEXTURERENDERER_H

#include <vector>
#include "RenderInfo.h"
#include "../shader/ShaderProgram.h"
#include "../NonCopyable.h"
#include "../shader/UniformBufferObj.h"

class TextureRenderer : public NonCopyable
{
public:
    TextureRenderer();

    void addObject(const RenderInfo*);
    void removeObject(const RenderInfo*);

    void render(const glm::mat4& ProjViewMat);

private:
    ShaderProgram m_shader = {nullptr, "shaders/texture.vs", "shaders/texture.fs"};
    UniformBufferObj m_ubo;
    std::vector<const RenderInfo*> m_objects;
};


#endif //OPENGLHOUSE_TEXTURERENDERER_H
