/**
 * @file HardStepper.h
 *
 * @brief Define the hard stepper. This stepper is a stepper used for hard reflective boundaries.
 */

#pragma once

#include "Stepper.h"

/**
 * @class HaloStepper
 *
 * @brief THe halo stepper class is in charge of performing a step using the ghost particle based reflecting boundaries.
 */
class HardStepper : public Stepper {
private:
public:
    /**
     * Define the default halo stepper constructor.
     */
    HardStepper() = default;

    /**
     * Define the default halo stepper destructor.
     */
    ~HardStepper() = default;

    /**
     * The halo step method performs a single simulation step using ghost particle reflecting boundaries.
     *
     * @param calc THe calculator that should be used for the step.
     */
    virtual void step(physicsCalculator::Calculator& calc);
};
