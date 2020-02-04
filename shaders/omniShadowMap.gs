/*
*   Modified from learnopengl.com
*/
#version 330 core
layout(triangles) in;
layout(triangle_strip, max_vertices=18) out;

out vec4 FragPos;

in vec2 UV[];
out vec2 FragUV;

layout(std140) uniform GeomData
{
   mat4 ShadowMats[6];
};

void main()
{
    for(int face = 0; face < 6; ++face)
    {
        // Which face is being rendered to
        gl_Layer = face;
        for(int tvert = 0; tvert < 3; ++tvert)
        {
            FragUV = UV[tvert];
            FragPos = gl_in[tvert].gl_Position;
            gl_Position = ShadowMats[face] * FragPos;
            EmitVertex();
        }
        EndPrimitive();
    }
}