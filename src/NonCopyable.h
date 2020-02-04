/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#ifndef OPENGLHOUSE_NONCOPYABLE_H
#define OPENGLHOUSE_NONCOPYABLE_H

/// This makes a child class unable to be copied. This is because
/// that classes that interact directly with gpu memory (creating buffers, textures, etc)
/// creating and destroying objects in gpu memory when copied will end very badly
class NonCopyable
{
public:
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};

#endif //OPENGLHOUSE_NONCOPYABLE_H
