#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

// GLEW
#include <GL/glew.h>

// GLFW
#define GLFW_DLL
#include <GLFW/glfw3.h>

#include "Shader.hpp"
#include "Texture.hpp"
#include "View.hpp"
#include "HEALPix.hpp"

class Graphics
{
    int width = 1024, height = 768;
    GLFWwindow* window;

    float lastTime;

    GLuint program;
    GLuint heightMapTexture;

    View view;
    HEALPix<7> healpix;

public:
    int init();
    int initWindow();
    int initOptions();
    int initProgram();
    int initTexture();
    int loop();
    void inputs();
    void draw();
    int terminate();
    void update();

    Graphics(): healpix(3.0)
    {
        init();
    }

    ~Graphics()
    {
        terminate();
    }
};

#endif // GRAPHICS_H_INCLUDED
