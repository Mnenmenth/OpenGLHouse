#version 330 core

out vec4 FragColor;

in vec2 FragUV;
uniform sampler2D diffusemap;

layout(std140) uniform FragData
{
    vec4 ambient;
};

void main()
{
    vec4 texcolor = texture(diffusemap, FragUV);
    // Discard the fragment if the alpha value is lower than 0.1f
    if(texcolor.a < 0.1f)
        discard;
    FragColor = ambient * texcolor;
}