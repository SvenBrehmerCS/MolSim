//
// Created by frederik on 11/10/24.
//

#ifndef PARTICLEGENERATOR_H
#define PARTICLEGENERATOR_H
#include "Particle.h"
#include "utils/MaxwellBoltzmannDistribution.h"


class ParticleGenerator {

public:
    ParticleGenerator();

    ~ParticleGenerator();

    void generateCuboid(std::vector<Particle>& particles, int num_particles, std::array<double, 3> x, std::array<double, 3> y, double m,
        std::array<int, 3> N, double h, double b_m, int num_dimensions);
};


#endif // PARTICLEGENERATOR_H
