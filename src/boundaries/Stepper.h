/**
 * @file Stepper.h
 *
 * @brief Define the stepper interface.
 */

#pragma once

#include <memory>

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
    std::array<std::unique_ptr<Boundary>, 6> bc;

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
    /**
     * Create a stepper.
     *
     * @param bt The boundary types used for the simulation.
     * @param new_domain The size of the new domain.
     */
    Stepper(const std::array<BoundaryType, 6>& bt, const std::array<double, 3>& new_domain);

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
