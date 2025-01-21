/**
 * @file
 *
 * @brief Pick some particles and apply a constant force to them.
 */

#pragma once

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
        double end;

    public:
        /**
         * Create a tweezers object.
         *
         * @param i The indices of the particles that should be tweezered.
         * @param f The force that should be applied to the particles.
         * @param e THe end of the tweezers application.
         */
        Tweezers(const std::vector<size_t>& i, const Vec<double>& f, const double e);

        /**
         * Destroy a tweezers object.
         */
        ~Tweezers() = default;

        /**
         * Apply the tweezers to the particles.
         *
         * @param particles The particles that should be tweezered.
         */
        void apply(ParticleContainer& particles);

        /**
         * Get the end time of the application of the tweezers.
         *
         * @return The end time of the application of the tweezers.
         */
        inline double get_end() const { return end; }
    };

} // namespace physicsCalculator
