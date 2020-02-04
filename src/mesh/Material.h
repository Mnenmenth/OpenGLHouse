/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#ifndef OPENGLHOUSE_MATERIAL_H
#define OPENGLHOUSE_MATERIAL_H

#include <cstdint>
#include <cstddef>

struct Material
{
    std::uint32_t texID, specTexID;
    std::uint32_t minIndex, maxIndex;
    std::size_t numIndices;
};
#endif //OPENGLHOUSE_MATERIAL_H
