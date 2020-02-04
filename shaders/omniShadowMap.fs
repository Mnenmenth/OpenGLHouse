/*
*   Modified from learnopengl.com
*/
#version 330 core

in vec4 FragPos;
in vec2 FragUV;
uniform sampler2D diffusemap;

layout(std140) uniform FragData
{
    vec4 LightData; // xyz -> pos ; w -> far plane
};

void main()
{
    // Discard the fragment if the alpha value is lower than 0.1f
    if(texture(diffusemap, FragUV).a < 0.1f)
        discard;

    float lightDist = length(FragPos.xyz - LightData.xyz);

    // Map distance to [0;1]
    lightDist = lightDist / LightData.w;

    gl_FragDepth = lightDist;
}