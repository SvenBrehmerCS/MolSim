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
 * @class CellList
 *
 * @brief Define the cell list layout and its utility functions.
 */
class CellList {
private:
    /**
     * Store the cells in a flat out vector.
     */
    std::vector<std::list<size_t>> cells;

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
    std::array<double, 3> cell_size;

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
    CellList(const double rc, const std::array<double, 3>& domain);

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
     */
    std::array<double, 3> get_corner_vector();

    /**
     * Create the cell list using the particle vector. This method must only be called if the cell list was initialized with the detailed constructor.
     *
     * @param particles The vector of particles, that should be used for the simulation.
     */
    void create_list(const std::vector<Particle>& particles);

    /**
     * Loop through the particle pairs within the domain and halo.
     *
     * @param iterator The particle iteration lambda.
     * @param particles The particles vector.
     */
    void loop_cell_pairs(std::function<particle_pair_it> iterator, std::vector<Particle>& particles);

    /**
     * Get the cutoff distance.
     *
     * @return The cutoff distance.
     */
    double getRC();
};
