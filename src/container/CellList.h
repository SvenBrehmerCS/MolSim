/**
 * @file CellList.h
 *
 * @brief Define the utility required for the linked cell algorithm.
 */

#pragma once

#include "Particle.h"

#include <functional>
#include <list>
#include <vector>

/**
 * @typedef particle_pair_it
 *
 * The particle pair iterator type is a method taking two particle references.
 */
typedef void(particle_pair_it)(Particle&, Particle&);

/**
 * @typedef particle_it
 *
 * The particle iterator type is a method taking a particle reference.
 */
typedef void(particle_it)(Particle&);

/**
 * @class CellList
 *
 * @brief Define the cell list layout and its utility functions.
 */
class CellList {
private:
    /**
     * Store the cells in a flat out vector.
     */
    std::vector<std::vector<size_t>> cells;

    /**
     * Define the dimensions of the domain.
     */
    size_t n_x, n_y, n_z;

    /**
     * Define the cutoff distance.
     */
    double rc, rc_squ;

    /**
     * Define the cell sizes
     */
    Vec<double> cell_size;

    /**
     * Define the domain size and sub dimensions.
     */
    Vec<double> dom, domain_x, domain_y, domain_z, domain_xy, domain_xz, domain_yz;


public:
    /**
     * Define the default constructor.
     */
    CellList() = default;

    /**
     * Define a constructor for a cell list.
     *
     * @param rc The new cutoff distance.
     * @param domain The domain size.
     */
    CellList(const double rc, const Vec<double>& domain);

    /**
     * Define the default destructor.
     */
    ~CellList() = default;

    /**
     * Get the index of the given coordinates within the flat out cell list.
     *
     * @param x The x coordinate.
     * @param y The y coordinate.
     * @param z The z coordinate.
     */
    size_t get_cell_index(const size_t x, const size_t y, const size_t z);

    /**
     * Get the corner vector of the front up right corner.
     *
     * @return The corner vector.
     */
    Vec<double> get_corner_vector();

    /**
     * Create the cell list using the particle vector. This method must only be called if the cell list was initialized with the detailed constructor.
     *
     * @param particles The vector of particles, that should be used for the simulation.
     */
    void create_list(const std::vector<Particle>& particles);

    /**
     * Loop through the particle pairs within the domain.
     *
     * @param iterator The particle iteration lambda.
     * @param particles The particles vector.
     */
    void loop_cell_pairs(const std::function<particle_pair_it>& iterator, std::vector<Particle>& particles);

    /**
     * Loop through the particles within the halo.
     *
     * @param iterator The particle iteration lambda.
     * @param particles The particles vector.
     */
    void loop_halo(const std::function<particle_it>& iterator, std::vector<Particle>& particles);

    /**
     * Loop through the particles within the boundary cells.
     *
     * @param iterator The particle iteration lambda.
     * @param particles The particles vector.
     */
    void loop_boundary(const std::function<particle_it>& iterator, std::vector<Particle>& particles);

    /**
     * Loop through the inner particles (including the boundary particles).
     *
     * @param iterator The particle iteration lambda.
     * @param particles The particles vector.
     */
    void loop_inner(const std::function<particle_it>& iterator, std::vector<Particle>& particles);

    /**
     * Loop through the xy boundary plain pairs.
     *
     * @param iterator The particle iteration lambda.
     * @param particles The particles vector.
     */
    void loop_xy_pairs(const std::function<particle_pair_it>& iterator, std::vector<Particle>& particles);

    /**
     * Loop through the xz boundary plain pairs.
     *
     * @param iterator The particle iteration lambda.
     * @param particles The particles vector.
     */
    void loop_xz_pairs(const std::function<particle_pair_it>& iterator, std::vector<Particle>& particles);

    /**
     * Loop through the yz boundary plain pairs.
     *
     * @param iterator The particle iteration lambda.
     * @param particles The particles vector.
     */
    void loop_yz_pairs(const std::function<particle_pair_it>& iterator, std::vector<Particle>& particles);

    /**
     * Loop through the near x-axis pairs.
     *
     * @param iterator The particle iteration lambda.
     * @param particles The particles vector.
     */
    void loop_x_near(const std::function<particle_pair_it>& iterator, std::vector<Particle>& particles);

    /**
     * Loop through the far x-axis pairs.
     *
     * @param iterator The particle iteration lambda.
     * @param particles The particles vector.
     */
    void loop_x_far(const std::function<particle_pair_it>& iterator, std::vector<Particle>& particles);

    /**
     * Loop through the near y-axis pairs.
     *
     * @param iterator The particle iteration lambda.
     * @param particles The particles vector.
     */
    void loop_y_near(const std::function<particle_pair_it>& iterator, std::vector<Particle>& particles);

    /**
     * Loop through the far y-axis pairs.
     *
     * @param iterator The particle iteration lambda.
     * @param particles The particles vector.
     */
    void loop_y_far(const std::function<particle_pair_it>& iterator, std::vector<Particle>& particles);

    /**
     * Loop through the near z-axis pairs.
     *
     * @param iterator The particle iteration lambda.
     * @param particles The particles vector.
     */
    void loop_z_near(const std::function<particle_pair_it>& iterator, std::vector<Particle>& particles);

    /**
     * Loop through the far z-axis pairs.
     *
     * @param iterator The particle iteration lambda.
     * @param particles The particles vector.
     */
    void loop_z_far(const std::function<particle_pair_it>& iterator, std::vector<Particle>& particles);

    /**
     * Loop through the origin corner pairs.
     *
     * @param iterator The particle iteration lambda.
     * @param particles The particles vector.
     */
    void loop_origin_corner(const std::function<particle_pair_it>& iterator, std::vector<Particle>& particles);

    /**
     * Loop through the x corner pairs.
     *
     * @param iterator The particle iteration lambda.
     * @param particles The particles vector.
     */
    void loop_x_corner(const std::function<particle_pair_it>& iterator, std::vector<Particle>& particles);

    /**
     * Loop through the y corner pairs.
     *
     * @param iterator The particle iteration lambda.
     * @param particles The particles vector.
     */
    void loop_y_corner(const std::function<particle_pair_it>& iterator, std::vector<Particle>& particles);

    /**
     * Loop through the xy corner pairs.
     *
     * @param iterator The particle iteration lambda.
     * @param particles The particles vector.
     */
    void loop_xy_corner(const std::function<particle_pair_it>& iterator, std::vector<Particle>& particles);

    /**
     * Get the cutoff distance.
     *
     * @return The cutoff distance.
     */
    double getRC();
};
