// Copyright 2013 Harry Hill
//
// This file is part of ook.
//
// ook is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// ook is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public License
// along with ook.  If not, see <http://www.gnu.org/licenses/>.

#ifndef OOK_FINITE_DIFFERENCES_CENTRAL_DIFFERENCE_HPP_
#define OOK_FINITE_DIFFERENCES_CENTRAL_DIFFERENCE_HPP_

#include <limits>
#include <cmath>
#include <tuple>

#include <algorithm>
#include "ook/finite_differences/detail/transform.hpp"

namespace ook{
namespace finite_differences{

/// Central difference approximation
struct central_difference{
	/// \brief Calculate the central difference approximation to the gradient of f.
	template <typename F, typename X>
	static
	auto
	gradient(F f, X x);

	/// \brief Calculate the central difference approximation to the hessian of f.
    template <typename M, typename F, typename X>
	static
	auto
	hessian(F f, const X& x);
};

template <typename F, typename X>
auto
central_difference::gradient(F f, X x)
{
	// Generate a set of sample points
	// (x + he_1, x + he_2, ..., x+he_n, x-he_1,..., x-he_n, x)
	const size_t n = std::distance(x.begin(), x.end());
	const double eps = std::numeric_limits<double>::epsilon();
	const double hmin(exp(log(eps) / 3.0));

	std::vector<X> sample_points(2 * n + 1);
	X h(n);

	for (size_t i = 0; i < n; ++i)
	{
		const double xi = x[i];
		const double hx = hmin * (1 + fabs(xi));
		h[i] = hx;

		x[i] = xi + hx;
		sample_points[i] = x;
		x[i] = xi - hx;
		sample_points[i+n] = x;

		x[i] = xi;
	}
	sample_points[2 * n] = x;
	// evaluate function at each point
	std::vector<double> function_values(sample_points.size());
	detail::transform(sample_points, function_values, f);

	// assemble
	X df(n);
	const double fx = function_values[2 * n];
	for (size_t i = 0; i < n; ++i)
    {
		df[i] = (function_values[i] - function_values[i + n]) / (2.0 * h[i]);
	}
	return std::make_tuple(fx, df);
}

/// Calculate the central difference approximation to the hessian of f.
template <typename M, typename F, typename X>
auto
central_difference::hessian(F f, const X& x)
{
	const double eps = std::numeric_limits<double>::epsilon();
	const double hmin = std::pow(eps, 1.0/3.0);

	const size_t n = std::distance(x.begin(), x.end());
	const double fx = f(x);

	X local_x(x);
	M H(n, n);
	for (size_t i = 0; i < n; ++i){
		// Work out diagonal term first

		const double hi = hmin * (1.0 + fabs(local_x[i]));
		const double xi = local_x[i];

		// f(x + h_i e_i)
		local_x[i] = xi + hi;
		const double fp = f(local_x);

		// f(x + 2 h_i e_i)
		local_x[i] = xi + 2.0 * hi;
		const double fpp = f(local_x);

		// f(x - h_i e_i)
		local_x[i] = xi - hi;
		const double fm = f(local_x);

		// f(x - 2 h_i e_i)
		local_x[i] = xi - 2.0 * hi;
		const double fmm = f(local_x);

		H(i, i) = (-fpp + 16.0 * fp - 30.0 * fx + 16.0 * fm - fmm)/(12.0 * hi * hi);

		local_x[i] = xi;

		for (size_t j = 0; j < i; ++j){

			const double xj = local_x[j];

			const double hj = hmin * (1.0 + fabs(local_x[j]));

			// f(x + h_i e_i + h_j e_j)
			local_x[i] = xi + hi;
			local_x[j] = xj + hj;
			const double fpp = f(local_x);

			// f(x - h_i e_i + h_j e_j)
			local_x[i] = xi - hi;
			local_x[j] = xj + hj;
			const double fmp = f(local_x);

			// f(x + h_i e_i - h_j e_j)
			local_x[i] = xi + hi;
			local_x[j] = xj - hj;
			const double fpm = f(local_x);

			// f(x - h_i e_i - h_j e_j)
			local_x[i] = xi - hi;
			local_x[j] = xj - hj;
			const double fmm = f(local_x);

			H(i, j) = H(j, i) = (fpp - fpm - fmp + fmm)/(4.0 * hi * hj);

			local_x[i] = xi;
			local_x[j] = xj;
		}
	}
	return std::make_tuple(fx, H);
}

} // ns finite_differences
} // ns ook

#endif
