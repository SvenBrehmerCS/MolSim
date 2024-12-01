/**
 * @file OutStepper.h
 *
 * @brief Describe how the output stepper is
 */

#pragma once

#include "Stepper.h"

/**
 * @class OutStepper
 *
 * @brief Define the stepper for the outflow boundary condition.
 */
class OutStepper : public Stepper {
private:
public:
    /**
     * Define the default outflow stepper constructor.
     */
    OutStepper() = default;

    /**
     * Define the default outflow stepper destructor.
     */
    ~OutStepper() = default;

    /**
     * Perform a single outflow step using the outflow stepper.
     *
     * @param calc The calculator used for the simulation.
     */
    virtual void step(physicsCalculator::Calculator& calc);
};
