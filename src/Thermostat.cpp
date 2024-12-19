#include "Thermostat.h"

void Thermostat::regulate_Temperature() {

    double E_kin = 0.0;
    for (const auto& p : *particles) {
        E_kin += particles->get_type_descriptor(p.getType()).get_mass() * ArrayUtils::L2Norm(p.getV()) * ArrayUtils::L2Norm(p.getV());
    }

    double T_curr = E_kin / static_cast<double>(dimensions * particles->size());
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
