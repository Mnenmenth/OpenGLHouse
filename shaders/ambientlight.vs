#version 330 core
layout(location = 0) in vec3 VertexPos;
layout(location = 2) in vec2 VertexUV;

out vec2 FragUV;

layout(std140) uniform GlobalVertData
{
    mat4 PV;
    mat4 Model;
};

void main()
{
    FragUV = VertexUV;
    gl_Position = PV * Model * vec4(VertexPos, 1.0f);
}