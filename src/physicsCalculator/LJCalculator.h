/**
 * @file
 *
 * @brief Handles lenard jones based physics calculations
 */

#pragma once

#include "Calculator.h"
#include "Environment.h"
#include "container/ParticleContainer.h"
#include "utils/ArrayUtils.h"

namespace physicsCalculator {

    /**
     * @class LJCalculator
     *
     * @brief Class corresponding to a leap frog integrator using Lenard Jones potentials.
     */
    class LJCalculator : public Calculator {

    public:
        /**
         * Initialize a Lenard Jones calculator using a simulation environment.
         *
         * @param new_env The simulation environment that should be used for initialization.
         * @param new_cont The container storing the particles that should be used throughout the simulation.
         */
        LJCalculator(const Environment& new_env, const std::shared_ptr<ParticleContainer>& new_cont);

        /**
         * Provide a constructor that allows the construction of a calculator using a particle container
         * and simulation environment. This class should be used for testing.
         *
         * @param new_env The new simulation environment.
         * @param particles The vector storing the particles that should be used throughout the simulation.
         * @param init_forces Define wether the forces should be initialized.
         * @param is_infinite A boolean indicating if the simulation ahs an infinite domain size.
         */
        LJCalculator(
            const Environment& new_env, const std::vector<Particle>& particles, const bool init_forces = true, const bool is_infinite = true);

        /**
         * Define a constructor for a Lenard Jones calculator.
         */
        virtual ~LJCalculator();

        /**
         * Get the force absolute and sign direction between two particles.
         *
         * @param p1 The first particle.
         * @param p2 The second particle.
         * @param dist The distance between the particles.
         */
        virtual double calculateFAbs(const Particle& p1, const Particle& p2, const double dist);

        /**
         * Get the force absolute and sign direction between two particles.
         *
         * @param dist The distance between two particles.
         * @param t1 The type of the first particle.
         * @param t2 The type of the second particle.
         */
        virtual double calculateFDist(const double dist, const int t1, const int t2) const;
    };
} // namespace physicsCalculator