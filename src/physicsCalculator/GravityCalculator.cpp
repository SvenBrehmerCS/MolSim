#include "GravityCalculator.h"

#include <spdlog/spdlog.h>

namespace physicsCalculator {
    GravityCalculator::GravityCalculator(const Environment& new_env)
        : Calculator(new_env) {
        // Initialize the forces
        calculateF();
    }

    GravityCalculator::GravityCalculator(const Environment& new_env, const std::vector<Particle>& particles, const bool init_forces) {
        spdlog::warn("Called a GravityCalculator constructor wich should only be used for testing.");
        env = new_env;
        container = ParticleContainer(particles);

        // Initialize the forces
        if (init_forces) {
            calculateF();
        }
    }

    GravityCalculator::~GravityCalculator() = default;

    void GravityCalculator::calculateF() {
        for (auto i = container.begin(); i < container.end(); i++) {
            for (auto j = i + 1; j < container.end(); j++) {
                // Calculate the distance and force experienced by two particles
                const double distance = ArrayUtils::L2Norm(j->getX() - i->getX());
                const double force = i->getM() * j->getM() / (distance * distance * distance);

                // Update the forces for both particles
                i->setF(force * (j->getX() - i->getX()) + i->getF());
                j->setF(force * (i->getX() - j->getX()) + j->getF());
            }
        }

        spdlog::debug("Calculated the new force.");
    }
} // namespace physicsCalculator
