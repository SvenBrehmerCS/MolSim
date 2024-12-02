#include "NoBoundary.h"

NoBoundary::NoBoundary(const double new_pos, const int new_dim)
    : Boundary(new_pos, new_dim) { }

void NoBoundary::postF(Particle& particle, physicsCalculator::Calculator& calc) { }

void NoBoundary::postX(Particle& particle) { }
