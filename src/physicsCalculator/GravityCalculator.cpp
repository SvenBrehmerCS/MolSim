#include "GravityCalculator.h"

#include <spdlog/spdlog.h>

#include "container/DSContainer.h"

namespace physicsCalculator {
    GravityCalculator::GravityCalculator(const Environment& new_env, const std::shared_ptr<ParticleContainer>& new_cont)
        : Calculator(new_env, new_cont) {
        // Initialize the forces
        calculateF();
    }

    GravityCalculator::GravityCalculator(const Environment& new_env, const std::vector<Particle>& particles, const std::vector<TypeDesc>& new_desc,
        const bool init_forces, const BoundaryType type) {
        SPDLOG_WARN("Called a GravityCalculator constructor which should only be used for testing.");
        env = new_env;

        switch (type) {
        case INF_CONT:
            cont.reset(new DSContainer(particles, new_desc));
            break;

        default:
            SPDLOG_CRITICAL("Tried to create a simulation with an illegal particle type: {}", static_cast<int>(type));
            break;
        }


        // Initialize the forces
        if (init_forces) {
            calculateF();
        }
    }

    GravityCalculator::~GravityCalculator() = default;

    double GravityCalculator::calculateFDist(const double dist, const int t1, const int t2) const {
        return cont->get_type_pair_descriptor(t1, t2).get_mass() / (dist * dist * dist);
    }

    double GravityCalculator::calculateFAbs(const Particle& p1, const Particle& p2, const double dist) {
        // Calculate the distance and force experienced by two particles
        return calculateFDist(dist, p1.getType(), p2.getType());
    }

} // namespace physicsCalculator
