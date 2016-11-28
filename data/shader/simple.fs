precision mediump float;
uniform vec3 	iResolution;
uniform float 	iGlobalTime;
uniform vec4 	iMouse;

#define M_PI 3.14159265358979
#define M_PI_2 (M_PI/2.)
#define M_PI_4 (M_PI/4.)

//http://stackoverflow.com/questions/15057720/read-pixels-in-webgltexture-rendering-webgl-to-texture

/* HEALPix */

/* (tht, phi) --> (x,y) --> pixel */

uniform int Dim;
const int H = 4, K = 3;

float facet_xwidth_2()   	{ return M_PI / float(H); }
float facet_yheight_2()  	{ return M_PI / (float(K)+1.); }
float facet_xwidth()     	{ return 2. * facet_xwidth_2(); }
float facet_yheight()    	{ return 2. * facet_yheight_2(); }
vec2 facet_xydim()     		{ return vec2(facet_xwidth(), facet_yheight()); }
float x_c(int h, int k)  	{ return float(h) * facet_xwidth() + float(k) * facet_xwidth_2(); }
float y_c(int k)         	{ return (float(k) - (float(K)-1.)/2.) * facet_yheight_2(); }
vec2 uv_c(int f)
{
	int h = int(mod(float(f),float(H))), k = f/H;
	float uc = - float(h) - (float(K)-1.)/4.;
	float vc = - float(h) - float(k) + (float(K)-1.)/4.;
	return vec2(uc, vc);
}
float y_x()              	{ return M_PI_2 * (float(K)-1.) / float(H); }
float sigma(float y)     	{ return (float(K)+1.)/2. - abs(y*float(H))/M_PI; }

float tht_x()              	{ return asin((float(K)-1.)/float(K)); }
float sigma_t(float tht)	{ return sqrt(float(K) * (1.-abs(sin(tht)))); }

struct Pixel
{
    int f;
    vec2 ij;
};

Pixel P(vec2 xy)
{
	vec2 uv = mat2(-1, -1, 1, -1) * (xy / facet_xydim());
	vec2 hk = uv + vec2(.5) + vec2((float(K)-1.)/4., -(float(K)-1.)/4.);
	vec2 hkc = floor(hk);
	int h = int(-hkc.x);
	int k = int(-hkc.y) - h;
	int f = h + H * k;
	vec2 ij = float(Dim) * (hk - hkc);
	return Pixel(f, ij);
}

vec2 XY(float tht, float phi)
{
	float x, y;
	if (abs(tht) < tht_x())
	{
		x = phi;
		y = M_PI_2 * float(K) / float(H) * sin(tht);
	}
	else
	{
		float phi_c = 0.;
		float tht_c = 0.;
		for(int h=-H; h<H; h++)
		{
			float c = float(h) * 2. * M_PI / float(H) ;
			if (abs(c - phi) < abs(phi_c - phi)) phi_c = c;
		}
		for(int k=-K; k<K; k++)
		{
			float c = float(k) * M_PI / (float(K)+1.) ;
			if (abs(c - tht) < abs(tht_c - tht)) tht_c = c;
		}

		x = phi_c + (phi - phi_c) * sigma_t(tht);
		if (x < -M_PI) x += M_PI;
		if (x > M_PI) x -= M_PI;

		y = (tht > 0. ? 1.:-1.) * M_PI / float(H) * ( (float(K)+1.)/2. - sigma_t(tht) );
	}
	return vec2(x, y);
}

/* pixel --> (x,y) --> (tht,phi) */

vec2 XY(Pixel p)
{
	vec2 ij = p.ij;
	//if (ij.x == 0) ij.x += 10e-3;
	//if (ij.y == 0) ij.y += 10e-3;
	//if (ij.x == Dim) ij.x -= 10e-3;
	//if (ij.y == Dim) ij.y -= 10e-3;
	vec2 uv = uv_c(p.f) + ij/float(Dim) - vec2(.5);
	vec2 xy = facet_xydim() * (mat2(-1, 1, -1, -1) / 2. * uv);
	return xy;
}

vec2 PhiSintht(vec2 xy, int f)
{
	int h = int(mod(float(f),float(H))), k = f/H;
	float x = xy.x, y = xy.y;
	float phi       = abs(y) <= y_x() ?   x : x_c(h, k) + (x-x_c(h, k)) / sigma(y);
	float sintht    = abs(y) <= y_x() ?   y * float(H) / M_PI_2 / float(K) : (y > 0. ? 1.:-1.) * ( 1. - sigma(y)*sigma(y)/float(K) );
	return vec2(phi, sintht);
}

/* */

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
	if (D < 0.) return Intersection(false, vec3(0));
	float a = dot(l,c-o) + sqrt(D);
	if (a < 0.) return Intersection(false, vec3(0));
	return Intersection(true, o + a*l);
}

Ray createCameraRay(Camera camera, vec2 uv)
{
	vec3 rayDir = (1. - 2. * uv.x) * tan(camera.fov * .5) * camera.right * camera.aspectRatio
				+ (1. - 2. * uv.y) * tan(camera.fov * .5) * camera.up
				+ camera.dir;
	rayDir = normalize(rayDir);
	return Ray(camera.position, rayDir);
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

	/*
	switch (texture2D(texture, texcoord))
	{

	}
	*/

	// TODO: healpix grid?
}
