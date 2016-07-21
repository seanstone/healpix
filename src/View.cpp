#include "View.hpp"

#include <iostream>

void View::inputs(GLFWwindow* window)
{
    /*static float lastTime = glfwGetTime();
    float currentTime = glfwGetTime();

    // Cursor position
    double u, v; glfwGetCursorPos(window, &u, &v);
    float x = 2*u/Width-1.0, y = -2*v/Height+1.0;

    // Rotate
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        quat RotateVector = ~Rotor * ((u-Width/2)*quat(0,-1,0) + (v-Height/2)*quat(-1,0,0)) * Rotor;
        Rotor *= rotor(RotationSpeed, RotateVector);
    }

    lastTime = currentTime;*/
}

/*
mat4 View::Simple()
{
    mat4 ProjectionMatrix = mat4(0.3);
    ProjectionMatrix[0][0] *= Height/Width;
    ProjectionMatrix[3][3] = 1.0;
    mat4 ViewMatrix = mat4(1.0);

	return ProjectionMatrix * ViewMatrix;
}

mat4 View::Orthographic(float magnification, float near, float far)
{
    mat4 ProjectionMatrix = mat4(magnification);
    //ProjectionMatrix[0][0] *= Height/Width;
    //ProjectionMatrix[2][2] /= (far-near);
    //ProjectionMatrix[3][3] = 1.0;
    mat4 ViewMatrix = mat4(1.0);

	return ProjectionMatrix * ViewMatrix;
}
*/

mat4 View::Perspective(double fovy, double aspect, double near, double far)
{
	double tanHalfFovy = tan(fovy / 2.0);

	mat4 P(1.0);
	P[0][0] = 1.0 / (aspect * tanHalfFovy);
	P[1][1] = 1.0 / (tanHalfFovy);
	P[2][2] = - (far + near) / (far - near);
	P[3][2] = - 1.0;
	P[2][3] = - (2.0 * far * near) / (far - near);
	return P;
}

mat4 View::lookAt(vec3 const & eye, vec3 const & center, vec3 const & up)
{
	vec3 f = normalize(vec3(center - eye));
	vec3 s = normalize(cross(f, up));
	vec3 u = cross(s, f);

	mat4 Result(1);
	Result[0][0] = s.x;
	Result[0][1] = s.y;
	Result[0][2] = s.z;
	Result[1][0] = u.x;
	Result[1][1] = u.y;
	Result[1][2] = u.z;
	Result[2][0] =-f.x;
	Result[2][1] =-f.y;
	Result[2][2] =-f.z;
	Result[0][3] =-dot(s, eye);
	Result[1][3] =-dot(u, eye);
	Result[2][3] = dot(f, eye);
	return Result;
}
