#include "Sphere.hpp"
using namespace Pulsar;

float3 findIntersection(Ray ray, Sphere sphere)
{
    float3 c = ray.origin;
    float3 d = ray.direction;
    float3 o = sphere.Origin;
    float r = sphere.Radius;

    float b = dot(o-c, d);

    float D = b*b - dot(d,d) * ( dot(o-c,o-c) - r*r );

    if (D >= 0)
    {
        float a = (b - sqrt(D)) / (dot(d,d));
        return c + a * d;
    }
    else return float3(0);
}
