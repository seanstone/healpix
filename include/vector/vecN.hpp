#ifndef VECN_HPP
#define VECN_HPP

#include <math.h>
#ifndef M_PI
	#define M_PI 3.14159265358979
	#define M_PI_2 (M_PI/2.)
	#define M_PI_4 (M_PI/4.)
#endif
#include <string.h>
#include <iostream>

// TODO: use preprocessor directives instead

#define scalar float


struct Vec
{
	/* Component */

	inline constexpr scalar& operator[](int i) const { return *(reinterpret_cast<scalar*>(const_cast<Vec*>(this)) + i); }

	/* Copy assignment */

	inline constexpr Derived& operator=(const Derived& rhs) { return this == &rhs ?: memcpy(this, rhs, N*sizeof(scalar)), *this; };

	/* Move assignment */
	// TODO/FIXME: rvalue operators
	inline constexpr Derived& operator=(Derived&& rhs) { return memcpy(this, rhs, N*sizeof(scalar)), *this; };

	/* Negative and Positive */

	inline constexpr Derived& negative(int a=N-1) { return (*this)[a] = -(*this)[a], a ? negative(--a): *this; }
	inline constexpr friend Derived operator-(Derived rhs) { return rhs.negative(); }
	inline constexpr friend Derived operator+(const Derived& rhs) { return rhs; }

	/* Addition */

	inline constexpr void add(const Derived& rhs, int a=N-1) { return (*this)[a] += rhs[a], a ? add(rhs, --a): void(); }
	inline constexpr Derived& operator+=(const Derived& rhs) { return add(rhs), *this; }
  	inline constexpr friend Derived operator+(Derived lhs, const Derived& rhs) { return lhs += rhs; }

	/* Subtraction */

	inline constexpr void subtract(const Derived& rhs, int a=N-1) { return (*this)[a] -= rhs[a], a ? subtract(rhs, --a): void(); }
	inline constexpr Derived& operator-=(const Derived& rhs) { return subtract(rhs),  *this; }
	inline constexpr friend Derived operator-(Derived lhs, const Derived& rhs) { return lhs -= rhs; }

	/* Scalar multiplication */

	inline constexpr void multiply(const scalar& rhs, int a=N-1) { return (*this)[a] *= rhs, a ? multiply(rhs, --a): void(); }
	inline constexpr Derived& operator*=(const scalar& rhs) { return multiply(rhs), *this; }
	inline constexpr friend Derived operator*(Derived lhs, const scalar& rhs) { return lhs *= rhs; }
	inline constexpr friend Derived operator*(const scalar& lhs, Derived rhs) { return rhs *= lhs; }

	/* Scalar division */

	inline constexpr void divide(const scalar& rhs, int a=N-1) { return (*this)[a] /= rhs, a ? divide(rhs, --a): void(); }
	inline constexpr Derived& operator/=(const scalar& rhs) { return divide(rhs), *this; }
	inline constexpr friend Derived operator/(Derived lhs, const scalar& rhs) { return lhs /= rhs; }

	/* Comparison */

	inline constexpr friend bool equal(const Derived& lhs, const Derived& rhs, int a=N-1) { return lhs[a] == rhs[a] ? a ? equal(lhs, rhs, --a) : true : false; }
	inline constexpr friend bool operator==(const Derived& lhs, const Derived& rhs) { return &lhs == &rhs ?: equal(lhs, rhs); }
	inline constexpr friend bool operator!=(const Derived& lhs, const Derived& rhs) { return !(lhs == rhs); }

	/* Stream operators */

	inline constexpr friend void output(std::ostream& os, const Vec& rhs, int a=0) 	{ return os << rhs[a], a<N-1 ? (os << ", ", output(os, rhs, ++a)) : void(); }
	inline friend std::ostream& operator<<(std::ostream& os, const Vec& rhs)		{ return output(os, rhs), os; }

	// TODO: cin
	//inline friend std::istream& operator>>(std::istream& is, Derived& v)	{ return is >> v.x >> v.y; }
};

template<int N, class scalar, class Derived>
struct VecConstructor
{
	/* Component */

	inline constexpr scalar& component(int a) const { return *(reinterpret_cast<scalar*>(const_cast<VecConstructor*>(this)) + a); }

	/* Constructor */

	inline constexpr VecConstructor() {}
	template<typename... args> inline constexpr void construct(int a, scalar x_a, args... X) { return component(a) = x_a, construct(++a, X...);}
	inline constexpr void construct(int a) {}
	template<typename... args> inline constexpr VecConstructor(scalar x0, args... X) { component(0) = x0, construct(1, X...); }
	inline constexpr VecConstructor(const Vec<N, scalar, Derived>& rhs) { memcpy(this, &rhs, N*sizeof(scalar)); };
};

#endif
