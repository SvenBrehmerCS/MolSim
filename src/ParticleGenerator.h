/**
 * @file
 *
 * @brief In charge of complex particle structure generation.
 */
#pragma once

#include "Particle.h"
#include "container/ParticleContainer.h"
#include "utils/MaxwellBoltzmannDistribution.h"


/**
 * @class ParticleGenerator
 *
 * @brief A class handling the generation of complex particles structures such as cubes.
 */
class ParticleGenerator {
public:
    /** Create a particle Generator**/
    ParticleGenerator();

    /** Particle Generator destructor**/
    ~ParticleGenerator();

    /**
     * Generates the cuboid from the given parameters.
     *
     * @param container the particle container where the particles from the cuboid are stored inside
     * @param num_particles the offset inside the container where the cuboid starts at
     * @param x the position of the lower left corner of the cuboid
     * @param y the base velocity of each particle
     * @param m the mass of the particles
     * @param N the number of particles in each dimension
     * @param h the distance between particles
     * @param b_m the brownian motion
     * @param num_dimensions the number of dimensions
     **/
    void generateCuboid(ParticleContainer& container, int num_particles, std::array<double, 3> x, std::array<double, 3> y, double m,
        std::array<int, 3> N, double h, double b_m, int num_dimensions);

    // TODO: Doc
    int generateDisc(ParticleContainer& container, int num_particles, std::array<double, 3> center, std::array<double, 3> velocity, double mass,
        double radius, double h, double b_m, int dim);
};
