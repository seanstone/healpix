precision mediump float;
uniform vec3 	iResolution;
uniform float 	iGlobalTime;
uniform vec4 	iMouse;

//http://stackoverflow.com/questions/15057720/read-pixels-in-webgltexture-rendering-webgl-to-texture

struct Sphere
{
	vec3 origin;
	float radius;
};
// TODO: make sphere uniform

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

struct Intersection
{
	bool intersect;
	vec3 point;
};

Ray createCameraRay(Camera camera, vec2 uv)
{
	vec3 rayDir = (1. - 2. * uv.x) * tan(camera.fov * .5) * camera.right * camera.aspectRatio
				+ (1. - 2. * uv.y) * tan(camera.fov * .5) * camera.up
				+ camera.dir;
	rayDir = normalize(rayDir);
	return Ray(camera.position, rayDir);
}

Intersection findSphereIntersection(Sphere sphere, Ray ray)
{	/* https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection */
	vec3 c = sphere.origin;
	float r = sphere.radius;
	vec3 l = ray.dir;
	vec3 o = ray.origin;

	float D = dot(l,c-o) * dot(l,c-o) - dot(c-o,c-o) + r*r;
	if (D < 0.) return Intersection(false, vec3(0));
	float a = dot(l,c-o) + sqrt(D);
	if (a < 0.) return Intersection(false, vec3(0));
	return Intersection(true, o + a*l);
}

void main()
{
	Camera camera;
	camera.position = vec3(0,0,-4);
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
	Intersection intersection = findSphereIntersection(sphere, ray);
	float r = sphere.radius;
	vec3 p = intersection.point/r * .5 + 1.;
	if (!intersection.intersect) p = vec3(0,0,0);
	gl_FragColor = vec4(p * 0.5, 1);
}
