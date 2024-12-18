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

    inline const bool get_active() const { return active; }

    inline void set_dimensions(int dim) { dimensions = dim; }

    inline void set_T_target(double T_target) { this->T_target = T_target; }

    inline void set_max_change(double max_change) { this->max_change = max_change; }

    inline void set_particles(const std::shared_ptr<ParticleContainer>& particles) { this->particles = particles; }

    inline void set_active(bool active) { this->active = active; }

    /**
     * Regulates the Temperature of the given particle container.
     */
    void regulate_Temperature();
};
