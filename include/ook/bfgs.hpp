
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

#ifndef OOK_LINE_SEARCH_METHODS_BFGS_HPP_
#define OOK_LINE_SEARCH_METHODS_BFGS_HPP_

#include <tuple>

#include "linalg.hpp"
#include "ook/line_search/mcsrch.hpp"
#include "ook/line_search_method.hpp"

namespace ook{
/// \brief Implementation of the required steps of line_search_method
/// for BFGS method.
template <typename X>
struct bfgs_impl
{
    typedef X vector_type;
    typedef typename std::remove_reference<decltype(X()[0])>::type value_type;
    typedef typename linalg::associated_matrix<X>::type matrix_type;

    struct state
    {
        typedef X vector_type;
        typedef typename std::remove_reference<decltype(X()[0])>::type value_type;
        typedef typename linalg::associated_matrix<X>::type matrix_type;

        matrix_type H;
    };

    template <typename State>
    bfgs_impl(const State& s)
    :
        dfx(s.dfx),
        p(s.dfx.size()),
        H(s.dfx.size(), s.dfx.size(), 0.0)
    {
        const int n = dfx.size();
        for (int i = 0; i < n; ++i)
        {
            H(i, i) = 1.0;
        }
    }

    template <typename State>
    vector_type
    descent_direction(const State& s)
    {
        linalg::gemv(-1.0, H, s.dfx, 0.0, p);
        return p;
    }

    template <typename State>
    void
    update(const State& s)
    {
        X yk(s.dfx - dfx);
        const value_type rho = 1.0/linalg::inner_prod(yk, s.dx);
        const int n = linalg::size(s.dfx);
        matrix_type Z(- rho * linalg::outer_prod(s.dx, yk));
        for (int i = 0; i < n; ++i) Z(i, i) += 1.0;

        matrix_type tmp(n, n);
        linalg::gemm(1.0, H, linalg::trans(Z), 0.0, tmp);
        linalg::gemm(1.0, Z, tmp, 0.0, H);
        matrix_type ss = rho * linalg::outer_prod(s.dx, s.dx);
        H += ss;
        dfx = s.dfx;
    }

private:
    vector_type dfx;
    vector_type p;
    matrix_type H;
};

/// \brief The Broyden-Fletcher-Goldfarb-Shanno (BFGS) algorithm.
template <typename F, typename X, typename Options, typename Observer>
typename line_search_method<bfgs_impl<X>, line_search::mcsrch>::state_type
bfgs(F f, const X& x0, const Options& opts, Observer& observer)
{
    typedef bfgs_impl<X> scheme;
    line_search_method<scheme, line_search::mcsrch> method;
    return method(f, x0, opts, observer);
}

} //ns ook

#endif
