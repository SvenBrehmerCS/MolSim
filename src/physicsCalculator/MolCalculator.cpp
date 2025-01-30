#include "MolCalculator.h"

#include <spdlog/spdlog.h>

#include "container/BoxContainer.h"
#include "container/DSContainer.h"

namespace physicsCalculator {
    MolCalculator::MolCalculator(const Environment& new_env, const std::shared_ptr<ParticleContainer>& new_cont)
        : Calculator { new_env, new_cont } {
        // Initialize the forces
        calculateF();
    }


    MolCalculator::MolCalculator(const Environment& new_env, const std::vector<Particle>& particles, const std::vector<TypeDesc>& new_desc,
        const bool init_forces, const bool is_infinite) {
        SPDLOG_WARN("Called a MolCalculator constructor which should only be used for testing.");
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

    MolCalculator::~MolCalculator() = default;

    double MolCalculator::calculateFAbs(const Particle& p1, const Particle& p2, const double dist_squ) {
        // Calculate the distance and force experienced by two particles
        if (p1.isMolecule() && (p1.getType() == p2.getType())) {
            if (p1.isDirectNeighbour(p2.getIndex())) {
                std::cout << "direct" << std::endl;
                const double dist = std::sqrt(dist_squ);
                return cont->get_type_descriptor(p1.getType()).get_k() * (dist - cont->get_type_descriptor(p1.getType()).get_r0()) / dist;
            }

            if (p1.isCornerNeighbour(p2.getIndex())) {
                std::cout << "corner" << std::endl;
                const double dist = std::sqrt(dist_squ);
                return cont->get_type_descriptor(p1.getType()).get_k() * (dist - cont->get_type_descriptor(p1.getType()).get_r1()) / dist;
            }

            if (dist_squ <= cont->get_type_descriptor(p1.getType()).get_cut()) {
                std::cout << "repulsion" << std::endl;
                return calculateFDist(dist_squ, p1.getType(), p2.getType());
            }

            std::cout << "ignored" << std::endl;
            return 0.0;

        } else {
            return calculateFDist(dist_squ, p1.getType(), p2.getType());
        }
    }

    double MolCalculator::calculateFDist(const double dist_squ, const int t1, const int t2) const {
        // Calculate the powers of (sigma / distance)
        const double term_to_2 = (cont->get_type_pair_descriptor(t1, t2).get_sigma_squared() / (dist_squ));
        const double term_to_6 = term_to_2 * term_to_2 * term_to_2;

        return (cont->get_type_pair_descriptor(t1, t2).get_scaled_epsilon() / (dist_squ)) * std::fma(-2.0 * term_to_6, term_to_6, term_to_6);
    }
} // namespace physicsCalculator
