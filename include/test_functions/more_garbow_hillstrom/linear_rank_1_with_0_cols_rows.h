#ifndef OOK_TEST_FUNCTIONS_MORE_GARBOW_HILLSTROM_LINEAR_RANK_1_WITH_0_COLS_ROWS_H_
#define OOK_TEST_FUNCTIONS_MORE_GARBOW_HILLSTROM_LINEAR_RANK_1_WITH_0_COLS_ROWS_H_

#include <tuple>
#include <limits>
#include <vector>

namespace ook{
namespace test_functions{

template <typename Vector>
struct linear_rank_1_with_0_cols_rows
{
    typedef Vector vector_type;
    typedef typename vector_type::value_type real_type;

    std::tuple<real_type, vector_type>
    operator()(const vector_type& x) const
    {
        real_type f(1.0);
        vector_type df(0, 1.0);
        return std::make_pair(f, df);
    }

    static const int n = 0;
    static const int m = 0;
    static real_type f_min;
    static real_type tolerance;
    static std::vector<real_type> minima;
    static std::vector<real_type> x0;
};

template <typename Vector>
typename Vector::value_type
linear_rank_1_with_0_cols_rows<Vector>::f_min = 0.0;

template <typename Vector>
typename Vector::value_type
linear_rank_1_with_0_cols_rows<Vector>::tolerance = std::numeric_limits<typename Vector::value_type>::epsilon();

template <typename Vector>
std::vector<typename Vector::value_type>
linear_rank_1_with_0_cols_rows<Vector>::minima = {};

template <typename Vector>
std::vector<typename Vector::value_type>
linear_rank_1_with_0_cols_rows<Vector>::x0 = {};


} // ns test_functions
} // ns ook

#endif
