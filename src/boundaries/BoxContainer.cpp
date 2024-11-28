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

void BoxContainer::iterate_boundary_cells(std::function<particle_it> iterator) { cells.loop_boundary_cells(iterator, particles); }

void BoxContainer::update_positions() { cells.move_particles(particles); }

double BoxContainer::getRC() { return cells.getRC(); }

std::array<double, 3> BoxContainer::get_corner_vector() { return cells.get_corner_vector(); }

void BoxContainer::clear_halo_cells() { cells.clear_halo_cells(); }