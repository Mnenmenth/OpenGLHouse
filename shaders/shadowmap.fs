#version 330 core

in vec2 FragUV;
uniform sampler2D diffusemap;

void main()
{
    // Discard the fragment if the alpha value is lower than 0.1f
    if(texture(diffusemap, FragUV).a < 0.1f)
        discard;

    gl_FragDepth = gl_FragCoord.z;
}