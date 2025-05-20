#include "TypeDesc.h"

TypeDesc::TypeDesc(const double m, const double s, const double e, const double delta_t, const double g, const double k, const double r0) {
    mass = m;
    sigma = s;
    epsilon = e;
    dt_m = delta_t * 0.5 / m;
    dt_dt_m = delta_t * dt_m;
    G = { 0.0, m * g, 0.0 };
    stiff = k;
    rad = r0;
    rad_scale = r0 * 1.4142135623730950488016;
    c = (s * 1.1224620483093729814335330496791795162324111106139867534404095) * (s * 1.1224620483093729814335330496791795162324111106139867534404095);
}
