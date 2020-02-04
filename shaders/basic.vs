#version 330

layout(location = 0) in vec3 Vertex;

layout(std140) uniform VertData
{
    mat4 MVP;
};
void main() {
    gl_Position = MVP * vec4(Vertex, 1.0f);
}
