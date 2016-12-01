#include <CanvasShader.hpp>
#include <PulsarTexture.hpp>
#include <Pulsar.hpp>
using namespace Pulsar;

// void CanvasShader::setTexture(Texture* tex)
// {
// 	texture = tex;
// }

bool CanvasShader::compile()
{
	bool success = true;
	success = Shader::compile();
	if(success == false)
		return false;

	// Check if all uniforms we need are avliable
	for(string& str : requiredUniform)
	{
		if(getUniform(str) == -1)
		{
			cout << "Warning : Missing uniform \"" << str << "\" in shader." << endl;
			success = false;
		}
	}

	return success;
}

void CanvasShader::bind()
{
	// if(texture != NULL)
	// 	texture->bind();

	Shader::bind();
	updateInternalParameters();
}

void CanvasShader::unbind()
{
	// if(texture != NULL)
	// 	texture->unbind();
	Shader::unbind();
}

// void CanvasShader::setTransformation(mat4 transformation)
// {
// 	//transformMatrix = transformation;
// }
//
// void CanvasShader::setCamera(Camera* cam)
// {
// 	//camera = cam;
// }
//
// void CanvasShader::setProjection(Projection* project)
// {
// 	//projection = project;
// }

void CanvasShader::updateInternalParameters()
{
	// setParameter("transformMatrix",transformMatrix);
	// setParameter("cameraMatrix",camera->getCameraMatrix());
	// setParameter("projectionMatrix",projection->getProjectionMatrix());
}
