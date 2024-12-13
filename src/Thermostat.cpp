#include "Thermostat.h"

Thermostat::Thermostat(int dimensions, double T_target, double max_change, std::shared_ptr<ParticleContainer> particles)
    : dimensions { dimensions }
    , T_target { T_target }
    , max_change { max_change }
    , particles { particles } { }

void Thermostat::regulate_Temperature() { // TODO Overflow checks?

    double E_kin = 0.0;
    for (const auto& p : *particles) {
        E_kin += (p.getM() * ArrayUtils::L2Norm(p.getV()));
    }

    double T_curr = E_kin / (dimensions * particles->size());
    // TODO T_curr == 0.0, INF(no particles)
    double diff = T_target - T_curr;

    double beta;
    if (std::abs(diff) <= max_change) {
        beta = std::sqrt(T_target / T_curr);
        if (isnan(beta)) {
            SPDLOG_CRITICAL("Temperature scaling factor should never be NaN.");
            std::exit(EXIT_FAILURE);
        }
    } else if (diff < 0.0) {
        beta = std::sqrt((T_curr - max_change) / T_curr);
    } else {
        beta = std::sqrt((T_curr + max_change) / T_curr);
    }

    for (auto& p : *particles) { // TODO Change from vector multiplication to scaling.
        p.setV(p.getV() * std::array<double, 3> { beta, beta, beta });
    }
}
