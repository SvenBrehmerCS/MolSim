#include "TypePairDesc.h"

TypePairDesc::TypePairDesc(const double m1, const double s1, const double e1, const double m2, const double s2, const double e2) {
    scaled_epsilon = 24.0 * std::sqrt(static_cast<long double>(e1 * e2));
    const double sigma = (s1 + s2) * 0.5;
    sigma_squared = sigma * sigma;

    mass = m1 * m2;
}
