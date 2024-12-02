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
     * @param new_domain Vector of the number of cells in each direction.
     */
    BoxContainer(const double rc, const std::array<double, 3>& new_domain);

    /**
     * Define the box container.
     *
     * @param new_particles Vector of test particles.
     * @param rc The cutoff distance used for the simulation.
     * @param new_domain Vector of the number of cells in each direction.
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
