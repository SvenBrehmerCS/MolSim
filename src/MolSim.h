/**
 * MolSim.h
 *
 * This file povides the definitions required in MolSim.cpp
 */

#pragma once

#include "Environment.h"

/**
 * Update the forces and old forces experienced by all the particles.
 *
 * @param env The simulation environment.
 */
void calculateF(const Environment& env);

/**
 * Update the position of all the particles.
 *
 * @param env The simulation environment.
 */
void calculateX(const Environment& env);

/**
 * Update the velocity of all the particles.
 *
 * @param env The simulation environment.
 */
void calculateV(const Environment& env);
