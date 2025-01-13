#include "Thermostat.h"

void Thermostat::regulate_Temperature() {

    double E_kin = 0.0;
#pragma omp parallel for schedule(auto) reduction(+ : E_kin)
    for (const auto& p : *particles) {
        E_kin += particles->get_type_descriptor(p.getType()).get_mass() * p.getV().len_squ();
    }

    double T_curr = E_kin / static_cast<double>(dimensions * particles->size());
    double diff = T_target - T_curr;

    if (particles->size() && T_curr == 0.0) {
        SPDLOG_CRITICAL("Congratulations, you broke physics! Temperature off non empty system should never be 0.");
        std::exit(EXIT_FAILURE);
    }

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

#pragma omp parallel for schedule(auto)
    for (auto& p : *particles) {
        p.setV(beta * p.getV());
    }
}
