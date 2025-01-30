#include "LJSmoothCalculator.h"

#include <spdlog/spdlog.h>

#include "container/BoxContainer.h"
#include "container/DSContainer.h"

namespace physicsCalculator {
    LJSmoothCalculator::LJSmoothCalculator(const Environment& new_env, const std::shared_ptr<ParticleContainer>& new_cont, const double new_rl)
        : Calculator { new_env, new_cont }
        , r_l_squ { new_rl * new_rl }
        , r_l { new_rl }
        , r_c_squ { new_env.get_r_cutoff() * new_env.get_r_cutoff() } {
        // Initialize the forces
        calculateF();
    }

    LJSmoothCalculator::LJSmoothCalculator(const Environment& new_env, const std::vector<Particle>& particles, const double new_rl,
        const std::vector<TypeDesc>& new_desc, const bool init_forces, const bool is_infinite)
        : r_l_squ { new_rl * new_rl }
        , r_l { new_rl }
        , r_c_squ { new_env.get_r_cutoff() * new_env.get_r_cutoff() } {
        SPDLOG_WARN("Called a LJSmoothCalculator constructor which should only be used for testing.");
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

    LJSmoothCalculator::~LJSmoothCalculator() = default;

    double LJSmoothCalculator::calculateFAbs(const Particle& p1, const Particle& p2, const double dist_squ) {
        // Calculate the distance and force experienced by two particles
        return calculateFDist(dist_squ, p1.getType(), p2.getType());
    }

    double LJSmoothCalculator::calculateFDist(const double dist_squ, const int t1, const int t2) const {

        if (dist_squ <= r_l_squ) {
            std::cout << "dist is less than r_l\n";
            // Calculate the powers of (sigma / distance)
            const double term_to_2 = (cont->get_type_pair_descriptor(t1, t2).get_sigma_squared() / (dist_squ));
            const double term_to_6 = term_to_2 * term_to_2 * term_to_2;

            return (cont->get_type_pair_descriptor(t1, t2).get_scaled_epsilon() / (dist_squ)) * std::fma(-2.0 * term_to_6, term_to_6, term_to_6);
        } else if (dist_squ >= r_c_squ) {
            std::cout << "dist is greater than r_c<\n";
            return 0.0;
        } else {
            std::cout << "dist is between r_l and r_c\n";

            const double sigma_to_6 = cont->get_type_pair_descriptor(t1, t2).get_sigma_squared()
                * cont->get_type_pair_descriptor(t1, t2).get_sigma_squared() * cont->get_type_pair_descriptor(t1, t2).get_sigma_squared();
            const double dist = std::sqrt(dist_squ);
            const double dist_to_4 = dist_squ * dist_squ;
            const double dist_to_6 = dist_to_4 * dist_squ;
            const double dist_to_7 = dist_to_6 * dist;
            const double dist_to_14 = dist_to_7 * dist_to_7;
            const double rc_l = env.get_r_cutoff() - r_l;

            std::cout << "dist: " << dist << "\n";
            std::cout << "dist_to_4: " << dist_to_4 << "\n";
            std::cout << "dist_to_6: " << dist_to_6 << "\n";
            std::cout << "dist_to_7: " << dist_to_7 << "\n";
            std::cout << "dist_to_14: " << dist_to_14 << "\n";
            std::cout << "r_c: " << env.get_r_cutoff() << "\n";
            std::cout << "r_l: " << r_l << "\n";
            std::cout << "rc_l: " << rc_l << "\n";
            std::cout << "sigma_to_6: " << sigma_to_6 << "\n";
            std::cout << "r_c_squ: " << r_c_squ << "\n";
            std::cout << (cont->get_type_pair_descriptor(t1, t2).get_scaled_epsilon() * sigma_to_6 / (dist_to_14 * rc_l * rc_l * rc_l)
                * (env.get_r_cutoff() - dist)) << "\n";
            std::cout << ((r_c_squ * (2.0 * sigma_to_6 - dist_to_6)) + env.get_r_cutoff() * (3.0 * r_l - dist) * (dist_to_6 - 2.0 * sigma_to_6)
                + dist * (5.0 * r_l * sigma_to_6 - 2.0 * r_l * dist_to_6 - 3.0 * sigma_to_6 * dist + dist_to_7)) << "\n\n";

            return cont->get_type_pair_descriptor(t1, t2).get_scaled_epsilon() * sigma_to_6 / (dist_to_14 * rc_l * rc_l * rc_l)
                * (env.get_r_cutoff() - dist)
                * ((r_c_squ * (2.0 * sigma_to_6 - dist_to_6)) + env.get_r_cutoff() * (3.0 * r_l - dist) * (dist_to_6 - 2.0 * sigma_to_6)
                    + dist * (5.0 * r_l * sigma_to_6 - 2.0 * r_l * dist_to_6 - 3.0 * sigma_to_6 * dist + dist_to_7));
        }
    }
} // namespace physicsCalculator
