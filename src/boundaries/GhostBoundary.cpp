#include "GhostBoundary.h"

GhostBoundary::GhostBoundary(const double new_pos, const int new_dim)
    : Boundary(new_pos, new_dim) { }

GhostBoundary::~GhostBoundary() = default;

void GhostBoundary::postF(Particle& particle, physicsCalculator::Calculator& calc) {
    const double scale = std::pow(2.0, 1.0 / 6.0);
    const double r = calc.get_env().get_sigma() * scale * 0.5;

    // Create ghost particles if required
    if (pos == 0.0) {
        if (particle.getX()[dim] < r) {
            Vec<double> f = { 0.0, 0.0, 0.0 };
            const double dist = (pos - particle.getX()[dim]) * 2.0;
            f[dim] = -calc.calculateFDist(dist * dist, particle.getType(), particle.getType()) * (particle.getX()[dim]) * 2.0;
            particle.setF(particle.getF() + f);
        }
    } else {
        if (particle.getX()[dim] > pos - r) {
            Vec<double> f = { 0.0, 0.0, 0.0 };
            const double dist = (pos - particle.getX()[dim]) * 2.0;
            f[dim] = calc.calculateFDist(dist * dist, particle.getType(), particle.getType()) * (pos - particle.getX()[dim]) * 2.0;
            particle.setF(particle.getF() + f);
        }
    }
}

void GhostBoundary::postX(Particle& container) { }
