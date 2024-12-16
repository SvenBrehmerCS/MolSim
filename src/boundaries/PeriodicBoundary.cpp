#include "PeriodicBoundary.h"

#include <cmath>

PeriodicBoundary::PeriodicBoundary(const double new_pos, const int new_dim)
    : Boundary(new_pos, new_dim) { }

void PeriodicBoundary::postF(Particle& particle, physicsCalculator::Calculator& calc) { }

void PeriodicBoundary::postX(Particle& particle) {
    if (pos == 0.0) {
        return;
    }

    std::array<double, 3> arr = particle.getX();

    if (arr[dim] < 0.0) {
        arr[dim] = pos + arr[dim];
    } else if (arr[dim] >= pos) {
        arr[dim] = arr[dim] - pos;
    }

    particle.setX(arr);
}