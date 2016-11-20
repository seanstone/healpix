#ifndef COMPLEX_HPP_INCLUDED
#define COMPLEX_HPP_INCLUDED

#include "vec2.hpp"

template <class scalar>
struct Complex : NormedVec<2, scalar, Complex<scalar>>, VecConstructor<2, scalar, Complex<scalar>>
{
	scalar x, y;

	using VecConstructor<2, scalar, Complex<scalar>>::VecConstructor;

	/* Complex multiplication */	// NOTE: overrides normed vector dot prodeuct

	inline constexpr Complex& operator*=(const scalar& rhs) 	{ return NormedVec<2, scalar, Complex<scalar>>::operator*=(rhs); }
	inline constexpr Complex& operator*=(const Complex& rhs) 	{ return *this = *this * rhs; }
	inline constexpr friend Complex operator*(const Complex& lhs, const Complex& rhs) { return Complex(lhs.x*rhs.x - lhs.y*rhs.y, lhs.x*rhs.y + lhs.y*rhs.x); }

	/* Complex division */

	inline constexpr Complex& operator/=(const scalar& rhs) 	{ return NormedVec<2, scalar, Complex<scalar>>::operator/=(rhs); }
	inline constexpr Complex& operator/=(const Complex& rhs) 	{ return *this = *this / rhs; }
	inline constexpr friend Complex operator/(const Complex& lhs, const Complex& rhs) { return Complex(lhs.x*rhs.x + lhs.y*rhs.y, lhs.y*rhs.x - lhs.x*rhs.y)/rhs.norm2(); }

	/* Complex conjugate */			// NOTE: overrides normed vector conjugate

	inline constexpr friend Complex operator~(const Complex& rhs) { return Complex(rhs.x, -rhs.y); }

	// TODO: power ^ operator
	// TODO: comparsion operators

	/* Increment and decrement */

	inline constexpr Complex& operator++() 		{ return *this += 1; }
	inline constexpr Complex operator++(int)	{ return operator++(), Complex(*this-1); }
	inline constexpr Complex& operator--() 		{ return *this -= 1; }
	inline constexpr Complex operator--(int)	{ return operator--(), Complex(*this+1); }

	/* Stream operators */

	inline friend std::ostream& operator<<(std::ostream& os, const Complex& v)
	{
		if (v.x || !v.y) 		os << v.x;
		if (v.x && v.y > 0) 	os << "+";
		if (v.y) 				os << v.y << "i";
		return os;
	}
};

typedef Complex<float> complex;
const complex I = {0, 1};

struct RadialComplex
{
	// TODO
};

#endif
