/**
 * @file
 *
 * @brief Handles the main program flow. It initializes the simulation and simulates each step, using the force, position and velocity
 * calculations.
 */

#pragma once

#include "Environment.h"
#include "ParticleContainer.h"

/**
 * Update the forces and old forces experienced by all the particles.
 *
 * @param container The container for the particles.
 * @param env The simulation environment.
 */
void calculateF(ParticleContainer& container, const Environment& env);

/**
 * Update the position of all the particles.
 *
 * @param container The container for the particles.
 * @param env The simulation environment.
 */
void calculateX(ParticleContainer& container, const Environment& env);

/**
 * Update the velocity of all the particles.
 *
 * @param container The container for the particles.
 * @param env The simulation environment.
 */
void calculateV(ParticleContainer& container, const Environment& env);
