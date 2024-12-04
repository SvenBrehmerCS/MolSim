#include "LJCalculator.h"

#include <spdlog/spdlog.h>

#include "boundaries/BoxContainer.h"
#include "boundaries/InfContainer.h"

namespace physicsCalculator {
    LJCalculator::LJCalculator(const Environment& new_env, const std::shared_ptr<ParticleContainer>& new_cont)
        : Calculator(new_env, new_cont) {
        // Initialize the forces
        calculateF();
    }

    LJCalculator::LJCalculator(const Environment& new_env, const std::vector<Particle>& particles, const bool init_forces, const bool is_infinite) {
        spdlog::warn("Called a LJCalculator constructor which should only be used for testing.");
        env = new_env;

        if (is_infinite) {
            cont.reset(new InfContainer(particles, env.get_domain_size()));
        } else {
            cont.reset(new BoxContainer(particles, env.get_r_cutoff(), env.get_domain_size()));
        }

        // Initialize the forces
        if (init_forces) {
            calculateF();
        }
    }

    LJCalculator::~LJCalculator() = default;

    double LJCalculator::calculateFAbs(const Particle& p1, const Particle& p2) {
        // Calculate the distance and force experienced by two particles
        const double distance = ArrayUtils::L2Norm(p1.getX() - p2.getX());

        return calculateFDist(distance);
    }

    double LJCalculator::calculateFDist(const double dist) const {
        // Calculate the powers of (sigma / distance)
        const double term = (env.get_sigma() / dist);
        const double term_to_2 = term * term;
        const double term_to_6 = term_to_2 * term_to_2 * term_to_2;

        return (24.0 * env.get_epsilon() / (dist * dist)) * (term_to_6 - 2.0 * term_to_6 * term_to_6);
    }
} // namespace physicsCalculator
