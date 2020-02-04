/**
  * Created by Earl Kennedy
  * https://github.com/Mnenmenth
  */

#ifndef OPENGLHOUSE_INCLUDE_GLUT_H
#define OPENGLHOUSE_INCLUDE_GLUT_H

#if defined(OSX) || defined(__APPLE__)
        #if defined(FREEGLUT)
            #include <GLUT/freeglut.h>
        #else
            #include <GLUT/glut.h>
        #endif
    #else
        #ifdef FREEGLUT
            #include <GL/freeglut.h>
        #else
            #include <GL/glut.h>
    #endif
#endif

#endif //OPENGLHOUSE_INCLUDE_GLUT_H
