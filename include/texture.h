#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLuint loadBMP(const char * imagepath);
GLuint loadDDS(const char * imagepath);
//GLuint loadTGA(const char * imagepath);

#endif
