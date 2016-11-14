#ifndef VEC4_HPP
#define VEC4_HPP

#include "normed.hpp"
#include "vec3.hpp"

template <class scalar>
struct Vec4 : NormedVec<4, scalar, Vec4<scalar>>, VecConstructor<4, scalar, Vec4<scalar>>
{
	using VecConstructor<4, scalar, Vec4<scalar>>::VecConstructor;
	Vec4() {};

	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wpedantic"
	union {
		struct { scalar w, x, y, z; }; // NOTE the order of components!
		struct { scalar r, g, b, a; };
		struct { NormedArray<2, scalar, Vec2<scalar>> wx, yz; };
		struct { scalar w_; NormedArray<2, scalar, Vec2<scalar>> xy; };
		struct { scalar w__; NormedArray<3, scalar, Vec3<scalar>> xyz; };
		struct { NormedArray<3, scalar, Vec3<scalar>> wxy; };
		struct { NormedArray<2, scalar, Vec2<scalar>> rg, ba; };
		struct { scalar r_; NormedArray<2, scalar, Vec2<scalar>> gb; };
		struct { scalar r__; NormedArray<3, scalar, Vec3<scalar>> gba; };
		struct { NormedArray<3, scalar, Vec3<scalar>> rgb; };
	};
	#pragma GCC diagnostic pop
};

//typedef Vec4<float> vec4;

#endif
