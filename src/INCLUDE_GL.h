/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#ifndef OPENGLHOUSE_INCLUDE_GL_H
#define OPENGLHOUSE_INCLUDE_GL_H

#if defined(OSX) || defined(__APPLE__)
    #include <OpenGL/gl3.h>
#else
    #include <GL/glew.h>
#endif

#endif //OPENGLHOUSE_INCLUDE_GL_H
