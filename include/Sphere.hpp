#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <PulsarRay.hpp>

namespace Pulsar
{

struct Sphere
{
    float   Radius;
    float3  Origin;
};

float3 findIntersection(Pulsar::Ray ray, Sphere sphere);

}

#endif
