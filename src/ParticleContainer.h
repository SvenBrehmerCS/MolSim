/**
 * @file
 *
 * @brief The ParticleContainer is in charge of storing the particles as well as their molecular bonds.
 */

#pragma once

#include "Particle.h"
#include <iterator>
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
     * Get the reference of a particle at a certain index.
     * 
     * @param idx THe index of the referenced particle.
     * 
     * @return The reference to the particle 
     */
    Particle& operator[](const size_t idx);

    std::vector<Particle>::iterator begin();
    std::vector<Particle>::const_iterator begin() const;

    std::vector<Particle>::iterator end();
    std::vector<Particle>::const_iterator end() const;

    size_t size() const;
    void resize(size_t count);
};
