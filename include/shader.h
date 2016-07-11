#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

int loadShader(const char* shader_filepath, GLuint &Shader);
GLuint loadShaders(const char* vertex_filepath, const char* fragment_filepath);

#endif
