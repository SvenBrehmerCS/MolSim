/**
 * @file
 *
 * @brief Defines a default interface for the physics calculators.
 */

#pragma once

#include "Environment.h"
#include "boundaries/ParticleContainer.h"

#include <memory>

/**
 * @brief Collection of calculators for different levels of complexity.
 */
namespace physicsCalculator {

    /**
     * @class Calculator
     *
     * @brief The default class for an instance of a force calculator. It implements the leap frog method.
     */
    class Calculator {
    protected:
        /**
         * Store the simulation environment used throughout the simulation.
         */
        Environment env;

        /**
         * Store the particles used throughout the simulation.
         */
        std::shared_ptr<ParticleContainer> cont;

    public:
        /**
         * Construct a default calculator.
         */
        Calculator();

        /**
         * Construct a calculator using the provided calculator.
         *
         * @param new_env The simulation environment that should be used.
         * @param new_cont The container storing the particles that should be used throughout the simulation.
         */
        Calculator(const Environment& new_env, const std::shared_ptr<ParticleContainer>& new_cont);

        /**
         * Create a calculator from an environment and a particle vector. This method should only be used for debugging.
         *
         * @param new_env The simulation environment.
         * @param particles The particles used for testing.
         */
        Calculator(const Environment& new_env, const std::vector<Particle>& particles);

        /**
         * Define the default destructor for every calculator.
         */
        virtual ~Calculator() {};

        /**
         * Get the force absolute and sign direction between two particles.
         */
        virtual double calculateFDist(const double dist) const = 0;

        /**
         * Get the force absolute and sign direction between two particles.
         */
        virtual double calculateFAbs(const Particle& p1, const Particle& p2) = 0;

        /**
         * Update the forces experienced by all the particles.
         */
        void calculateF();

        /**
         * Update the old forces and set the current forces to 0.
         */
        void calculateOldF();

        /**
         * Update the position of all the particles.
         */
        void calculateX();

        /**
         * Update the velocity of all the particles.
         */
        void calculateV();

        /**
         * Get a reference to the particle container.
         *
         * @return A reference to the particle container.
         */
        ParticleContainer& get_container();

        /**
         * Get a reference to the simulation environment.
         *
         * @return A reference to the simulation environment.
         */
        const Environment& get_env() const;
    };
} // namespace physicsCalculator