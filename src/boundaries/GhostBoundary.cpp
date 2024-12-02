#include "GhostBoundary.h"

GhostBoundary::GhostBoundary(const double new_pos, const int new_dim)
    : Boundary(new_pos, new_dim) { }

void GhostBoundary::postF(Particle& particle, physicsCalculator::Calculator& calc) {
    const double scale = std::pow(2.0, 1.0 / 6.0);
    const double r = calc.get_env().get_sigma() * scale * 0.5;

    // Create ghost particles if required
    if (pos - particle.getX()[dim] < r) {
        std::array<double, 3> f = { 0.0, 0.0, 0.0 };
        f[dim] = calc.calculateFDist(particle.getX()[dim] - pos);
        particle.setF(particle.getF() + f);
    }
}

void GhostBoundary::postX(Particle& container) { }
