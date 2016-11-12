precision mediump float;
uniform vec3 	iResolution;
uniform float 	iGlobalTime;
uniform vec4 	iMouse;

struct Sphere
{
	vec3 origin;
	float radius;
};

struct Camera
{
	vec3 position;
	vec2 resolution;
	float fov;
	vec3 right;
	vec3 up;
	vec3 dir;
	float aspectRatio;
};
// TODO: make camera uniform

struct Ray
{
	vec3 origin;
	vec3 dir;
};

Ray createCameraRay(Camera camera, vec2 uv)
{
	vec3 rayDir = (1. - 2. * uv.x) * tan(camera.fov * .5) * camera.right * camera.aspectRatio
				+ (1. - 2. * uv.y) * tan(camera.fov * .5) * camera.up
				+ camera.dir;
	rayDir = normalize(rayDir);
	return Ray(camera.position, rayDir);
}

bool findSphereIntersection(Sphere sphere, Ray ray)
{
	vec3 c = sphere.origin;
	float r = sphere.radius;
	vec3 l = ray.dir;
	vec3 o = ray.origin;

	float a = - dot(o-c,l) / dot(l,l);
	float d = sqrt(dot(o-c,o-c) + 2.*a*dot(o-c,l) + a*a*dot(l,l));
	return d < r;
}

void main()
{
	Camera camera;
	camera.position = vec3(0,0,2);
	camera.resolution = iResolution.xy;
	camera.fov = radians(90.);
	camera.right = vec3(1,0,0);
	camera.up = vec3(0,1,0);
	camera.dir = cross(camera.right, camera.up);
	camera.aspectRatio = iResolution.x / iResolution.y;

	Sphere sphere;
	sphere.origin = vec3(0,0,0);
	sphere.radius = 3.;

	Ray ray = createCameraRay(camera, gl_FragCoord.xy / iResolution.xy);
	float r = findSphereIntersection(sphere, ray) ? 1. : 0.;
	gl_FragColor = vec4(r, r, r, 1);
}
