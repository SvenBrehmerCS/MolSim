/**
 * @file BoxContainer.h
 *
 * @brief Define the box container.
 */

#pragma once

#include "CellList.h"
#include "ParticleContainer.h"

/**
 * @class BoxContainer
 *
 * @brief Define a container for the particles, which are located in a boxs.
 */
class BoxContainer : public ParticleContainer {
private:
    /**
     * Define the cell list.
     */
    CellList cells;

public:
    /**
     * Define the box container.
     *
     * @param rc The cutoff distance used for the simulation.
     * @param n_x The number of cells in the x dimension.
     * @param n_y The number of cells in the y dimension.
     * @param n_z The number of cells in the z dimension.
     */
    BoxContainer(std::vector<Particle>& new_particles, const double rc, const std::array<double, 3>& new_domain);

    /**
     * Define the default destructor for a box container.
     */
    ~BoxContainer();

    /**
     * Iterate through the particle pairs.
     *
     * @param iterator The function used to iterate over the particle pairs.
     */
    virtual void iterate_pairs(std::function<particle_pair_it> iterator);

    /**
     * Get the cutoff distance of the simulation.
     *
     * @return The cutoff distance.
     */
    double getRC();

    /**
     * Update the particle positions in their cells.
     */
    virtual void update_positions();
};
