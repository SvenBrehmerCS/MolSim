/**
 * @file
 *
 * @brief Handles the storage of the particles as well as their molecular bonds.
 */

#pragma once

#include "Particle.h"
#include "container/CellList.h"
#include "container/TypeDesc.h"
#include "container/TypePairDesc.h"

#include <functional>
#include <iterator>
#include <vector>

/**
 * @class ParticleContainer
 *
 * @brief Store all the particles during the simulation.
 */
class ParticleContainer {
protected:
    /**
     * Store all the particles required during the simulation.
     */
    std::vector<Particle> particles;

    /**
     * Store the domain size of the container.
     */
    std::array<double, 3> domain = { 0.0, 0.0, 0.0 };

    /**
     * Store the descriptors for the different types.
     */
    std::vector<TypeDesc> types;

    /**
     * Store the descriptors for the different type pairs.
     */
    std::vector<TypePairDesc> type_pairs;

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
    ParticleContainer(const std::vector<Particle>& new_particles, const std::vector<TypeDesc>& new_desc);

    /**
     * Create a particle container of a certain size.
     *
     * @param new_domain The new domain size.
     */
    ParticleContainer(const std::array<double, 3>& new_domain);

    /**
     * Create a particle container of a certain size from a vector of particles.
     *
     * @param new_particles The particles which should be stored.
     * @param new_domain The new domain size.
     */
    ParticleContainer(const std::vector<Particle>& new_particles, const std::array<double, 3>& new_domain, const std::vector<TypeDesc>& new_desc);

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

    /**
     * Returns a read/write iterator that points to the first element in the particle container.
     * Iteration is done in ordinary element order
     *
     * @return read/write iterator
     */
    std::vector<Particle>::iterator begin();

    /**
     * Returns a read-only (constant) iterator that points to the first element in the particle container.
     * Iteration is done in ordinary element order.
     *
     * @return read-only iterator
     */
    std::vector<Particle>::const_iterator begin() const;

    /**
     * Returns a read/write iterator that points one past the last element in the particle container.
     * Iteration is done in ordinary element order.
     *
     * @return read/write iterator
     */
    std::vector<Particle>::iterator end();

    /**
     * Returns a read-only (constant) iterator that points one past the last element in the particle container.
     * Iteration is done in ordinary element order.
     *
     * @return read-only iterator
     */
    std::vector<Particle>::const_iterator end() const;

    /**
     * Returns the number of elements in the particle container.
     */
    size_t size() const;

    /**
     * Resizes the particle container to the specified number of elements.
     * If the number is smaller than the particle container current size the particle container is truncated,
     * otherwise default constructed elements are appended.
     *
     * @param new_size  Number of elements the particle container should contain.
     */
    void resize(size_t new_size);

    /**
     * Iterate over all pairs and apply the provided method to both particles of the pair.
     *
     * @param iterator The iterator lambda that should loop over all the pairs.
     */
    virtual void iterate_pairs(const std::function<particle_pair_it>& iterator) = 0;

    /**
     * Remove all particles which are out of the domain.
     *
     * @param domain The size of the domain.
     */
    void remove_particles_out_of_domain();

    /**
     * Update the particle positions in their cells.
     */
    virtual void update_positions() = 0;

    /**
     * Get the vector pointing to the corner of the domain.
     *
     * @return The corner vector.
     */
    const std::array<double, 3>& get_corner_vector() const;

    /**
     * Create a type table using the types.
     *
     * @param A vector of types that should be used for the type creation.
     */
    void build_type_table(const std::vector<TypeDesc>& new_types);

    /**
     * Get the descriptor of a particle pair.
     *
     * @param t1 The first type.
     * @param t1 The second type.
     *
     * @return The pair descriptor.
     */
    inline const TypePairDesc& get_type_pair_descriptor(const int t1, const int t2) const { return type_pairs[t1 + types.size() * t2]; }

    /**
     * Get the descriptor of a particle.
     *
     * @param t The particle type.
     *
     * @return The particle type descriptor.
     */
    inline const TypeDesc& get_type_descriptor(const int t) const { return types[t]; }
};
