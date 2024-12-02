/**
 * @file Stepper.h
 *
 * @brief Define the stepper interface.
 */

#pragma once

#include "boundaries/Boundary.h"
#include "physicsCalculator/Calculator.h"

/**
 * @class Stepper
 *
 * @brief Define the default stepper.
 */
class Stepper {
private:
    /**
     * An array storing the domain size.
     */
    std::array<Boundary*, 6> bc;

    /**
     * A boolean indicating if the simulation has an infinite domain.
     */
    bool inf;

public:
    /**
     * Create a stepper.
     *
     * @param boundaries The boundaries of the simulation.
     * @param is_inf A boolean indicating if the simulation is an infinite domain.
     */
    Stepper(const std::array<Boundary*, 6>& boundaries, const bool is_inf);

    /**
     * Provide a default destructor for a stepper.
     */
    ~Stepper() = default;

    /**
     * Perform a single simulation step.
     *
     * @param calc The calculator that should be used for the step.
     */
    void step(physicsCalculator::Calculator& calc);
};
