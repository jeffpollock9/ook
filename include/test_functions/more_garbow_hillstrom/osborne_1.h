#ifndef OOK_TEST_FUNCTIONS_MORE_GARBOW_HILLSTROM_OSBORNE_1_H_
#define OOK_TEST_FUNCTIONS_MORE_GARBOW_HILLSTROM_OSBORNE_1_H_

#include <tuple>
#include <limits>
#include <vector>

namespace ook{
namespace test_functions{

template <typename Vector>
struct osborne_1
{
    typedef Vector vector_type;
    typedef typename vector_type::value_type real_type;

    std::tuple<real_type, vector_type>
    operator()(const vector_type& x) const
    {
        real_type f(1.0);
        vector_type df(5, 1.0);
        return std::make_pair(f, df);
    }

    static const int n = 5;
    static const int m = 33;
    static real_type f_min;
    static real_type tolerance;
    static std::vector<real_type> minima;
    static std::vector<real_type> x0;
};

template <typename Vector>
typename Vector::value_type
osborne_1<Vector>::f_min = 5.46489e-05;

template <typename Vector>
typename Vector::value_type
osborne_1<Vector>::tolerance = std::numeric_limits<typename Vector::value_type>::epsilon();

template <typename Vector>
std::vector<typename Vector::value_type>
osborne_1<Vector>::minima = {};

template <typename Vector>
std::vector<typename Vector::value_type>
osborne_1<Vector>::x0 = {0.5, 1.5, -1, 0.01, 0.02};


} // ns test_functions
} // ns ook

#endif