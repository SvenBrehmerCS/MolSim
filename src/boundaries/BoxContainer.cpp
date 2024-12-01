#include "BoxContainer.h"

BoxContainer::BoxContainer(const double rc, const size_t n_x, const size_t n_y, const size_t n_z)
    : ParticleContainer() { }

BoxContainer::BoxContainer(std::vector<Particle> new_particles, const double rc, const size_t n_x, const size_t n_y, const size_t n_z)
    : ParticleContainer(new_particles) {
    cells = CellList(rc, n_x, n_y, n_z);
    cells.create_list(particles);
};

BoxContainer::~BoxContainer() = default;

void BoxContainer::iterate_pairs(std::function<particle_pair_it> iterator) { cells.loop_cell_pairs(iterator, particles); }

void BoxContainer::update_positions() { cells.create_list(particles); }

double BoxContainer::getRC() { return cells.getRC(); }
