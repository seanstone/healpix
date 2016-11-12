precision mediump float;
uniform vec3 	iResolution;
uniform float 	iGlobalTime;
uniform vec4 	iMouse;

//http://stackoverflow.com/questions/15057720/read-pixels-in-webgltexture-rendering-webgl-to-texture

/* HEALPix */

/* (tht, phi) --> (x,y) --> pixel */

const int H = 4, K = 3;

struct Pixel
{
    int f;
    vec2 ij;
};

Pixel P(vec2 xy)
{
	vec2 uv = mat2({-1, -1, 1, -1}) * (xy / facet_xydim());
	vec2 hk = uv + vec2(0.5) + vec2((K-1)/4.0, -(K-1)/4.0);
	vec2 hkc = floor(hk);
	int h = -hkc.x;
	int k = -hkc.y - h;
	int f = h + H * k;
	vec2 ij = Dim * (hk - hkc);
	return Pixel(f, ij);
}

vec2 XY(float tht, float phi)
{
	float x, y;
	if (abs(tht) < tht_x())
	{
		x = phi;
		y = M_PI_2 * K / H * sin(tht);
	}
	else
	{
		float phi_c = 0;
		float tht_c = 0;
		for(int h=-H; h<H; h++)
		{
			float c = h * 2 * M_PI / H ;
			if (abs(c - phi) < abs(phi_c - phi)) phi_c = c;
		}
		for(int k=-K; k<K; k++)
		{
			float c = k * M_PI / (K+1) ;
			if (abs(c - tht) < abs(tht_c - tht)) tht_c = c;
		}

		x = phi_c + (phi - phi_c) * sigma_t(tht);
		if (x < -M_PI) x += M_PI;
		if (x > M_PI) x -= M_PI;

		y = (tht > 0 ? 1:-1) * M_PI / H * ( (K+1)/2.0 - sigma_t(tht) );
	}
	return vec2(x, y);
}

/* pixel --> (x,y) --> (tht,phi) */

float2 XY(Pixel p)
{
	//cout << "f:" << p.f << " ij:" << p.ij << "\t";
	float2 ij = p.ij;
	//if (ij.x == 0) ij.x += 10-3;
	//if (ij.y == 0) ij.y += 10-3;
	//if (ij.x == Dim) ij.x -= 10-3;
	//if (ij.y == Dim) ij.y -= 10-3;
	float2 uv = uv_c(p.f) + (float2)ij/(float)Dim - float2(0.5);
	float2 xy = facet_xydim() * (mat2({-1, 1, -1, -1}) / 2.0f * uv);
	//cout << xy << "\t";
	return xy;
}

float2 PhiSintht(float2 xy, int f)
{
	int h = f%H, k = f/H;
	float x = xy.x, y = xy.y;
	float phi       = fabs(y) <= y_x() ?   x : x_c(h, k) + (x-x_c(h, k)) / sigma(y);
	float sintht    = fabs(y) <= y_x() ?   y * H / M_PI_2 / K : (y > 0 ? 1:-1) * ( 1 - sigma(y)*sigma(y)/K );
	return float2(phi, sintht);
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

	/*
	switch (texture2D(texture, texcoord))
	{

	}
	*/

	// TODO: healpix grid?
}
