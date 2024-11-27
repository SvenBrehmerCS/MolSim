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

    double LJCalculator::calculateFAbs(const Particle& p1, const Particle& p2) {
        // Calculate the distance and force experienced by two particles
        const double distance = ArrayUtils::L2Norm(p1.getX() - p2.getX());
        // Calculate the powers of (sigma / distance)
        const double term = (env.get_sigma() / distance);
        const double term_to_2 = term * term;
        const double term_to_6 = term_to_2 * term_to_2 * term_to_2;

        return (24.0 * env.get_epsilon() / (distance * distance)) * (term_to_6 - 2.0 * term_to_6 * term_to_6);
    }
} // namespace physicsCalculator
