#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <PulsarShader.hpp>

class CanvasShader : public Pulsar::Shader
{
public:
	// void setTexture(Texture* tex);
	virtual bool compile();
	virtual void bind();
	virtual void unbind();
	// void setTransformation(mat4 transformation);
	// void setCamera(Camera* cam);
	// void setProjection(Projection* project);
	void updateInternalParameters();
	std::string requiredUniform [0]; // = {}; //{"transformMatrix","cameraMatrix","projectionMatrix"};

protected:
	// Texture* texture = NULL;
	// mat4 transformMatrix;
	// Camera* camera = NULL;
	// Projection* projection = NULL;
};

#endif
