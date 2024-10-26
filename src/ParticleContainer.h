/**
 * ParticleContainer.h
 *
 * The class ParticleContainer is in charge of storing the particules as well as there molecular bondings.
 */

#pragma once

#include "Particle.h"
#include <vector>

/**
 * Store all the particles during the simulation.
 */
class ParticleContainer {
private:
    /**
     * Store all the particles required during the simulation.s
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
     * @param new_particles The particles wich should be stored.
     */
    ParticleContainer(std::vector<Particle>& new_particles);

    /**
     * Destroy all particles.
     */
    ~ParticleContainer();

    /**
     * Get a non constant refference to all the particles.
     *
     * @return The particles.
     */
    std::vector<Particle>& get_particles();
};
