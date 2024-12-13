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
    double T_target = NAN;

    /**
     * The maximal allowed absolute temperature change.
     */
    double max_change = NAN;

    /**
     * The particles to be regulated.
     */
    std::shared_ptr<ParticleContainer> particles;

public:
    /**
     * Create a thermostat regulating certain particles.
     */
    Thermostat(int dimensions, double T_target, double max_change, std::shared_ptr<ParticleContainer> particles);

    /**
     * Standard destructor for a thermostat.
     */
    ~Thermostat() = default;

    /**
     * Regulates the Temperature of the given particle container.
     */
    void regulate_Temperature();
};
