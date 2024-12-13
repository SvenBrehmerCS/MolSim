#include "TypeDesc.h"

TypeDesc::TypeDesc(const double m, const double s, const double e, const double delta_t, const double g) {
    mass = m;
    sigma = s;
    epsilon = e;
    dt_m = delta_t * 0.5 / m;
    dt_dt_m = delta_t * dt_m;
    G = { 0.0, m * -g, 0.0 };
}
