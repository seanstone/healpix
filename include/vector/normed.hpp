#ifndef NORMED_HPP
#define NORMED_HPP

#include "vecN.hpp"

template <int N, class scalar, class Derived>
struct NormedVec : Vec<N, scalar, Derived>
{
	using Vec<N, scalar, Derived>::Vec;

	/* Dot product */

	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wpedantic"
	inline constexpr friend scalar dot(const NormedVec& lhs, const NormedVec& rhs, int a=N-1) { return lhs[a]*rhs[a] + (a ? dot(lhs, rhs, a-1) : 0); }
	#pragma GCC diagnostic pop
	inline constexpr friend scalar operator*(const NormedVec& lhs, const NormedVec& rhs) { return dot(lhs, rhs); }

	/* Norm */

	//inline constexpr scalar norm2() const { return (*this) * (*this); }
	//inline constexpr scalar norm() const { return sqrt(norm2()); }
	//inline constexpr friend scalar norm(const Derived& v) { return v.norm(); }

	/* Normalization */

	//inline constexpr Derived& normalize() { return *static_cast<Derived*>(&(norm() ? *this/=norm() : *this=0)); }
	//inline constexpr friend Derived normalize(Derived v) { return v.normalize(); }

	/* Metric conjugate */ // NOTE: default conjugate = self

	inline constexpr friend Derived operator~(const NormedVec& rhs) { return *static_cast<Derived*>(const_cast<NormedVec*>(&rhs)); }
};

template<int N, class scalar, class Derived>
struct NormedArray : NormedVec<N, scalar, Derived>
{ scalar X[N]; };

/*template <int dim, class scalar> class Metric
{
    private:
        scalar g[dim][dim];

    public:

        // Constructors
        Metric(int dim_pos, int dim_neg) {for(int i=0; i<dim; i++) for(int j=0; j<dim; j++) g[i][j] = i==j ?  (i<dim_pos)*2-1 : 0; }
        Metric(list<scalar> g0) { auto g_ij = g0.begin(); for(int i=0; i<dim; i++) for(int j=0; j<dim; j++) g[i][j] = i*dim+j<g0.size() ?  *g_ij++ : 0; }
        Metric(scalar g0[dim][dim]) { for(int i=0; i<dim; i++) for(int j=0; j<dim; j++) g[i][j] = g0[i][j]; }

        // cout
        friend std::ostream& operator<< (std::ostream& out, const Metric& M)
        {
            for(int i=0; i<dim; i++)
            {
                for(int j=0; j<dim; j++) out << M.g[i][j] << "\t";
                out << "\n";
            }
            return out;
        }
};

typedef Metric<4, double> metric4;*/

#endif
