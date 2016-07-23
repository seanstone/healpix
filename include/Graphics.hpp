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
#include "HEALPixDrawable.hpp"

class Graphics
{
    int width = 1024, height = 768;
    GLFWwindow* window;

    float lastTime;

    HEALPixDrawable healpix;
    View view;

public:
    int init();
    int initWindow();
    int initOptions();
    int loop();
    void inputs();
    void draw();
    int terminate();
    void update();

    ~Graphics()
    {
        terminate();
    }
};

#endif // GRAPHICS_H_INCLUDED
