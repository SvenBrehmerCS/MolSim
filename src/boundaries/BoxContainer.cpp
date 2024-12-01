#include <cmath>

#include "BoxContainer.h"

BoxContainer::BoxContainer(std::vector<Particle>& new_particles, const double rc, const std::array<double, 3> &new_domain)
    : ParticleContainer(new_particles, new_domain) {
    cells = CellList(rc, std::ceil(domain[0] / rc), std::ceil(domain[1] / rc), std::ceil(domain[2] / rc));
    cells.create_list(particles);
};

BoxContainer::~BoxContainer() = default;

void BoxContainer::iterate_pairs(std::function<particle_pair_it> iterator) { cells.loop_cell_pairs(iterator, particles); }

void BoxContainer::update_positions() { cells.create_list(particles); }

double BoxContainer::getRC() { return cells.getRC(); }
