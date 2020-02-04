/*
*   Modified from learnopengl.com
*/
#version 330 core
layout(location = 0) in vec3 VertPos;
layout(location = 2) in vec2 VertUV;

out vec2 UV;

layout(std140) uniform VertData
{
    mat4 ModelMat;
};

void main()
{
    UV = VertUV;
    gl_Position = ModelMat * vec4(VertPos, 1.0f);
}