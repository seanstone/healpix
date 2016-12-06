#ifndef INTERSECT_HPP
#define INTERSECT_HPP

#include <PulsarMath.hpp>

// FIXME: no "using" in headers
using Pulsar::vec3;
using Pulsar::vec2;

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

Intersection findSphereIntersection(Sphere sphere, Ray ray)
{	/* https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection */
	vec3 c = sphere.origin;
	float r = sphere.radius;
	vec3 l = ray.dir;
	vec3 o = ray.origin;

	float D = dot(l,c-o) * dot(l,c-o) - dot(c-o,c-o) + r*r;
	if (D < 0.f) return Intersection({false, vec3(0.f)});
	float a = dot(l,c-o) + sqrt(D);
	if (a < 0.f) return Intersection({false, vec3(0.f)});
	return Intersection({true, o + a*l});
}

Ray createCameraRay(Camera camera, vec2 uv)
{
	vec3 rayDir = (1.f - 2.f * uv.x) * tan(camera.fov * .5f) * camera.right * camera.aspectRatio
				+ (1.f - 2.f * uv.y) * tan(camera.fov * .5f) * camera.up
				+ camera.dir;
	rayDir = normalize(rayDir);
	Ray ray = {camera.position, rayDir};
	return ray;
}

#endif
