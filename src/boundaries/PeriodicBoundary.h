/**
 * @file
 *
 * @brief Define the boundary condition allowing for a periodic boundary.
 */

#pragma once

#include "boundaries/Boundary.h"

/**
 * @class PeriodicBoundary
 *
 * @brief Define the boundary condition allowing for a periodic boundary.
 */
class PeriodicBoundary : Boundary {
private:
public:
    /**
     * Define a constructor for a periodic boundary condition.
     *
     * @param new_pos The position of the boundary.
     * @param new_dim The dimension of the boundary.
     */
    PeriodicBoundary(const double new_pos, const int new_dim);

    /**
     * Define a default destructor for a periodic boundary.
     */
    ~PeriodicBoundary() {};

    /**
     * Define which calculations should be performed after calling updateF.
     *
     * @param particle The particle which should be updated.
     * @param calc The calculator used for the force calculation.
     */
    virtual void postF(Particle& particle, physicsCalculator::Calculator& calc);

    /**
     * Define which calculations should be performed after calling updateX.
     *
     * @param particle The particle container of which should be updated.
     */
    virtual void postX(Particle& particle);
};
