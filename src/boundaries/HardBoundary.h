/**
 * @file HardBoundary.h
 *
 * @brief Define the boundary condition arising from a hard reflecting boundary.
 */

#pragma once

#include "boundaries/Boundary.h"

/**
 * @class HardBoundary
 *
 * @brief Define the default layout for the boundary force calculations.
 */
class HardBoundary : public Boundary {
public:
    /**
     * Define a constructor for a hard boundary condition.
     *
     * @param new_pos The position of the boundary.
     * @param new_dim The dimension of the boundary.
     */
    HardBoundary(const double new_pos, const int new_dim);

    /**
     * Define a default destructor for a hard boundary.
     */
    virtual ~HardBoundary();

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
