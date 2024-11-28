#pragma once

#include "Container.h"
#include "ParticleContainer.h"

class BoxContainer : public ParticleContainer {
private:
    CellList cells;

public:
    BoxContainer(const double rc, const size_t n_x, const size_t n_y, const size_t n_z);

    BoxContainer(std::vector<Particle> new_particles, const double rc, const size_t n_x, const size_t n_y, const size_t n_z);

    ~BoxContainer();

    virtual void iterate_pairs(std::function<particle_pair_it> iterator);

    void iterate_boundary_cells(std::function<particle_it> iterator);

    void update_positions();

    double getRC();

    std::array<double, 3> get_corner_vector();

    void clear_halo_cells();
};
