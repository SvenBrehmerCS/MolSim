/**
 * @file
 *
 * @brief Defines a default interface for the physics caculators
 */

#pragma once

#include "../Environment.h"
#include "../ParticleContainer.h"

/**
 * @brief Collection of calculators for different levels of complexity
 */
namespace physicsCalculator {

    /**
     * @class Calculator
     *
     * @brief The default class for an instance of a calculator
     */
    class Calculator {
    protected:
        /**
         * Store the particles used throughout the simulation.
         */
        ParticleContainer container;

        /**
         * Store the simulation environment used throughout the simulation.
         */
        Environment env;

    public:
        /**
         * Construct a default calculator.
         */
        Calculator();

        /**
         * Construct a calculator using the provided calculator.
         *
         * @param new_env THe simulation environment that should be used.
         */
        Calculator(const Environment& new_env);

        /**
         * Define the default destructor for every calculator.
         */
        virtual ~Calculator() {};

        /**
         * Update the forces and old forces experienced by all the particles.
         */
        virtual void calculateF() = 0;

        /**
         * Update the position of all the particles.
         */
        virtual void calculateX() = 0;

        /**
         * Update the velocity of all the particles.
         */
        virtual void calculateV() = 0;

        /**
         * Perform a single simulation step, assuming that the leep frog method is being used.
         */
        void step();

        /**
         * Get a refference to the particle container.
         *
         * @return A refference to the particle container.
         */
        ParticleContainer& get_container();

        /**
         * Get a refference to the vector storing the particles.
         *
         * @return A refference to the particle vector.
         */
        std::vector<Particle>& get_particles();
    };
} // namespace physicsCalculator