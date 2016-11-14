#ifndef VEC6_HPP
#define VEC6_HPP

#include "normed.hpp"
#include "vec3.hpp"

template <class scalar>
struct Vec6 : NormedVec<6, scalar, Vec6<scalar>>, VecConstructor<6, scalar, Vec6<scalar>>
{
	using VecConstructor<6, scalar, Vec6<scalar>>::VecConstructor;
	Vec6() {};

	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wpedantic"
	union {
		struct { scalar d1, d2, d3, m1, m2, m3; };
		struct { NormedArray<3, scalar, Vec3<scalar>> d, m; };
	};
	#pragma GCC diagnostic pop
};

//typedef Vec4<float> vec4;

#endif
