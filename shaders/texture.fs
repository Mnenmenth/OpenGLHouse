#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
uniform sampler2D tex;

void main()
{
    // Discard the fragment if the alpha value is lower than 0.1f
    if(texture(tex, TexCoord).a < 0.1f)
        discard;

    FragColor = texture(tex, TexCoord);
}