#define GL_GLEXT_PROTOTYPES 1
#define GL3_PROTOTYPES 1
#include <GL/gl.h>

#include <string>
#include "shader.hpp"

#include <iostream>
#include <fstream>

class Renderer
{
	GLuint shaderProgram;
	int startTime, seconds;

	int width, height, isFullscreen;

	// Uniforms
	GLuint iResolution, iGlobalTime, iMouse;
	// Attributes
	GLuint aVertexPosition, vertexPositionBuffer;

public:

	std::string readAllText(std::string path)
	{
		std::ifstream ifs(path);

		if(ifs.good() == false)
		{
			std::cout << "Failed to open file " << path << std::endl;
			return "";
		}

		std::string content( (std::istreambuf_iterator<char>(ifs) ),
			(std::istreambuf_iterator<char>()));
		return content;
	}

	void init()
	{
		printf("init()\r\n");

		initAttribBuffers();
		initOptions();

		emscripten_wget("glsl/shadertoy.vs", "glsl/shadertoy.vs");
		emscripten_wget("glsl/simple.fs", "glsl/simple.fs");
	 	const char* vsSource = readAllText("glsl/shadertoy.vs").c_str();
		const char* fsSource = readAllText("glsl/simple.fs").c_str();
		shaderProgram = compileProgram(&vsSource, &fsSource);
		glUseProgram(shaderProgram);
		initShaderVars();
		//
		// emscripten_get_canvas_size(&width, &height, &isFullscreen);
	}

	void initOptions()
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		printf("Render options initialized\r\n");
	}

	void initShaderVars()
	{
		// Init uniforms
		iResolution = 	glGetUniformLocation(shaderProgram, "iResolution");
		iGlobalTime = 	glGetUniformLocation(shaderProgram, "iGlobalTime");
		iMouse =  		glGetUniformLocation(shaderProgram, "iMouse");

		// Init attributes
		aVertexPosition = glGetAttribLocation(shaderProgram, "aVertexPosition");
		glEnableVertexAttribArray(aVertexPosition);

		printf("Shader variables initialized\r\n");
	}

	void initAttribBuffers()
	{
        glGenBuffers(1, &vertexPositionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);

		float vertices[12] = {
	    	1.0,  1.0,  0.0,
	    	-1.0, 1.0,  0.0,
	    	1.0,  -1.0, 0.0,
	    	-1.0, -1.0, 0.0
	  	};
	  	glBufferData(GL_ARRAY_BUFFER, 12, vertices, GL_STATIC_DRAW);
		printf("Attribute buffers initialized\r\n");
	}

	void draw()
	{
		//var seconds = (new Date().getTime() - startTime) / 1000;

		// Clear buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Uniforms
		glUniform3f(iResolution, width, height, width*1./height);
		glUniform1f(iGlobalTime, seconds);
		glUniform4f(iMouse, 0, 0, 0, 0); // TODO: implement mouse position

		// Attribute: vertexPosition
		glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);
		glVertexAttribPointer(aVertexPosition, 3, GL_FLOAT, false, 0, 0);

		// Draw
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

};
