#include "Container.h"

#include <algorithm>

CellList::CellList() = default;

CellList::~CellList() = default;

size_t CellList::get_cell_index(size_t x, size_t y, size_t z) { return z + y * n_z + x * n_y * n_z; }

void CellList::iterate_boundary_cells(std::function<index_it> iterator) {
    // Loop through the six planes
}

void CellList::iterate_halo_cells(std::function<index_it> iterator) {
    // Loop through the six planes
}

void CellList::loop_cell_pairs(std::function<index_pair_it> iterator, std::vector<Particle>& particles) {
    // Loop through the cells using the indices

    for (size_t i = 0; i < n_x * n_y * n_z; i++) {
        // Loop through the cell itself
    }
}

Cell::Cell() = default;

Cell::Cell(CellList* new_cells, size_t new_x, size_t new_y, size_t new_z) {
    cells = new_cells;
    x = new_x;
    y = new_y;
    z = new_z;
}

Cell::~Cell() = default;

void Cell::add_particles(std::list<size_t>& add) { list.merge(add); }

void Cell::remove_particles(std::list<size_t>& remove) {
    list.remove_if([this, remove](size_t elem) {
        for (size_t r : remove) {
            if (elem == r) {
                return true;
            }
        }

        return false;
    });
}

void Cell::iterate_particle_indicies(std::function<index_it> iterator) {
    for (size_t i : list) {
        iterator(i);
    }
}
