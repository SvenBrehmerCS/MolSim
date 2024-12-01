/**
 * @file InfContainer.h
 *
 * @brief Define the infinite container, a container that supports the O(n^2) calculation time.
 */

#pragma once

#include "ParticleContainer.h"

/**
 * @class InfContainer
 *
 * @brief Define the infinity particle container. This container supports an infinite size domain, but it requires O(n^2) force calculation.
 */
class InfContainer : public ParticleContainer {
private:
public:
    /**
     * Define the default infinity container constructor.
     */
    InfContainer();

    /**
     * Define the infinity container constructor, using a particle vector.
     *
     * @param new_particles The particles vector.
     */
    InfContainer(std::vector<Particle> new_particles);

    /**
     * Define the default infinity container constructor.
     */
    ~InfContainer();

    /**
     * Iterate through the particle pairs O(n^2).
     *
     * @param iterator The particle pair iterator.
     */
    virtual void iterate_pairs(std::function<particle_pair_it> iterator);

    /**
     * Iterate over all particles.
     *
     * @param iterator The particle iterator.
     */
    virtual void iterate_inner_particles(std::function<particle_it> iterator);

    /**
     * Update the particle positions in their cells.
     */
    virtual void update_positions();
};
