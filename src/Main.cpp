#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
    #ifndef __WINDOWS__
        #define __WINDOWS__
    #endif
    #include <windows.h>
#endif

#include "Graphics.hpp"

int main()
{
    Graphics graphics;
    graphics.loop();

    return 0;
}

/* TODO:

perspective transformation matrix
mat4 and linear algebra
put VBO inside healpix class
Plucker coordinates and coordinates based on ray tracing
UI
render text
CLOD

*/
