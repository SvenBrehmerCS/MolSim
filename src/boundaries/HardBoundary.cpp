#include "HardBoundary.h"
#include <spdlog/spdlog.h>

HardBoundary::HardBoundary(const double new_pos, const int new_dim)
    : Boundary(new_pos, new_dim) { }

HardBoundary::~HardBoundary() = default;

void HardBoundary::postF(Particle& particle, physicsCalculator::Calculator& calc) { }

void HardBoundary::postX(Particle& particle) {
    if (pos == 0.0) {
        if (particle.getX()[dim] < pos) {
            Vec<double> v = particle.getV();
            v[dim] *= -1.0;
            particle.setV(v);
            Vec<double> x = particle.getX();
            x[dim] *= -1.0;
            particle.setX(x);
        }
    } else {
        if (particle.getX()[dim] > pos) {
            Vec<double> v = particle.getV();
            v[dim] *= -1.0;
            particle.setV(v);
            Vec<double> x = particle.getX();
            x[dim] = pos * 2.0 - particle.getX()[dim];
            particle.setX(x);
        }
    }
}
