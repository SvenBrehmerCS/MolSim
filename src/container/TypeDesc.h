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

    /**
     * Store delta_t * 0.5 / m of the type.
     */
    double dt_m;

    /**
     * Store delta_t * delta_t * 0.5 / m of the type.
     */
    double dt_dt_m;

    /**
     * Store the gravity affecting the type.
     */
    std::array<double, 3> G;

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

    /**
     * @brief Get the mass of a specific type.
     *
     * @return The mass.
     */
    inline double get_mass() const { return mass; }

    /**
     * @brief Get the sigma of a specific type.
     *
     * @return The sigma.
     */
    inline double get_sigma() const { return sigma; }

    /**
     * @brief Get the epsilon of a specific type.
     *
     * @return The epsilon.
     */
    inline double get_epsilon() const { return epsilon; }

    /**
     * @brief Get the delta_t * 0.5 / m of a specific type.
     *
     * @return The delta_t * 0.5 / m.
     */
    inline double get_dt_m() const { return dt_m; }

    /**
     * @brief Get the delta_t * delta_t * 0.5 / m of a specific type.
     *
     * @return The delta_t * delta_t * 0.5 / m.
     */
    inline double get_dt_dt_m() const { return dt_dt_m; }

    /**
     * @brief Get the gravity vector of a specific type.
     *
     * @return The gravity vector.
     */
    inline std::array<double, 3> get_G() const { return G; }
};
