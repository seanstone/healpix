#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) \
 || defined(__TOS_WIN__) || defined(__WINDOWS__)
/* Compiling for Windows */
#ifndef __WINDOWS__
#define __WINDOWS__
#endif
#  include <windows.h>
#endif/* Predefined Windows macros */

#include "graphics.h"

#include <iostream>
#include <ctime>

using namespace std;

int main()
{
    graphics Graphics;
    Graphics.loop();

    return 0;
}

/*#include <windows.h>
extern "C" _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;*/

/* TODO:

perspective transformation matrix
mat4 and linear algebra
put VBO inside healpix class
Plucker coordinates and coordinates based on ray tracing
UI
render text
CLOD

*/
