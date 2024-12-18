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

/**
 * @class Particle
 *
 * @brief Store a single particle during the simulation.
 */
class Particle {

private:
    /**
     * Position of the particle
     */
    std::array<double, 3> x;

    /**
     * Velocity of the particle
     */
    std::array<double, 3> v;

    /**
     * Force effective on this particle
     */
    std::array<double, 3> f;

    /**
     * Force which was effective on this particle
     */
    std::array<double, 3> old_f;

    /**
     * Type of the particle.
     */
    int type;

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
    Particle(const std::array<double, 3>& x_arg, const std::array<double, 3>& v_arg, const int type_arg = 0);

    /**
     * Destroy a particle.
     */
    virtual ~Particle();

    /**
     * Get the constant reference to the particle position.
     *
     * @return The particle position.
     */
    const std::array<double, 3>& getX() const;

    /**
     * Get the constant reference to the particle velocity.
     *
     * @return The particle velocity.
     */
    const std::array<double, 3>& getV() const;

    /**
     * Get the constant reference to the force on the particle.
     *
     * @return The force on the particle.
     */
    const std::array<double, 3>& getF() const;

    /**
     * Get the constant reference to the old force on the particle.
     *
     * @return The force on the particle.
     */
    const std::array<double, 3>& getOldF() const;

    /**
     * Get the type of the particle.
     *
     * @return The type of the particle.
     */
    int getType() const;

    /**
     * Set the position of the particle.
     *
     * @param x_new The new position.
     */
    void setX(const std::array<double, 3>& x_new);

    /**
     * Set the velocity of the particle.
     *
     * @param v_new The new velocity.
     */
    void setV(const std::array<double, 3>& v_new);

    /**
     * Set the force affecting the particle.
     *
     * @param f_new The new force.
     */
    void setF(const std::array<double, 3>& f_new);

    /**
     * Set the force that affected the particle the previous iteration.
     *
     * @param old_f_new The new old force.
     */
    void setOldF(const std::array<double, 3>& old_f_new);

    /**
     * Sets the type of a particle
     *
     * @param new_type Type to be given to the particle.
     */
    void setType(const int new_type);

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
 * @param stream THe output stream, to which the particle should be printed. (cout, cerr)
 * @param p The particle that should be printed.
 *
 * @return The output stream.
 */
std::ostream& operator<<(std::ostream& stream, const Particle& p);
