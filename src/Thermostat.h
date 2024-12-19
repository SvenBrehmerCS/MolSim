/**
 * @file
 *
 * @brief Defines the thermostat interface.
 */

#pragma once

#include "Environment.h"
#include "container/ParticleContainer.h"
#include "spdlog/spdlog.h"

#include <math.h>
#include <memory>

/**
 * @class Thermostat
 *
 * @brief Implements a Thermostat to regulate the temperature of the simulation.
 */
class Thermostat {
private:
    /**
     * Number of simulated dimensions.
     */
    int dimensions;

    /**
     * The desired temperature of the system.
     */
    double T_target = std::numeric_limits<double>::signaling_NaN();

    /**
     * The maximal allowed absolute temperature change.
     */
    double max_change = std::numeric_limits<double>::infinity();

    /**
     * The particles to be regulated.
     */
    std::shared_ptr<ParticleContainer> particles;

    /**
     * Activity status of thermostat
     */
    bool active = false;

public:
    /**
     * Create a thermostat regulating certain particles.
     */
    Thermostat() = default;

    /**
     * Standard destructor for a thermostat.
     */
    ~Thermostat() = default;

    /**
     * Get wether the thermostat is active.
     *
     * @return The activity status.
     */
    inline const bool get_active() const { return active; }

    /**
     * Set the dimensions used for energie calculations.
     *
     * @param dimensions The dimensions of the simulation.
     */
    inline void set_dimensions(int dimensions) { this->dimensions = dimensions; }

    /**
     * Set the temperature to regulate to.
     *
     * @param T_target The goal temperature.
     */
    inline void set_T_target(double T_target) { this->T_target = T_target; }

    /**
     * Set the absolute maximum change of temperature per application.
     *
     * @param max_change The maximal temperature change.
     */
    inline void set_max_change(double max_change) { this->max_change = max_change; }

    /**
     * Set the container the thermostat is regulating.
     *
     * @param particles The particle container representing the simulation.
     */
    inline void set_particles(const std::shared_ptr<ParticleContainer>& particles) { this->particles = particles; }

    /**
     * Set the activity status of the thermostat.
     *
     * @param active The activity status.
     */
    inline void set_active(bool active) { this->active = active; }

    /**
     * Regulates the Temperature of the given particle container.
     */
    void regulate_Temperature();

    /**
     * Converts the string into an easy to read string.
     *
     * @return String representation of the Thermostat.
     */
    inline std::string toString() const {
        std::stringstream stream;
        stream << "Thermostat: dimensions: " << dimensions << " T_target: " << T_target << " max_change: " << max_change
               << " particles: " << particles << " active: " << active;
        return stream.str();
    }

    /**
     * An equal operator for testing purposes.
     *
     * @return Boolean representation of equality between the two thermostats.
     */
    inline bool operator==(const Thermostat& other) const {
        return (dimensions == other.dimensions) && (T_target == other.T_target) && (max_change == other.max_change) && (particles == other.particles)
            && (active == other.active);
    }
};

/**
 * Writes a textual representation of the thermostat to the stream.
 *
 * @param stream The stream to be written to.
 * @param t The thermostat to be parsed.
 *
 * @return The stream with the textual representation added.
 */
inline std::ostream& operator<<(std::ostream& stream, const Thermostat& t) {
    stream << t.toString();
    return stream;
}
