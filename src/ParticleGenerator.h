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
     * @param container the particle container where the particles will be stored.
     * @param num_particles the offset inside the container where the cuboid starts at.
     * @param x the position of the lower left corner of the cuboid.
     * @param v the base velocity of each particle.
     * @param type the type of the particles.
     * @param N the number of particles in each dimension.
     * @param h the distance between particles.
     * @param b_m the brownian motion.
     * @param num_dimensions the number of dimensions.
     **/
    void generateCuboid(ParticleContainer& container, int num_particles, const Vec<double>& x, const Vec<double>& v, int type,
        const std::array<int, 3>& N, double h, double b_m, int num_dimensions);

    /**
     * Generates a disc from the given parameters.
     *
     * @param container The particle container where the particles from the cuboid are stored inside.
     * @param num_particles The offset inside the container where the cuboid starts at.
     * @param center The position of the center particle of the disc.
     * @param velocity Base velocity of the disc.
     * @param type The type of the particles.
     * @param radius The radius of the disc as a number of particles.
     * @param h The distance between particles.
     * @param b_m The brownian motion.
     * @param dim The number of dimensions affected by the brownian motion.
     *
     * @return The number of new particles.
     */
    int generateDisc(ParticleContainer& container, int num_particles, const Vec<double>& center, const Vec<double>& velocity, int type, double radius,
        double h, double b_m, int dim);
};
