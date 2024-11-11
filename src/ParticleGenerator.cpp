//
// Created by frederik on 11/10/24.
//

#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator() = default;
ParticleGenerator::~ParticleGenerator() = default;

//* function generates a cuboid, with the given parameters*//

void ParticleGenerator::generateCuboid(std::vector<Particle>& particles, int num_particles, std::array<double, 3> x, std::array<double, 3> v, double m,
    std::array<int, 3> N, double h, double b_m, int dim) {
    int particle_index = num_particles;

    double N1 = N[0];
    double N2 = N[1];
    double N3 = N[2];

    std::array<double, 3> boltz_v;

    for (int i = 0; i < N3; i++) {
        for (int j = 0; j < N2; j++) {
            for (int k = 0; k < N1; k++) {
                // set place
                particles[particle_index].setX({ x[0] + k * h, x[1] + j * h, x[2] + i * h });

                boltz_v = maxwellBoltzmannDistributedVelocity(b_m, dim);

                // set the velocity add the set velocity and boltzman velocity
                particles[particle_index].setV({ v[0] + boltz_v[0], v[1] + boltz_v[1], v[2] + boltz_v[2] });

                // set mass
                particles[particle_index].setM(m);

                particle_index++;
            }
        }
    }
}
