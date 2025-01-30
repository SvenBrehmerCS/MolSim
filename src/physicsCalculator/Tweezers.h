/**
 * @file
 *
 * @brief Pick some particles and apply a constant force to them.
 */

#pragma once

#include <spdlog/spdlog.h>
#include <vector>

#include "container/ParticleContainer.h"

namespace physicsCalculator {
    /**
     * @class Tweezers
     *
     * @brief Class for applying a constant force to a set of particles.
     */
    class Tweezers {
    private:
        /**
         * The indices of the particles that should be tweezered.
         */
        std::vector<size_t> indices;

        /**
         * The force that should be applied to the particles.
         */
        Vec<double> force;

        /**
         * Store until when the tweezers should be applied.
         */
        double end = 0.0;

    public:
        /**
         * Create an empty Tweezers object.
         */
        Tweezers();

        /**
         * Create a tweezers object.
         *
         * @param i The indices of the particles that should be tweezed.
         * @param f The force that should be applied to the particles.
         * @param e The end of the tweezers application.
         */
        Tweezers(const std::vector<size_t>& i, const Vec<double>& f, const double e);

        /**
         * Destroy a tweezers object.
         */
        ~Tweezers() = default;

        /**
         * Apply the tweezers to the particles.
         *
         * @param particles The particles that should be tweezed.
         */
        void apply(ParticleContainer& particles);

        /**
         * Get the end time of the application of the tweezers.
         *
         * @return The end time of the application of the tweezers.
         */
        inline double get_end() const { return end; }

        /**
         * Set the particles effected by the tweezers.
         *
         * @param indices The indices of the effected particles.
         */
        inline void set_indices(const std::vector<size_t>& indices) { this->indices = indices; }

        /**
         * Set the force exerted tweezers.
         *
         * @param force The force effecting the particles.
         */
        inline void set_force(const Vec<double>& force) { this->force = force; }

        /**
         * Set the point till which the tweezers apply their force.
         *
         * @param end The end time.
         */
        inline void set_end(const double end) { this->end = end; }
    };

} // namespace physicsCalculator
