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

#ifndef OOK_LINE_SEARCH_METHODS_STEEPEST_DESCENT_HPP_
#define OOK_LINE_SEARCH_METHODS_STEEPEST_DESCENT_HPP_

#include <tuple>
#include <type_traits>

#include "linalg/operations.hpp"

#include "ook/line_search_method.hpp"

namespace ook{
/// \brief Implementation for the steepest descent method.
struct steepest_descent_impl
{
    /// \brief Constructor required by scheme concept.
    template <typename T>
    steepest_descent_impl(const T&){}

    /// \brief The descent direction in the steepest descent algorithm
    /// is the negative gradient, \f$- \nabla f(x) \f$.
    /// \tparam State Optimiser state type.
    /// \param s The current state.
    /// \return The negative of the gradient vector.
    template <typename State>
    auto
    descent_direction(const State& s)
    {
        return -s.dfx;
    }

    /// \brief Update, does nothing in this case. Required by
    /// scheme concept.
    template <typename T>
    void
    update(const T& s){}
};

/// \brief The Steepest descent algorithm. This is just a convenience function
/// which forwards the call to the generic function line_search_method
template <typename F, typename X, typename Options, typename Observer>
std::tuple<ook::message, X>
steepest_descent(F f, const X& x0, const Options& opts, Observer& observer)
{
    typedef steepest_descent_impl scheme;
    line_search_method<scheme> method;
    return method(f, x0, opts, observer);
}

} //ns ook

#endif
