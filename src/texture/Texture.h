/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */
#ifndef OPENGLHOUSE_TEXTURE_H
#define OPENGLHOUSE_TEXTURE_H


#include <cstdint>
#include <array>

class Texture
{
public:

    static std::uint32_t NewTex2D(const char* filePath, std::uint32_t wrapType, std::uint32_t filterType, bool genMipMaps);
    static void FreeMemory(std::uint32_t& textureID);

    enum BasicColors
    {
        BLACK,
        WHITE,
        GREY,
        RED,
        GREEN,
        BLUE
    };

    static void InitBasicColors();
    static void FreeBasicColors();
    static std::uint32_t GetBasicColor(BasicColors color);

private:
    static bool HasInit;
    static std::array<std::uint32_t, 6> BasicColorIDs;
};


#endif //OPENGLHOUSE_TEXTURE_H
