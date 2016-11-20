#ifndef VEC2_HPP
#define VEC2_HPP

#include "normed.hpp"

template <class scalar>
struct Vec2 : NormedVec<2, scalar, Vec2<scalar>>, VecConstructor<2, scalar, Vec2<scalar>>
{
	using VecConstructor<2, scalar, Vec2<scalar>>::VecConstructor;
	Vec2() {};

	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wpedantic"
	union {
		struct { scalar x, y; };
		struct { scalar u, v; };
		struct { scalar i, j; };
	};
	#pragma GCC diagnostic pop
};

//typedef Vec2<float> vec2;

#endif
