/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#ifndef OPENGLHOUSE_RENDERINFO_H
#define OPENGLHOUSE_RENDERINFO_H

#include <cstdint>
#include <vector>
#include "../transform/Transformation.h"
#include "../mesh/Vertex.h"
#include "../INCLUDE_GL.h"
#include "../mesh/Mesh.h"

struct RenderInfo
{
    /// Item is/should be rendered as
        //! Transparent renders same as opaque, but disables face culling
            //! so hollow objects appear correctly
        //! Translucent does the same as transparent but enables blending
    enum Opacity
    {
        // Fully opaque
        Opaque,
        // Object that has translucency
        Translucent,
        // Has parts that are full transparent
        Transparent
    };

    std::uint32_t vao, vbo, ebo;
    Mesh* mesh;
    std::uint32_t texID, specTexID;
    Opacity opacity = Opaque;
    Transformation trans = Transformation();

    static void GenVAO(RenderInfo& info);
    //! Important to node that deleteTexture is false by default
    //! Only frees internal memory, does not call delete on pointer parameter
    static void FreeMemory(RenderInfo& info, bool deleteTexture = false);
};

#endif //OPENGLHOUSE_RENDERINFO_H
