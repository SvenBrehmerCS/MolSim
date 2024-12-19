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

    // TODO
    // inline const bool get_dimensions() const { return dimensions; }

    // inline const bool get_T_target() const { return T_target; }

    // inline const bool get_max_change() const { return max_change; }

    inline const bool get_active() const { return active; }

    inline void set_dimensions(int dimensions) { this->dimensions = dimensions; }

    inline void set_T_target(double T_target) { this->T_target = T_target; }

    inline void set_max_change(double max_change) { this->max_change = max_change; }

    inline void set_particles(const std::shared_ptr<ParticleContainer>& particles) { this->particles = particles; }

    inline void set_active(bool active) { this->active = active; }

    /**
     * Regulates the Temperature of the given particle container.
     */
    void regulate_Temperature();

    inline std::string toString() const {
        std::stringstream stream;
        stream << "Thermostat: dimensions: " << dimensions << " T_target: " << T_target << " max_change: " << max_change
               << " particles: " << particles << " active: " << active;
        return stream.str();
    }

    inline bool operator==(const Thermostat& other) const {
        return (dimensions == other.dimensions) && (T_target == other.T_target) && (max_change == other.max_change) && (particles == other.particles)
            && (active == other.active);
    }
};

inline std::ostream& operator<<(std::ostream& stream, const Thermostat& t) {
    stream << t.toString();
    return stream;
}
