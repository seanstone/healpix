#define GL_GLEXT_PROTOTYPES 1
#define GL3_PROTOTYPES 1
#include <GL/gl.h>

#include <string>
#include "shader.hpp"

class Renderer
{
	GLuint shaderProgram;
	int startTime;

	// Uniforms
	GLuint iResolution, iGlobalTime, iMouse;
	// Attributes
	GLuint aVertexPosition, vertexPositionBuffer;

public:

	void init()
	{
		initAttribBuffers();
		initOptions();

		emscripten_wget("glsl/shadertoy.vs", "glsl/shadertoy.vs");
		emscripten_wget("glsl/simple.fs", "glsl/simple.fs");
		std::string vsSource, fsSource;
		// vsSource = FS.readFile("glsl/shadertoy.vs", opts);
		// fsSource = FS.readFile("glsl/simple.fs", opts);
		// shaderProgram = compileProgram(vsSource, fsSource)
		glUseProgram(shaderProgram);
		initShaderVars();
	}

	void initOptions()
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		printf("Render options initialized");
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

		printf("Shader variables initialized");
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
		printf("Attribute buffers initialized");
	}

	void start()
	{
		//startTime = new Date().getTime();
		//setInterval(draw, 50);
		printf("Render loop started");
	}

	void draw()
	{
		//var seconds = (new Date().getTime() - startTime) / 1000;

		// Clear buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Uniforms
		//glUniform3f(iResolution, GL_drawingBufferWidth, GL_drawingBufferHeight, GL_drawingBufferWidth*1./GL_drawingBufferHeight);
		//glUniform1f(iGlobalTime, seconds);
		glUniform4f(iMouse, 0, 0, 0, 0); // TODO: implement mouse position

		// Attribute: vertexPosition
		glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);
		glVertexAttribPointer(aVertexPosition, 3, GL_FLOAT, false, 0, 0);

		// Draw
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

};
