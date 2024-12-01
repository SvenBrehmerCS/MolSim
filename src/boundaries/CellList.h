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
 * @typedef index_it
 *
 * The index iterator type is a method only taking a single int.
 */
typedef void(index_it)(size_t);

/**
 * @typedef index_pair_it
 *
 * The index pair iterator type is a method taking two integers.
 */
typedef void(index_pair_it)(size_t, size_t);

/**
 * @typedef particle_it
 *
 * The particle iterator type is a method taking a single particle reference.
 */
typedef void(particle_it)(Particle&);

/**
 * @typedef particle_pair_it
 *
 * The particle pair iterator type is a method taking two particle references.
 */
typedef void(particle_pair_it)(Particle&, Particle&);

// Forward define the cell class.
class Cell;

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
    double rc;

public:
    /**
     * Define the default constructor.
     */
    CellList();

    /**
     * Define a constructor for a cell list.
     *
     * @param rc The new cutoff distance.
     * @param n_x The new x dimension.
     * @param n_y The new y dimension.
     * @param n_z The new z dimension.
     */
    CellList(const double rc, const size_t n_x, const size_t n_y, const size_t n_z);

    /**
     * Define the default destructor.
     */
    ~CellList();

    /**
     * Get the index of the given coordinates within the flat out cell list.
     *
     * @param x The x coordinate.
     * @param y The y coordinate.
     * @param z The z coordinate.
     */
    size_t get_cell_index(size_t x, size_t y, size_t z);

    /**
     * Test if a position is within a given cell.
     *
     * @param pos The position vector
     * @param x The x position of the cell
     * @param y The y position of the cell
     * @param z The z position of the cell
     */
    bool in_cell(std::array<double, 3> pos, size_t x, size_t y, size_t z);

    /**
     * Get the corner vector of the front up right corner.
     */
    std::array<double, 3> get_corner_vector();

    /**
     * Create the cell list using the particle vector. This method must only be called if the cell list was initialized with the detailed constructor.
     *
     * @param particles The vector of particles, that should be used for the simulation.
     */
    void create_list(std::vector<Particle>& particles);

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
