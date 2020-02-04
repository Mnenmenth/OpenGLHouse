/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#ifndef OPENGLHOUSE_BASICRENDERER_H
#define OPENGLHOUSE_BASICRENDERER_H


#include <vector>
#include "RenderInfo.h"
#include "../shader/ShaderProgram.h"
#include "../NonCopyable.h"
#include "../shader/UniformBufferObj.h"

class BasicRenderer : public NonCopyable
{
public:
    BasicRenderer();

    void addObject(const RenderInfo*);
    void removeObject(const RenderInfo*);

    void render(const glm::mat4& ProjViewMat);

private:
    ShaderProgram m_shader = {nullptr, "shaders/basic.vs", "shaders/basic.fs"};
    UniformBufferObj m_ubo;
    std::vector<const RenderInfo*> m_objects;
};


#endif //OPENGLHOUSE_BASICRENDERER_H
