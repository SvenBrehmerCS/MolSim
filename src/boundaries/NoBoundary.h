/**
 * @file NoBoundary.h
 *
 * @brief Define the boundary condition allowing for infinite domains.
 */

#pragma once

#include "boundaries/Boundary.h"

/**
 * @class GhostBoundary
 *
 * @brief Define the infinite domain boundary calculations.
 */
class NoBoundary : public Boundary {
public:
    /**
     * Define a constructor for an infinite boundary condition.
     *
     * @param new_pos The position of the boundary.
     * @param new_dim The dimension of the boundary.
     */
    NoBoundary(const double new_pos, const int new_dim);

    /**
     * Define a default destructor for an infinite boundary.
     */
    ~NoBoundary() = default;

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
