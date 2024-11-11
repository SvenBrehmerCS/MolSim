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

    class PIterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = Particle;
        using pointer = Particle*;
        using reference = Particle&;

        PIterator(pointer ptr)
            : m_ptr(ptr) { }

        reference operator*() /*const?*/ { return *m_ptr; }
        pointer operator->() /*const?*/ { return m_ptr; }

        // Prefix/Postfix increment
        PIterator& operator++() {
            ++m_ptr;
            return *this;
        }
        PIterator operator++(int) {
            PIterator tmp = *this;
            ++(*this);
            return tmp;
        }
        // Prefix/Postfix decrement
        PIterator& operator--() {
            --m_ptr;
            return *this;
        }
        PIterator operator--(int) {
            PIterator tmp = *this;
            --(*this);
            return tmp;
        }

        difference_type operator-(const PIterator& other) const { return m_ptr - other.m_ptr; }

        PIterator operator+(difference_type offset) const { return PIterator(m_ptr + offset); }
        PIterator operator-(difference_type offset) const { return PIterator(m_ptr - offset); }
        PIterator& operator+=(difference_type offset) {
            m_ptr += offset;
            return *this;
        }
        PIterator& operator-=(difference_type offset) {
            m_ptr -= offset;
            return *this;
        }

        reference operator[](difference_type diff) const { return m_ptr[diff]; }

        bool operator==(const PIterator& other) const { return m_ptr == other.m_ptr; }
        bool operator!=(const PIterator& other) const { return m_ptr != other.m_ptr; }
        bool operator<(const PIterator& other) const { return m_ptr < other.m_ptr; }
        bool operator>(const PIterator& other) const { return m_ptr > other.m_ptr; }
        bool operator<=(const PIterator& other) const { return m_ptr <= other.m_ptr; }
        bool operator>=(const PIterator& other) const { return m_ptr >= other.m_ptr; }

    private:
        pointer m_ptr;
    };

    PIterator getparticles() {
        return begin();
    }

    PIterator begin() {
        return PIterator(&particles.front());
    }
    PIterator end() {
        return PIterator(&particles.back());
    }
};
