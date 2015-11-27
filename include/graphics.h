#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

// GLEW
#include <GL/glew.h>

// GLFW
#define GLFW_DLL
#include <GLFW/glfw3.h>

#include "shader.h"
#include "texture.h"
#include "view.h"
#include "healpix.h"

class graphics
{
        int Width = 1024, Height = 768;
        GLFWwindow* Window;

        float lastTime;

        GLuint Program;
        GLuint Texture;

        view View;
        HEALPix Healpix;

    public:
        int init();
        int initWindow();
        int initOptions();
        int initProgram();
        int initTexture();
        int loop();
        void inputs();
        void draw();
        int cleanup();
        void update();

        graphics(): Healpix(7, 3.0)
        {
            init();
        }

        ~graphics()
        {
            cleanup();
        }
};

#endif // GRAPHICS_H_INCLUDED
