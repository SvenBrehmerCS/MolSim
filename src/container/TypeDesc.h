/**
 * @file
 *
 * @brief Define the parameters describing a particle type or particle type mixture.
 */

#pragma once

#include "utils/ArrayUtils.h"
#include <array>
#include <cmath>

/**
 * @class TypePairDesc
 *
 * @brief Define the parameters describing a particle type or particle type mixture.
 */
class TypeDesc {
private:
    /**
     * Store the mass of the type.
     */
    double mass;

    /**
     * Store the sigma of the type.
     */
    double sigma;

    /**
     * Store the epsilon of the type.
     */
    double epsilon;


public:
    /**
     * @brief Define a default constructor for a particle descriptor.
     */
    TypeDesc() = default;

    /**
     * @brief Describe a type constructor using a single type.
     *
     * @param m The mass of the type.
     * @param s The sigma of the type.
     * @param e The epsilon of the type.
     * @param delta_t THe time delta for the simulation.
     * @param g The downward gravity
     */
    TypeDesc(const double m, const double s, const double e, const double delta_t, const double g);

    /**
     * @brief Define a default destructor for a particle descriptor.
     */
    ~TypeDesc() = default;
};
