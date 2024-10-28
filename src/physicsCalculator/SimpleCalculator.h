/**
 * @file
 *
 * @brief Handles the basic physics calculations
 */

#pragma once

#include "Calculator.h"
#include "Environment.h"
#include "ParticleContainer.h"
#include "utils/ArrayUtils.h"

namespace physicsCalculator {

    /**
     * @class Calculator
     *
     * @brief The default class for an instance of a calculator
     */
    class SimpleCalculator : public Calculator {

    public:
        SimpleCalculator();

        virtual ~SimpleCalculator();

        /**
         * Update the forces and old forces experienced by all the particles based on the basic calculation scheme.
         *
         * @param container The container for the particles.
         * @param env The simulation environment.
         */
        virtual void calculateF(ParticleContainer& container, const Environment& env);

        /**
         * Update the position of all the particles based on the basic calculation scheme.
         *
         * @param container The container for the particles.
         * @param env The simulation environment.
         */
        virtual void calculateX(ParticleContainer& container, const Environment& env);

        /**
         * Update the velocity of all the particles based on the basic calculation scheme.
         *
         * @param container The container for the particles.
         * @param env The simulation environment.
         */
        virtual void calculateV(ParticleContainer& container, const Environment& env);
    };
} // namespace physicsCalculator