#include "LJCalculator.h"

#include <spdlog/spdlog.h>

namespace physicsCalculator {
    LJCalculator::LJCalculator(const Environment& new_env)
        : Calculator(new_env) {
        // Initialize the forces
        calculateF();
    }

    LJCalculator::LJCalculator(const Environment& new_env, const std::vector<Particle>& particles, const bool init_forces) {
        spdlog::warn("Called a LJCalculator constructor wich should only be used for testing.");
        env = new_env;
        container = ParticleContainer(particles);

        // Initialize the forces
        if (init_forces) {
            calculateF();
        }
    }

    LJCalculator::~LJCalculator() = default;

    void LJCalculator::calculateF() {
        for (size_t i = 0; i < container.get_particles().size(); i++) {
            for (size_t j = i + 1; j < container.get_particles().size(); j++) {
                // Calculate the distance and force experienced by two particles
                const double distance = ArrayUtils::L2Norm(container.get_particles()[j].getX() - container.get_particles()[i].getX());
                // Calculate the powers of (sigma / distance)
                const double term = (env.get_sigma() / distance);
                const double term_to_2 = term * term;
                const double term_to_6 = term_to_2 * term_to_2 * term_to_2;
                const double force = -24.0 * env.get_epsilon() / (distance * distance * distance) * (term_to_6 * term_to_6 - term_to_6);

                // Update the forces for both particles
                container.get_particles()[i].setF(
                    force * (container.get_particles()[j].getX() - container.get_particles()[i].getX()) + container.get_particles()[i].getF());
                container.get_particles()[j].setF(
                    force * (container.get_particles()[i].getX() - container.get_particles()[j].getX()) + container.get_particles()[j].getF());
            }
        }

        spdlog::debug("Calculated the new force.");
    }
} // namespace physicsCalculator
