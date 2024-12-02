/**
 * @file GhostBoundary.h
 *
 * @brief Define the general layout of a boundary condition.
 */

#pragma once

#include "boundaries/ParticleContainer.h"
#include "physicsCalculator/Calculator.h"

/**
 * @class Boundary
 *
 * @brief Define the default layout for the boundary conditions.
 */
class Boundary {
protected:
    /**
     * Store the position of the boundary.
     */
    double pos;

    /**
     * Store the dimension of the boundary. (Either 0, 1 or 2)
     */
    int dim;

public:
    /**
     * Define a constructor for a boundary condition.
     *
     * @param new_pos The position of the boundary.
     * @param new_dim The dimension of the boundary.
     */
    Boundary(const double new_pos, const int new_dim);

    /**
     * Define a default destructor for a boundary.
     */
    virtual ~Boundary() {};

    /**
     * Define which calculations should be performed after calling updateF.
     *
     * @param particle The particle which should be updated.
     * @param calc The calculator used for the force calculation.
     */
    virtual void postF(Particle& particle, physicsCalculator::Calculator& calc) = 0;

    /**
     * Define which calculations should be performed after calling updateX.
     *
     * @param particle The particle container of which should be updated.
     */
    virtual void postX(Particle& particle) = 0;
};
