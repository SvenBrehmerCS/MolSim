/**
 * @file InfStepper.h
 *
 * @brief
 */

#pragma once

#include "Stepper.h"

/**
 * @class InfStepper
 *
 * @brief Define a stepper with infinite calculation distance.
 */
class InfStepper : public Stepper {
private:
public:
    /**
     * Define the default constructor for an infinite stepper.
     */
    InfStepper() = default;

    /**
     * Define the destructor for an infinite stepper.
     */
    ~InfStepper() = default;

    /**
     * Define the step method for an infinite stepper.
     *
     * @param calc The calculator that should be used for the stepping.
     */
    virtual void step(physicsCalculator::Calculator& calc);
};
