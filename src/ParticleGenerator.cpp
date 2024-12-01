
#include "ParticleGenerator.h"
#include "utils/ArrayUtils.h"

ParticleGenerator::ParticleGenerator() = default;
ParticleGenerator::~ParticleGenerator() = default;

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

int ParticleGenerator::generateDisc(ParticleContainer& container, int num_particles, std::array<double, 3> center, std::array<double, 3> velocity,
    double mass, double radius, double h, double b_m, int dim) {
    int num_particles_added = 0;

    double radius_distance = radius * h;
    std::array<double, 3> boltz_v;

    auto particle = container.begin();
    particle += num_particles;

    for (double x = -radius_distance; x <= radius_distance; x += h) {
        for (double y = -radius_distance; y <= radius_distance; y += h) {
            if (x * x + y * y <= radius_distance * radius_distance) {
                particle->setX({ center[0] + x, center[1] + y, center[2] });

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
