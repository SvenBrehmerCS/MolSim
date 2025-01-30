/**
 * @file
 *
 * @brief Define the parameters describing a particle type or particle type mixture.
 */

#pragma once

#include "utils/Vec.h"
#include <cmath>

/**
 * @class TypeDesc
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
     * Store the stiffness of the type.
     */
    double stiff;

    /**
     * Store the preferred distance of particles within the domain.
     */
    double rad;

    /**
     * Store the preferred distance of particles within the domain, over corners.
     */
    double rad_scale;

    /**
     * Store the membrane cutoff distance.
     */
    double c;

    /**
     * Store the gravity affecting the type.
     */
    Vec<double> G;

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
     * @param k The stiffness of the type. (default: 0.0)
     * @param r0 The particle distance of the type. (default: 0.0)
     */
    TypeDesc(const double m, const double s, const double e, const double delta_t, const double g, const double k = 0.0, const double r0 = 0.0);

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
     * @brief Get the stiffness of the molecule type.
     *
     * @return The stiffness of the molecule type.
     */
    inline double get_k() const { return stiff; }

    /**
     * @brief Get the preferred particle distance within the membrane.
     *
     * @return The prefered particle distance.
     */
    inline double get_r0() const { return rad; }

    /**
     * @brief Get the preferred particle distance within the membrane.
     *
     * @return The prefered particle distance.
     */
    inline double get_r1() const { return rad_scale; }

    /**
     * @brief Get the membrane force cutoff distance.
     *
     * @return The membrane force cutoff distance.
     */
    inline double get_cut() const { return c; }

    /**
     * @brief Get the gravity vector of a specific type.
     *
     * @return The gravity vector.
     */
    inline Vec<double> get_G() const { return G; }
};
