#include "HardBoundary.h"

HardBoundary::HardBoundary(const double new_pos, const int new_dim)
    : Boundary(new_pos, new_dim) { }

HardBoundary::~HardBoundary() = default;

void HardBoundary::postF(Particle& particle, physicsCalculator::Calculator& calc) { }

void HardBoundary::postX(Particle& particle) {
    if (pos == 0.0) {
        if (particle.getX()[dim] < pos) {
            std::array<double, 3> v = particle.getV();
            v[dim] *= -1.0;
            particle.setV(v);
            std::array<double, 3> x = particle.getX();
            x[dim] *= -1.0;
            particle.setV(x);
        }
    } else {
        if (particle.getX()[dim] > pos) {
            std::array<double, 3> v = particle.getV();
            v[dim] *= -1.0;
            particle.setV(v);
            std::array<double, 3> x = particle.getX();
            x[dim] = pos * 2.0 - particle.getX()[dim];
            particle.setV(x);
        }
    }
}
