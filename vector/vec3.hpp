#ifndef VEC3_HPP
#define VEC3_HPP

#include "normed.hpp"
#include "vec2.hpp"

template <class scalar>
struct Vec3 : NormedVec<3, scalar, Vec3<scalar>>, VecConstructor<3, scalar, Vec3<scalar>>
{
	using VecConstructor<3, scalar, Vec3<scalar>>::VecConstructor;
	Vec3() {};

	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wpedantic"
	union {
		struct { scalar x, y, z; };
		struct { scalar r, g, b; };
		struct { scalar i, j, k; };
		struct { NormedArray<2, scalar, Vec2<scalar>> xy; };
		struct { scalar x_; NormedArray<2, scalar, Vec2<scalar>> yz; };
	};
	#pragma GCC diagnostic pop

	// TODO: cross
};

//typedef Vec3<float> vec3;

#endif
