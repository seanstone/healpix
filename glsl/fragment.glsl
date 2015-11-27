#version 120

uniform sampler2D Sampler;
varying vec2 UV;

void main()
{
	gl_FragColor = texture2D(Sampler, UV);
}
