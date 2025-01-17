/**
 * @file
 *
 * @brief Define the parameters describing a particle type or particle type mixture.
 */

#pragma once

#include "utils/Vec.h"
#include <cmath>

/**
 * @class TypePairDesc
 *
 * @brief Define the parameters describing a particle type or particle type mixture.
 */
class TypePairDesc {
private:
    /**
     * Store -24 times epsilon.
     */
    double scaled_epsilon;

    /**
     * Store sigma squared.
     */
    double sigma_squared;

    /**
     * Store m1 * m2.
     */
    double mass;

public:
    /**
     * @brief Define a default constructor for a particle descriptor.
     */
    TypePairDesc() = default;

    /**
     * @brief Describe a type constructor using a single type.
     *
     * @param m1 The mass of the first type.
     * @param s1 The sigma of the first type.
     * @param e1 The epsilon of the first type.
     * @param m2 The mass of the second type.
     * @param s2 The sigma of the second type.
     * @param e2 The epsilon of the second type.
     */
    TypePairDesc(const double m1, const double s1, const double e1, const double m2, const double s2, const double e2);

    /**
     * @brief Define a default destructor for a particle descriptor.
     */
    ~TypePairDesc() = default;

    /**
     * @brief Get the scaled epsilon.
     *
     * @return The scaled epsilon.
     */
    inline double get_scaled_epsilon() const { return scaled_epsilon; }

    /**
     * @brief Get the squared sigma.
     *
     * @return The squared sigma.
     */
    inline double get_sigma_squared() const { return sigma_squared; }

    /**
     * @brief Get the mass product.
     *
     * @return The mass product.
     */
    inline double get_mass() const { return mass; }
};
