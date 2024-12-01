/**
 * @file
 *
 * @brief Handles the gravity based physics calculations
 */

#pragma once

#include "Calculator.h"
#include "Environment.h"
#include "boundaries/ParticleContainer.h"
#include "utils/ArrayUtils.h"

namespace physicsCalculator {

    /**
     * @class GravityCalculator
     *
     * @brief Class corresponding to a leap frog based gravity force calculator.
     */
    class GravityCalculator : public Calculator {

    public:
        /**
         * Initialize a simple calculator using a simulation environment.
         *
         * @param new_env The simulation environment that should be used for initialization.
         * @param new_cont The container storing the particles that should be used throughout the simulation.
         */
        GravityCalculator(const Environment& new_env, const std::shared_ptr<ParticleContainer>& new_cont);

        /**
         * Provide a constructor that allows the construction of a calculator using a particle container
         * and simulation environment. This class should be used for testing.
         *
         * @param new_env The new simulation environment.
         * @param particles The vector storing the particles that should be used throughout the simulation.
         * @param init_forces Define wether the forces should be initialized.
         */
        GravityCalculator(
            const Environment& new_env, const std::vector<Particle>& particles, const bool init_forces = true, const Boundary type = INF_CONT);

        /**
         * Define a constructor for a gravity calculator.
         */
        virtual ~GravityCalculator();

        /**
         * Get the force absolute and sign direction between two particles.
         */
        virtual double calculateFAbs(const Particle& p1, const Particle& p2);

        /**
         * Get the force absolute and sign direction between two particles.
         */
        virtual double calculateFDist(const double dist) const;
    };
} // namespace physicsCalculator