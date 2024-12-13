#include "TypeDesc.h"

TypeDesc::TypeDesc(const double m1, const double s1, const double e1, const double m2, const double s2, const double e2) {
    scaled_epsilon = -24.0 * std::sqrt(e1 * e2);
    const double sigma = (s1 + s2) * 0.5;
    sigma_squared = sigma;

    mass = m1 * m2;
}
