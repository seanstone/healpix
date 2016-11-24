// #define WEBGL
// #ifdef WEBGL
precision mediump float;
// #else
// //#version 330 core
// #endif

uniform mat4 transformMatrix;
uniform mat4 cameraMatrix;
uniform mat4 projectionMatrix;

// #ifdef WEBGL

attribute vec3 position;
attribute vec2 textCoord;
attribute vec3 normal;

varying vec3 vertexCoord;
varying vec2 textureCoord;
varying vec3 normalVec;

// #else
//
// layout (location = 0) in vec3 position;
// layout (location = 1) in vec2 textCoord;
// layout (location = 2) in vec3 normal;
//
// out vec3 vertexCoord;
// out vec2 textureCoord;
// out vec3 normalVec;
//
// #en//dif

void main()
{
	gl_Position = projectionMatrix*cameraMatrix*transformMatrix*vec4(position,1.0);
	//textureCoord = textCoord;
	normalVec = (transformMatrix*vec4(normal,0.0)).xyz;
	// vertexCoord = (transformMatrix*vec4(position,1.0)).xyz;
}
