/**
 * @file
 *
 * @brief Handles the gravity based physics calculations
 */

#pragma once

#include "Environment.h"
#include "ParticleContainer.h"
#include "utils/ArrayUtils.h"
#include "Calculator.h"

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
         * @param new_env THe simulation environment that should be used for initialization.
         */
        GravityCalculator(const Environment& new_env);

        /**
         * Provide a constructor that allows the construction of a calculator using a particle container
         * and simulation environment. This class should be used for testing.
         *
         * @param new_env The new simulation environment.
         * @param particles The vector storing the particles that should be used throughout the simulation.
         * @param init_forces Define wether the forces should be initialized.
         */
        GravityCalculator(const Environment& new_env, const std::vector<Particle>& particles, const bool init_forces = true);

        /**
         * Define a constructor for a gravity calculator.
         */
        virtual ~GravityCalculator();

        /**
         * Get the force absolute and sign direction between two particles.
         */
        virtual double calculateFAbs(const Particle& p1, const Particle& p2);
    };
} // namespace physicsCalculator