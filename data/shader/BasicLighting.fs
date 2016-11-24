#version 330 core
//#define WEBGL
#ifdef WEBGL
precision mediump float;
#endif

#ifdef WEBGL

varying vec4 color;

varying vec3 vertexCoord;
varying vec3 normalVec;
varying vec2 textureCoord;

uniform sampler2D surfaceTexture;

#else

out vec4 color;

in vec3 vertexCoord;
in vec3 normalVec;
in vec2 textureCoord;

uniform sampler2D surfaceTexture;

#endif

struct PointLight
{
	vec3 position;
	vec3 radiant;
};

uniform PointLight pointLight;

void main()
{
	vec3 diff = pointLight.position-vertexCoord;
	vec3 directionVec = normalize(diff);
	float dist = length(diff);
	vec3 lightStrengthOnFragment = pointLight.radiant*(dot(normalVec,directionVec)/(dist*dist));

	#ifdef WEBGL
	vec4 textureColor = texture2D(surfaceTexture,textureCoord);
	#else
	vec4 textureColor = texture(surfaceTexture,textureCoord);
	#endif

	if(dot(textureColor.xyz,vec3(1)) == 0.)//If no texture is applied
		textureColor = vec4(1,1,1,1);

	//Apply lighting
	vec3 renderedColor = textureColor.xyz*lightStrengthOnFragment;

	//Gamma correction
	const float gamma = 2.2;
	const float inverseGamma = 1.0/gamma;
	renderedColor = max(renderedColor,vec3(0,0,0));
	vec3 correctedColor = pow(renderedColor,vec3(inverseGamma));

	#ifdef WEBGL
	gl_FragColor = vec4(correctedColor,1);
	#else
	color = vec4(correctedColor,1);
	#endif
}
