#include "GravityCalculator.h"

#include <spdlog/spdlog.h>

#include "container/InfContainer.h"

namespace physicsCalculator {
    GravityCalculator::GravityCalculator(const Environment& new_env, const std::shared_ptr<ParticleContainer>& new_cont)
        : Calculator(new_env, new_cont) {
        // Initialize the forces
        calculateF();
    }

    GravityCalculator::GravityCalculator(
        const Environment& new_env, const std::vector<Particle>& particles, const bool init_forces, const BoundaryType type) {
        spdlog::warn("Called a GravityCalculator constructor which should only be used for testing.");
        env = new_env;

        switch (type) {
        case INF_CONT:
            cont.reset(new InfContainer(particles));
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

    double GravityCalculator::calculateFDist(const double dist, const int t1, const int t2) const {
        // TODO:
        return 0.0;
    }

    double GravityCalculator::calculateFAbs(const Particle& p1, const Particle& p2, const double dist) {
        // Calculate the distance and force experienced by two particles
        return calculateFDist(p1.getType(), p2.getType(), dist);
    }

} // namespace physicsCalculator
