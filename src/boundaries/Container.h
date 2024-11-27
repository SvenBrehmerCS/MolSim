#pragma once

#include "Particle.h"

#include <functional>
#include <list>
#include <vector>

typedef void(index_it)(size_t);

typedef void(index_pair_it)(size_t, size_t);

class Cell;

class CellList {
private:
    std::vector<Cell> cells;
    size_t n_x, n_y, n_z;
    double rc;

public:
    CellList();

    ~CellList();

    size_t get_cell_index(size_t x, size_t y, size_t z);

    void iterate_boundary_cells(std::function<index_it> iterator);

    void iterate_halo_cells(std::function<index_it> iterator);

    void loop_cell_pairs(std::function<index_pair_it> iterator, std::vector<Particle> &particles);
};

class Cell {
private:
    // TODO: Replace this list with a non shrinking vector
    std::list<size_t> list;
    CellList* cells;
    size_t x, y, z;

public:
    Cell();

    Cell(CellList* new_cells, size_t new_x, size_t new_y, size_t new_z);

    ~Cell();

    void add_particles(std::list<size_t>& add);

    void remove_particles(std::list<size_t>& remove);

    void iterate_particle_indicies(std::function<index_it> iterator);
};
