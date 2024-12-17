#include "LJCalculator.h"

#include <spdlog/spdlog.h>

#include "container/BoxContainer.h"
#include "container/DSContainer.h"

namespace physicsCalculator {
    LJCalculator::LJCalculator(const Environment& new_env, const std::shared_ptr<ParticleContainer>& new_cont)
        : Calculator { new_env, new_cont } {
        // Initialize the forces
        calculateF();
    }

    LJCalculator::LJCalculator(const Environment& new_env, const std::vector<Particle>& particles, const std::vector<TypeDesc>& new_desc,
        const bool init_forces, const bool is_infinite) {
        SPDLOG_WARN("Called a LJCalculator constructor which should only be used for testing.");
        env = new_env;

        if (is_infinite) {
            cont = std::make_shared<DSContainer>(particles, env.get_domain_size(), new_desc);
        } else {
            cont = std::make_shared<BoxContainer>(particles, env.get_r_cutoff(), env.get_domain_size(), new_desc);
        }

        // Initialize the forces
        if (init_forces) {
            calculateF();
        }
    }

    LJCalculator::~LJCalculator() = default;

    double LJCalculator::calculateFAbs(const Particle& p1, const Particle& p2, const double dist) {
        // Calculate the distance and force experienced by two particles
        return calculateFDist(dist, p1.getType(), p2.getType());
    }

    double LJCalculator::calculateFDist(const double dist, const int t1, const int t2) const {
        // Calculate the powers of (sigma / distance)
        const double term_to_2 = (cont->get_type_pair_descriptor(t1, t2).get_sigma_squared() / (dist * dist));
        const double term_to_6 = term_to_2 * term_to_2 * term_to_2;

        return (cont->get_type_pair_descriptor(t1, t2).get_scaled_epsilon() / (dist * dist)) * std::fma(-2.0 * term_to_6, term_to_6, term_to_6);
    }
} // namespace physicsCalculator
