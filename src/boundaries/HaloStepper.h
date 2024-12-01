/**
 * @file HaloStepper.h
 *
 * @brief Define the halo stepper. This stepper is a stepper used for ghost reflective boundaries.
 */

#pragma once

#include "Stepper.h"

/**
 * @class HaloStepper
 *
 * @brief THe halo stepper class is in charge of performing a step using the ghost particle based reflecting boundaries.
 */
class HaloStepper : public Stepper {
private:
public:
    /**
     * Define the default halo stepper constructor.
     */
    HaloStepper() = default;

    /**
     * Define the default halo stepper destructor.
     */
    ~HaloStepper() = default;

    /**
     * The halo step method performs a single simulation step using ghost particle reflecting boundaries.
     *
     * @param calc THe calculator that should be used for the step.
     */
    virtual void step(physicsCalculator::Calculator& calc);
};
