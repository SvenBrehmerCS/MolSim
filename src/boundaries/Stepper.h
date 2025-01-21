/**
 * @file Stepper.h
 *
 * @brief Define the stepper interface.
 */

#pragma once

#include <memory>

#include "boundaries/Boundary.h"
#include "physicsCalculator/Calculator.h"
#include "physicsCalculator/Tweezers.h"

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
    Vec<double> domain;

    /**
     * A boolean indicating if the simulation has an outflow boundary.
     */
    bool out = false;

    /**
     * The tweezers used for the simulation.
     */
    physicsCalculator::Tweezers tw = physicsCalculator::Tweezers({}, {}, 0.0);

public:
    /**
     * Create a stepper.
     *
     * @param bt The boundary types used for the simulation.
     * @param new_domain The size of the new domain.
     * @param tweezers The tweezers that should be used (optional)
     */
    Stepper(const std::array<BoundaryType, 6>& bt, const Vec<double>& new_domain,
        const physicsCalculator::Tweezers& tweezers = physicsCalculator::Tweezers({}, {}, 0.0));

    /**
     * Provide a default destructor for a stepper.
     */
    ~Stepper() = default;

    /**
     * Perform a single simulation step.
     *
     * @param calc The calculator that should be used for the step.
     * @param t The current time.
     */
    void step(physicsCalculator::Calculator& calc, const double t = 0.0);
};
