
#include "ParticleGenerator.h"
#include "utils/ArrayUtils.h"

ParticleGenerator::ParticleGenerator() = default;
ParticleGenerator::~ParticleGenerator() = default;

/**
 * Generate a cuboid
 * @param container
 * @param num_particles
 * @param x
 * @param v
 * @param m
 * @param N
 * @param h
 * @param b_m
 * @param dim
 */
void ParticleGenerator::generateCuboid(ParticleContainer& container, int num_particles, std::array<double, 3> x, std::array<double, 3> v, double m,
    std::array<int, 3> N, double h, double b_m, int dim) {

    int N1 = N[0];
    int N2 = N[1];
    int N3 = N[2];

    std::array<double, 3> boltz_v;

    auto particle = container.begin();
    particle += num_particles;

    for (int i = 0; i < N3; i++) {
        for (int j = 0; j < N2; j++) {
            for (int k = 0; k < N1; k++) {
                // set place
                particle->setX({ x[0] + k * h, x[1] + j * h, x[2] + i * h });

                boltz_v = maxwellBoltzmannDistributedVelocity(b_m, dim);

                // set the velocity add the set velocity and boltzmann velocity
                particle->setV(v + boltz_v);

                // set mass
                particle->setM(m);

                particle++;
            }
        }
    }
}

/**
 *
 * Generate a disc
 *
 * @param container
 * @param num_particles
 * @param center
 * @param velocity
 * @param mass
 * @param radius
 * @param h
 * @param b_m
 * @param dim
 * @return
 */
int ParticleGenerator::generateDisc(ParticleContainer& container, int num_particles, std::array<double, 3> center, std::array<double, 3> velocity,
    double mass, double radius, double h, double b_m, int dim) {
    int num_particles_added = 0;

    double radius_distance = radius * h;
    std::array<double, 3> boltz_v;

    auto particle = container.begin();
    particle += num_particles;

    double c0 = center[0];
    double c1 = center[1];
    double c2 = center[2];

    // Iteration over only one quadrant of the disc with the rest of the particles generated by reflection
    for (double x_off = 0; x_off <= radius_distance; x_off += h) {
        double max_y_offset = sqrt(radius_distance * radius_distance - x_off * x_off);
        for (double y_off = 0; y_off <= max_y_offset; y_off += h) {
            // First sector (up right)
            particle->setX({ c0 + x_off, c1 + y_off, c2 });
            boltz_v = maxwellBoltzmannDistributedVelocity(b_m, dim);
            particle->setV(velocity + boltz_v);
            particle->setM(mass);
            ++particle;
            ++num_particles_added;

            // In case of x != 0 => reflection to down right
            if (x_off != 0) {
                particle->setX({ c0 - x_off, c1 + y_off, c2 });
                boltz_v = maxwellBoltzmannDistributedVelocity(b_m, dim);
                particle->setV(velocity + boltz_v);
                particle->setM(mass);
                ++particle;
                ++num_particles_added;
            }

            // In case of y != 0 => reflection to up left
            if (y_off != 0) {
                particle->setX({ c0 + x_off, c1 - y_off, c2 });
                boltz_v = maxwellBoltzmannDistributedVelocity(b_m, dim);
                particle->setV(velocity + boltz_v);
                particle->setM(mass);
                ++particle;
                ++num_particles_added;
            }

            // In case of both => reflection to down left
            if (x_off != 0 && y_off != 0) {
                particle->setX({ c0 - x_off, c1 - y_off, c2 });
                boltz_v = maxwellBoltzmannDistributedVelocity(b_m, dim);
                particle->setV(velocity + boltz_v);
                particle->setM(mass);
                ++particle;
                ++num_particles_added;
            }
        }
    }
    return num_particles_added;
}