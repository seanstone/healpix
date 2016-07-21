#include "Shader.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

int loadShader(const char* shader_filepath, GLuint &Shader)
{
    // Read shader code
    std::string ShaderCode = "";
	std::ifstream ShaderStream(shader_filepath, std::ios::in);
	if(ShaderStream.is_open())
    {
		std::string Line = "";
		while(getline(ShaderStream, Line)) ShaderCode += "\n" + Line;
		ShaderStream.close();
	}
	else { printf("Failed to open %s.\n", shader_filepath); getchar(); return -1; }

    // Compile shader
	printf("Compiling shader: %s\n", shader_filepath);
	char const* Source = ShaderCode.c_str();
	glShaderSource(Shader, 1, &Source , NULL);
	glCompileShader(Shader);

	// Check shader
	GLint Result = GL_FALSE; int InfoLogLength;
	glGetShaderiv(Shader, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(Shader, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
    {
		char ShaderErrorMessage[InfoLogLength+1];
		glGetShaderInfoLog(Shader, InfoLogLength, NULL, ShaderErrorMessage);
		printf("%s\n", &ShaderErrorMessage[0]);
	}

	return 0;
}

GLuint loadShaders(const char * vertex_filepath, const char * fragment_filepath){

	// Create the shaders
	GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// load code into shader
	loadShader(vertex_filepath, VertexShader);
	loadShader(fragment_filepath, FragmentShader);

	// Link the program
	printf("Linking program\n");
	GLuint Program = glCreateProgram();
	glAttachShader(Program, VertexShader);
	glAttachShader(Program, FragmentShader);
	glLinkProgram(Program);

	// Check the program
	GLint Result = GL_FALSE; int InfoLogLength;
	glGetProgramiv(Program, GL_LINK_STATUS, &Result);
	glGetProgramiv(Program, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
    {
		char ProgramErrorMessage[InfoLogLength+1];
		glGetProgramInfoLog(Program, InfoLogLength, NULL, ProgramErrorMessage);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);

	return Program;
}
