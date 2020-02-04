#version 330 core
layout(location = 0) in vec3 VertexPos;
layout(location = 2) in vec2 VertexUV;

out vec2 FragUV;

layout(std140) uniform VertData
{
    mat4 LVP;
};

void main()
{
    FragUV = VertexUV;
    gl_Position = LVP * vec4(VertexPos, 1.0f);
}