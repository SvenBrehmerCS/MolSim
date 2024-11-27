#include "GravityCalculator.h"

#include <spdlog/spdlog.h>

#include "boundaries/InfContainer.h"

namespace physicsCalculator {
    GravityCalculator::GravityCalculator(const Environment& new_env)
        : Calculator(new_env) {
        // Initialize the forces
        calculateF();
    }

    GravityCalculator::GravityCalculator(
        const Environment& new_env, const std::vector<Particle>& particles, const bool init_forces, const Boundary type) {
        spdlog::warn("Called a GravityCalculator constructor which should only be used for testing.");
        env = new_env;

        switch (type) {
        case INF_CONT:
            container.reset(new InfContainer(particles));
            break;

        default:
            spdlog::critical("Tried to create a simulation with an illegal particle type: {}", static_cast<int>(type));
            break;
        }


        // Initialize the forces
        if (init_forces) {
            calculateF();
        }
    }

    GravityCalculator::~GravityCalculator() = default;

    double GravityCalculator::calculateFDist(const double dist) const {
        spdlog::error("Called calculate abs f on force calculator");
        return 0.0;
    }

    double GravityCalculator::calculateFAbs(const Particle& p1, const Particle& p2) { // Calculate the distance and force experienced by two particles
        const double distance = ArrayUtils::L2Norm(p2.getX() - p1.getX());
        return p1.getM() * p2.getM() / (distance * distance * distance);
    }

} // namespace physicsCalculator
