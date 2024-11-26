#include "LJCalculator.h"

#include <spdlog/spdlog.h>

namespace physicsCalculator {
    LJCalculator::LJCalculator(const Environment& new_env)
        : Calculator(new_env) {
        // Initialize the forces
        calculateF();
    }

    LJCalculator::LJCalculator(const Environment& new_env, const std::vector<Particle>& particles, const bool init_forces) {
        spdlog::warn("Called a LJCalculator constructor which should only be used for testing.");
        env = new_env;
        container = ParticleContainer(particles);

        // Initialize the forces
        if (init_forces) {
            calculateF();
        }
    }

    LJCalculator::~LJCalculator() = default;

    void LJCalculator::calculateF() {
        for (auto i = container.begin(); i < container.end(); i++) {
            for (auto j = i + 1; j < container.end(); j++) {
                // Calculate the distance and force experienced by two particles
                const double distance = ArrayUtils::L2Norm(i->getX() - j->getX());
                // Calculate the powers of (sigma / distance)
                const double term = (env.get_sigma() / distance);
                const double term_to_2 = term * term;
                const double term_to_6 = term_to_2 * term_to_2 * term_to_2;
                const double force = (-24.0 * env.get_epsilon() / (distance * distance)) * (term_to_6 - 2.0 * term_to_6 * term_to_6);

                // Update the forces for both particles
                i->setF(force * (i->getX() - j->getX()) + i->getF());
                j->setF(force * (j->getX() - i->getX()) + j->getF());
            }
        }

        spdlog::debug("Calculated the new force.");
    }
} // namespace physicsCalculator
