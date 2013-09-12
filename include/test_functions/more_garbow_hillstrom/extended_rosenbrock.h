#ifndef OOK_TEST_FUNCTIONS_MORE_GARBOW_HILLSTROM_EXTENDED_ROSENBROCK_H_
#define OOK_TEST_FUNCTIONS_MORE_GARBOW_HILLSTROM_EXTENDED_ROSENBROCK_H_

#include <tuple>
#include <limits>
#include <vector>

namespace ook{
namespace test_functions{

template <typename Vector>
struct extended_rosenbrock
{
    typedef Vector vector_type;
    typedef typename vector_type::value_type real_type;

    std::tuple<real_type, vector_type>
    operator()(const vector_type& x) const
    {
        real_type f(1.0);
        vector_type df(10, 1.0);
        return std::make_pair(f, df);
    }

    static const int n = 10;
    static const int m = 10;
    static real_type f_min;
    static real_type tolerance;
    static std::vector<real_type> minima;
    static std::vector<real_type> x0;
};

template <typename Vector>
typename Vector::value_type
extended_rosenbrock<Vector>::f_min = 0.0;

template <typename Vector>
typename Vector::value_type
extended_rosenbrock<Vector>::tolerance = std::numeric_limits<typename Vector::value_type>::epsilon();

template <typename Vector>
std::vector<typename Vector::value_type>
extended_rosenbrock<Vector>::minima = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, };

template <typename Vector>
std::vector<typename Vector::value_type>
extended_rosenbrock<Vector>::x0 = {-1.2, 1.0, -1.2, 1.0, -1.2, 1.0, -1.2, 1.0, -1.2, 1.0, };


} // ns test_functions
} // ns ook

#endif
