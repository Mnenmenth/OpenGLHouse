#version 330 core
layout(location = 0) in vec3 VertPos;

out vec3 TexCoords;

layout(std140) uniform VertData
{
    mat4 PV;
};

void main() {
    TexCoords = VertPos;
    gl_Position = (PV * vec4(VertPos, 1.0f)).xyww;
}
