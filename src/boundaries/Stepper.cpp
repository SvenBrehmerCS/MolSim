#include "Stepper.h"

Stepper::Stepper(const std::array<Boundary*, 6>& boundaries, const bool is_inf) {
    bc = boundaries;
    inf = is_inf;
}

void Stepper::step(physicsCalculator::Calculator& calc) {
    calc.calculateX();

    for (Particle& p : calc.get_container()) {
        for (size_t i = 0; i < bc.size(); i++) {
            bc[i]->postX(p);
        }
    }

    if (!inf) {
        calc.get_container().remove_particles_out_of_domain();
    }

    calc.get_container().update_positions();

    calc.calculateOldF();
    calc.calculateF();

    for (Particle& p : calc.get_container()) {
        for (size_t i = 0; i < bc.size(); i++) {
            bc[i]->postF(p, calc);
        }
    }

    calc.calculateV();
}
