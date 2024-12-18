/**
 * @file DSContainer.h
 *
 * @brief Define the infinite container, a container that supports the O(n^2) calculation time.
 */

#pragma once

#include "ParticleContainer.h"

/**
 * @class DSContainer
 *
 * @brief Define the infinity particle container. This container supports an infinite size domain, but it requires O(n^2) force calculation.
 */
class DSContainer : public ParticleContainer {
private:
public:
    /**
     * Define the default infinity container constructor.
     */
    DSContainer();

    /**
     * Define the infinity container constructor, using a particle vector and their types.
     *
     * @param new_particles The particles vector.
     * @param new_desc The types of the particles to be stored.
     */
    DSContainer(const std::vector<Particle>& new_particles, const std::vector<TypeDesc>& new_desc);

    /**
     * Define the infinity container constructor, using a particle vector.
     *
     * @param new_domain The new domain size.
     */
    DSContainer(const std::array<double, 3>& new_domain);

    /**
     * Define the infinity container constructor, using a particle vector and their types.
     *
     * @param new_particles The particles vector.
     * @param new_domain The new domain size.
     * @param new_desc The types of the particles to be stored.
     */
    DSContainer(const std::vector<Particle>& new_particles, const std::array<double, 3>& new_domain, const std::vector<TypeDesc>& new_desc);

    /**
     * Define the default infinity container constructor.
     */
    ~DSContainer();

    /**
     * Iterate through the particle pairs O(n^2).
     *
     * @param iterator The particle pair iterator.
     */
    virtual void iterate_pairs(const std::function<particle_pair_it>& iterator);

    /**
     * Update the particle positions in their cells.
     */
    virtual void update_positions();
};
