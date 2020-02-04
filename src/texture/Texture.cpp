/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#include "Texture.h"
#include "../INCLUDE_GL.h"
#include <stbi_image.h>

std::uint32_t Texture::NewTex2D(const char *filePath, std::uint32_t wrapType, std::uint32_t filterType, bool genMipMaps)
{
    std::uint32_t id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapType);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapType);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterType);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterType);

    int width, height, channels;
    unsigned char* data = stbi_load(filePath, &width, &height, &channels, 0);
    std::uint32_t format = channels == 3 ? GL_RGB : GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    if(genMipMaps) glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    return id;
}

void Texture::FreeMemory(std::uint32_t& textureID)
{
    glDeleteTextures(1, &textureID);
}

std::array<std::uint32_t, 6> Texture::BasicColorIDs;
bool Texture::HasInit;

void Texture::InitBasicColors()
{
    if(HasInit)
        return;

    HasInit = true;

    static unsigned char colors[6][4] {
            {0, 0, 0, 255},
            {255, 255, 255, 255},
            {static_cast<unsigned char>(0.4f * 255), static_cast<unsigned char>(0.4f * 255), static_cast<unsigned char>(0.4f * 255), static_cast<unsigned char>(1.0f * 255)},
            {255, 0, 0, 255},
            {0, 255, 0, 255},
            {0, 0, 255, 255}
    };

    for(int color = 0; color < 6; color++){
        glGenTextures(1, &BasicColorIDs[color]);
        glBindTexture(GL_TEXTURE_2D, BasicColorIDs[color]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, colors[color]);
    }
}

void Texture::FreeBasicColors()
{
    HasInit = false;

    glDeleteTextures(BasicColorIDs.size(), BasicColorIDs.data());
}

std::uint32_t Texture::GetBasicColor(Texture::BasicColors color)
{
    if(!HasInit) InitBasicColors();

    return BasicColorIDs[color];
}