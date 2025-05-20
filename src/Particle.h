/**
 * @file
 *
 * @brief Define the default particle layout.
 *
 * @author eckhardw
 */

#pragma once

#include <array>
#include <string>

#include "utils/Vec.h"

/**
 * @class Particle
 *
 * @brief Store a single particle during the simulation.
 */
class Particle {

private:
    /**
     * The indices of the connected particles in the cell list
     */
    std::array<size_t, 8> neighbors;

    /**
     * Position of the particle
     */
    Vec<double> x;

    /**
     * Velocity of the particle
     */
    Vec<double> v;

    /**
     * Force effective on this particle
     */
    Vec<double> f;

    /**
     * Force which was effective on this particle
     */
    Vec<double> old_f;

    /**
     * The index of the particle in the particle container
     */
    size_t index;

    /**
     * Type of the particle.
     */
    int type;

    /**
     * Define wether the particle is part of a molecule
     */
    bool in_molecule = false;


public:
    /**
     * Generate a Particle without initializing velocity, position and mass.
     *
     * @param type Optional: Define the atom type.
     */
    explicit Particle(const int type = 0);

    /**
     * Generate the copy of a particle.
     *
     * @param other The particle that should be copied.
     */
    Particle(const Particle& other);

    /**
     * Generate a particle with its position, velocity and mass.
     *
     * @param x_arg The position of the particle
     * @param v_arg The velocity of the particle
     * @param type_arg Optional: Define the atom type.
     */
    Particle(const Vec<double>& x_arg, const Vec<double>& v_arg, const int type_arg = 0);

    /**
     * Destroy a particle.
     */
    virtual ~Particle();

    /**
     * Get the constant reference to the particle position.
     *
     * @return The particle position.
     */
    const Vec<double>& getX() const;

    /**
     * Get the constant reference to the particle velocity.
     *
     * @return The particle velocity.
     */
    const Vec<double>& getV() const;

    /**
     * Get the constant reference to the force on the particle.
     *
     * @return The force on the particle.
     */
    const Vec<double>& getF() const;

    /**
     * Get the constant reference to the old force on the particle.
     *
     * @return The force on the particle.
     */
    const Vec<double>& getOldF() const;

    /**
     * Get the type of the particle.
     *
     * @return The type of the particle.
     */
    int getType() const;

    /**
     * Get the index of the particle in the particle container.
     *
     * @return The index of the particle in the particle container.
     */
    size_t getIndex() const;

    /**
     * Test if the particle is part of a molecule.
     *
     * @return A boolean indicating if the particle is part of a molecule.
     */
    bool isMolecule() const;

    /**
     * Test if a particle is a direct neighbour of the particle.
     *
     * @param idx The index of the particle to test.
     *
     * @return A boolean indicating if the particle is a neighbour.
     */
    bool isDirectNeighbour(const size_t idx) const;

    /**
     * Test if a particle is a corner neighbour of the particle.
     *
     * @param idx The index of the particle to test.
     *
     * @return A boolean indicating if the particle is a corner neighbour.
     */
    bool isCornerNeighbour(const size_t idx) const;

    /**
     * Set the position of the particle.
     *
     * @param x_new The new position.
     */
    void setX(const Vec<double>& x_new);

    /**
     * Set the velocity of the particle.
     *
     * @param v_new The new velocity.
     */
    void setV(const Vec<double>& v_new);

    /**
     * Set the force affecting the particle.
     *
     * @param f_new The new force.
     */
    void setF(const Vec<double>& f_new);

    /**
     * Set the force that affected the particle the previous iteration.
     *
     * @param old_f_new The new old force.
     */
    void setOldF(const Vec<double>& old_f_new);

    /**
     * Sets the type of a particle
     *
     * @param new_type Type to be given to the particle.
     */
    void setType(const int new_type);

    /**
     * Set the index of the particle in the particle container.
     *
     * @param index_new The new index of the particle in the particle container.
     */
    void setIndex(const size_t index_new);

    /**
     * Set the indices of the neighbors.
     *
     * @param neighbors_new The new indices of the neighbors.
     */
    void setNeighbors(const std::array<size_t, 8>& neighbors_new);

    /**
     * Set if the particle is part of a molecule.
     *
     * @param in_molecule A boolean indicating if the particle is part of a molecule.
     */
    void setInMolecule(const bool in_molecule);

    /**
     * Compare if two particles are equal.
     *
     * @param other The particle which should be compared.
     *
     * @return A boolean indicating if the particles are equal.
     */
    bool operator==(const Particle& other) const;

    /**
     * Generate a string describing the particle.
     *
     * @return A string describing the particle.
     */
    std::string toString() const;
};

/**
 * Print a particle to an output stream.
 *
 * @param stream The output stream, to which the particle should be printed. (cout, cerr)
 * @param p The particle that should be printed.
 *
 * @return The output stream.
 */
std::ostream& operator<<(std::ostream& stream, const Particle& p);
