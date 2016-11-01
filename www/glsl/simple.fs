precision mediump float;
uniform vec3 iResolution;
uniform float iGlobalTime;
uniform vec4 iMouse;

// Sphere
const vec3 origin = vec3(0, 0, 0);
const vec3 camera = vec3(0, 0, 5);
const float radius = 2.;

void main()
{
	//SEA_TIME = iGlobalTime * SEA_SPEED;
	//EPSILON_NRM	= 0.1 / iResolution.x;
	gl_FragColor = vec4(gl_FragCoord.x/iResolution.x, 0, 0, 1);
}
