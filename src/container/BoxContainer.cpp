#include <cmath>

#include "BoxContainer.h"

BoxContainer::BoxContainer(const double rc, const Vec<double>& new_domain, const UpdateStrategy strat)
    : ParticleContainer(new_domain) {
    cells = CellList(rc, domain, strat);
    cells.create_list(particles);
};

BoxContainer::BoxContainer(const std::vector<Particle>& new_particles, const double rc, const Vec<double>& new_domain,
    const std::vector<TypeDesc>& new_desc, const UpdateStrategy strat)
    : ParticleContainer(new_particles, new_domain, new_desc) {
    cells = CellList(rc, domain, strat);
    cells.create_list(particles);
};

BoxContainer::~BoxContainer() = default;

void BoxContainer::iterate_pairs(const std::function<particle_pair_it>& iterator) { cells.loop_cell_pairs(iterator, particles); }

void BoxContainer::iterate_xy_pairs(const std::function<particle_pair_it>& iterator) { cells.loop_xy_pairs(iterator, particles); }

void BoxContainer::iterate_xz_pairs(const std::function<particle_pair_it>& iterator) { cells.loop_xz_pairs(iterator, particles); }

void BoxContainer::iterate_yz_pairs(const std::function<particle_pair_it>& iterator) { cells.loop_yz_pairs(iterator, particles); }

void BoxContainer::loop_x_near(const std::function<particle_pair_it>& iterator) { cells.loop_x_near(iterator, particles); }

void BoxContainer::loop_x_far(const std::function<particle_pair_it>& iterator) { cells.loop_x_far(iterator, particles); }

void BoxContainer::loop_y_near(const std::function<particle_pair_it>& iterator) { cells.loop_y_near(iterator, particles); }

void BoxContainer::loop_y_far(const std::function<particle_pair_it>& iterator) { cells.loop_y_far(iterator, particles); }

void BoxContainer::loop_z_near(const std::function<particle_pair_it>& iterator) { cells.loop_z_near(iterator, particles); }

void BoxContainer::loop_z_far(const std::function<particle_pair_it>& iterator) { cells.loop_z_far(iterator, particles); }

void BoxContainer::loop_origin_corner(const std::function<particle_pair_it>& iterator) { cells.loop_origin_corner(iterator, particles); }

void BoxContainer::loop_x_corner(const std::function<particle_pair_it>& iterator) { cells.loop_x_corner(iterator, particles); }

void BoxContainer::loop_y_corner(const std::function<particle_pair_it>& iterator) { cells.loop_y_corner(iterator, particles); }

void BoxContainer::loop_xy_corner(const std::function<particle_pair_it>& iterator) { cells.loop_xy_corner(iterator, particles); }

void BoxContainer::update_positions() { cells.create_list(particles); }

double BoxContainer::getRC() { return cells.getRC(); }
