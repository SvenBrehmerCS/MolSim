/**
 * @file GhostBoundary.h
 *
 * @brief Define the boundary condition utilizing ghost particles.
 */

#pragma once

#include "boundaries/Boundary.h"

/**
 * @class GhostBoundary
 *
 * @brief Define the ghost boundary calculations.
 */
class GhostBoundary : public Boundary {
public:
    /**
     * Define a constructor for a ghost boundary condition.
     *
     * @param new_pos The position of the boundary.
     * @param new_dim The dimension of the boundary.
     */
    GhostBoundary(const double new_pos, const int new_dim);

    /**
     * Define a default destructor for a ghost boundary.
     */
    virtual ~GhostBoundary();

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
