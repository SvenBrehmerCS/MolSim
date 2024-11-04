/**
 * @file
 *
 * @brief Defines a default interface for the physics caculators
 */

#pragma once

class ParticleContainer;
class Environment;

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

    public:
        /**
         * Define the default destructor for every calculator.
         */
        virtual ~Calculator() {};

        /**
         * Update the forces and old forces experienced by all the particles.
         *
         * @param container The container for the particles.
         * @param env The simulation environment.
         */
        virtual void calculateF(ParticleContainer& container, const Environment& env) = 0;

        /**
         * Update the position of all the particles.
         *
         * @param container The container for the particles.
         * @param env The simulation environment.
         */
        virtual void calculateX(ParticleContainer& container, const Environment& env) = 0;

        /**
         * Update the velocity of all the particles.
         *
         * @param container The container for the particles.
         * @param env The simulation environment.
         */
        virtual void calculateV(ParticleContainer& container, const Environment& env) = 0;

        // TODO: Step (perform a single step)
        // TODO: Constructor with env and container
    };
} // namespace physicsCalculator