#include "GravityCalculator.h"

#include <spdlog/spdlog.h>

namespace physicsCalculator {
    GravityCalculator::GravityCalculator(const Environment& new_env)
        : Calculator(new_env) {
        // Initialize the forces
        calculateF();
    }

    GravityCalculator::GravityCalculator(const Environment& new_env, const std::vector<Particle>& particles, const bool init_forces) {
        spdlog::warn("Called a GravityCalculator constructor which should only be used for testing.");
        env = new_env;
        container = ParticleContainer(particles);

        // Initialize the forces
        if (init_forces) {
            calculateF();
        }
    }

    GravityCalculator::~GravityCalculator() = default;

    double GravityCalculator::calculateFAbs(const Particle& p1, const Particle& p2) { // Calculate the distance and force experienced by two particles
        const double distance = ArrayUtils::L2Norm(p2.getX() - p1.getX());
        return p1.getM() * p2.getM() / (distance * distance * distance);
    }

} // namespace physicsCalculator
