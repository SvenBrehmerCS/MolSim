/**
 * @file
 *
 * @brief The ParticleContainer is in charge of storing the particles as well as their molecular bonds.
 */

#pragma once

#include "Particle.h"
#include <vector>

/**
 * @class ParticleContainer
 *
 * @brief Store all the particles during the simulation.
 */
class ParticleContainer {
private:
    /**
     * Store all the particles required during the simulation.
     */
    std::vector<Particle> particles;

public:
    /**
     * Create a particle container with an empty particle vector.
     */
    ParticleContainer();

    /**
     * Create a particle container from a vector of particles.
     *
     * @param new_particles The particles which should be stored.
     */
    ParticleContainer(const std::vector<Particle>& new_particles);

    /**
     * Destroy all particles.
     */
    ~ParticleContainer();

    /**
     * Get a non constant reference to all the particles.
     *
     * @return The particles.
     */
    std::vector<Particle>& get_particles();
};
