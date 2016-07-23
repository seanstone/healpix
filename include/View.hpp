#ifndef VIEW_H_INCLUDED
#define VIEW_H_INCLUDED

#include "Quaternion.hpp"
#include "Vec.hpp"

// GLEW
#include <GL/glew.h>

// GLFW
#define GLFW_DLL
#include <GLFW/glfw3.h>

struct View
{
    float width = 1024, height = 768;

    //quat Rotor = quat(1.0);
    //float RotationSpeed = 1E-1f;
    //float FoV = M_PI/180.0 *45.0f;
    //float FoV_min = M_PI/180.0 * 5.0f, FoV_max = M_PI/180.0 * 135.0f;
    //float ZoomSpeed = M_PI/180.0 * 50.0f;

    void inputs(GLFWwindow* window);
    /*mat4 Simple();
    mat4 Orthographic(float magnification, float near, float far);*/
    mat4 Perspective(double fovy, double aspect, double near, double far);
    mat4 lookAt(vec3 const& eye, vec3 const& center, vec3 const& up);
};

#endif // VIEW_H_INCLUDED
