/**
 * @file Stepper.h
 *
 * @brief Define the stepper interface.
 */

#pragma once

#include "physicsCalculator/Calculator.h"

/**
 * @class Stepper
 *
 * @brief Define the default stepper interface.
 */
class Stepper {
public:
    /**
     * Create the default stepper constructor.
     */
    Stepper() = default;

    /**
     * Create the default copy constructor for a stepper.
     *
     * @param stepper The stepper that should be copied.
     */
    Stepper(const Stepper& stepper) = delete;

    /**
     * Provide a default destructor for a stepper.
     */
    ~Stepper() = default;

    /**
     * Perform a single simulation step.
     *
     * @param calc The calculator that should be used for the step.
     */
    virtual void step(physicsCalculator::Calculator& calc) = 0;
};
