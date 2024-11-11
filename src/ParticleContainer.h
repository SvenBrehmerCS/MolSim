/**
 * @file
 *
 * @brief The ParticleContainer is in charge of storing the particles as well as their molecular bonds.
 */

#pragma once

#include "Particle.h"
#include <cstddef>
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
     * Get a non constant reference to all the particles.
     *
     * @return The particles.
     */
    std::vector<Particle> get_particles();

    struct PIterator {
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = Particle;
        using pointer = Particle*;
        using reference = Particle&;

        PIterator(pointer ptr);

        reference operator*();
        pointer operator->();

        PIterator& operator++();
        PIterator operator++(int);
        PIterator& operator--();
        PIterator operator--(int);

        PIterator operator+(difference_type offset) const;
        PIterator operator-(difference_type offset) const;
        difference_type operator-(const PIterator& other) const;
        PIterator& operator+=(difference_type offset);
        PIterator& operator-=(difference_type offset);

        bool operator==(const PIterator& other) const;
        bool operator!=(const PIterator& other) const;
        bool operator<(const PIterator& other) const;
        bool operator>(const PIterator& other) const;
        bool operator<=(const PIterator& other) const;
        bool operator>=(const PIterator& other) const;

        reference operator[](difference_type index) const;

    private:
        pointer m_ptr;
    };

    PIterator begin();
    PIterator end();
};
