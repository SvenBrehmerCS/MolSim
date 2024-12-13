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
     * An array storing the boundary conditions.
     */
    std::array<Boundary*, 6> bc;

    /**
     * An array storing the boundary condition descriptors.
     */
    std::array<BoundaryType, 6> bound_t;

    /**
     * An array storing the domain size.
     */
    std::array<double, 3> domain;

    /**
     * A boolean indicating if the simulation has an infinite domain.
     */
    bool inf;

public:
    // TODO: Move the creation of the boundary conditions into the stepper constructor -> cleaner main function.
    /**
     * Create a stepper.
     *
     * @param boundaries The boundaries of the simulation.
     * @param is_inf A boolean indicating if the simulation is an infinite domain.
     */
    Stepper(const std::array<Boundary*, 6>& boundaries, const std::array<BoundaryType, 6>& bt, const bool is_inf, const std::array<double, 3>& d);

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
