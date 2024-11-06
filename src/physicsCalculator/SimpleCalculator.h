/**
 * @file
 *
 * @brief Handles the basic physics calculations
 */

#pragma once

#include "Calculator.h"
#include "../Environment.h"
#include "../ParticleContainer.h"
#include "../utils/ArrayUtils.h"

namespace physicsCalculator {

    /**
     * @class SimpleCalculator
     *
     * @brief Class corresponding to a simple position, velocity and force calculator
     */
    class SimpleCalculator : public Calculator {

    public:
        /**
         * Initialize a simple calculator using a simulation environmetn.
         * 
         * @param new_env THe simulation environment that should be used for initialization.
         */
        SimpleCalculator(const Environment& new_env);

        /**
         * Provide a constructor that allows the construction of a calculator using a particle container
         * and simulation environment. This class should be used for testing.
         * 
         * @param new_env The new simulation environment.
         * @param particles The vector storing the particles that should be used throughout the simulation.
         */
        SimpleCalculator(const Environment& new_env, const std::vector<Particle>& particles);

        /**
         * Define a constructor for a simple calculator.
         */
        virtual ~SimpleCalculator();

        /**
         * Update the forces and old forces experienced by all the particles based on the basic calculation scheme.
         *
         * @param container The container for the particles.
         * @param env The simulation environment.
         */
        virtual void calculateF();

        /**
         * Update the position of all the particles based on the basic calculation scheme.
         *
         * @param container The container for the particles.
         * @param env The simulation environment.
         */
        virtual void calculateX();

        /**
         * Update the velocity of all the particles based on the basic calculation scheme.
         *
         * @param container The container for the particles.
         * @param env The simulation environment.
         */
        virtual void calculateV();
    };
} // namespace physicsCalculator