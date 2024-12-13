/**
 * @file BoxContainer.h
 *
 * @brief Define the box container.
 *
 */

#pragma once

#include "CellList.h"
#include "ParticleContainer.h"

/**
 * @class BoxContainer
 *
 * @brief Define a container for the particles, which are located in a boxs.
 *
 * The graph shows the time required to simulate the force calculation for an increasing number of particles.
 * \image html performance.png "PerformanceTest"
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
    BoxContainer(const std::vector<Particle>& new_particles, const double rc, const std::array<double, 3>& new_domain);

    /**
     * Define the default destructor for a box container.
     */
    ~BoxContainer();

    /**
     * Iterate through the particle pairs.
     *
     * @param iterator The function used to iterate over the particle pairs.
     */
    virtual void iterate_pairs(const std::function<particle_pair_it>& iterator);

    /**
     * Loop through the xy boundary plain pairs.
     *
     * @param iterator The particle iteration lambda.
     */
    void iterate_xy_pairs(const std::function<particle_pair_it>& iterator);

    /**
     * Loop through the xz boundary plain pairs.
     *
     * @param iterator The particle iteration lambda.
     */
    void iterate_xz_pairs(const std::function<particle_pair_it>& iterator);

    /**
     * Loop through the yz boundary plain pairs.
     *
     * @param iterator The particle iteration lambda.
     */
    void iterate_yz_pairs(const std::function<particle_pair_it>& iterator);

    /**
     * Loop through the near x-axis pairs.
     *
     * @param iterator The particle iteration lambda.
     */
    void loop_x_near(const std::function<particle_pair_it>& iterator);

    /**
     * Loop through the far x-axis pairs.
     *
     * @param iterator The particle iteration lambda.
     */
    void loop_x_far(const std::function<particle_pair_it>& iterator);

    /**
     * Loop through the near y-axis pairs.
     *
     * @param iterator The particle iteration lambda.
     */
    void loop_y_near(const std::function<particle_pair_it>& iterator);

    /**
     * Loop through the far y-axis pairs.
     *
     * @param iterator The particle iteration lambda.
     */
    void loop_y_far(const std::function<particle_pair_it>& iterator);

    /**
     * Loop through the near z-axis pairs.
     *
     * @param iterator The particle iteration lambda.
     */
    void loop_z_near(const std::function<particle_pair_it>& iterator);

    /**
     * Loop through the far z-axis pairs.
     *
     * @param iterator The particle iteration lambda.
     */
    void loop_z_far(const std::function<particle_pair_it>& iterator);

    /**
     * Loop through the origin corner pairs.
     *
     * @param iterator The particle iteration lambda.
     */
    void loop_origin_corner(const std::function<particle_pair_it>& iterator);

    /**
     * Loop through the x corner pairs.
     *
     * @param iterator The particle iteration lambda.
     */
    void loop_x_corner(const std::function<particle_pair_it>& iterator);

    /**
     * Loop through the y corner pairs.
     *
     * @param iterator The particle iteration lambda.
     */
    void loop_y_corner(const std::function<particle_pair_it>& iterator);

    /**
     * Loop through the xy corner pairs.
     *
     * @param iterator The particle iteration lambda.
     */
    void loop_xy_corner(const std::function<particle_pair_it>& iterator);

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
