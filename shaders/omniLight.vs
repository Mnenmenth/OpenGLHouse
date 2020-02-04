#version 330 core
layout(location = 0) in vec3 VertPos;
layout(location = 1) in vec3 VertNormal;
layout(location = 2) in vec2 VertUV;

out vec3 FragPos;
out vec3 FragNormal;
out vec2 FragUV;

layout(std140) uniform GlobalVertData
{
    mat4 PV;
    mat4 Model;
    mat4 Normal;
};

void main()
{
    // Vertex pos
    FragPos = vec3(Model * vec4(VertPos, 1.0f));
    // Prevent normals from getting messed up by non-uniform scalings
    FragNormal = mat3(Normal) * VertNormal;
    FragUV = VertUV;
    gl_Position = PV * vec4(FragPos, 1.0f);
}