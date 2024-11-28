#pragma once

#include "Particle.h"

#include <functional>
#include <list>
#include <vector>

typedef void(index_it)(size_t);

typedef void(index_pair_it)(size_t, size_t);

typedef void(particle_it)(Particle&);

typedef void(particle_pair_it)(Particle&, Particle&);

class Cell;

class CellList {
private:
    std::vector<Cell> cells;
    size_t n_x, n_y, n_z;
    double rc;

public:
    CellList();

    CellList(const double rc, const size_t n_x, const size_t n_y, const size_t n_z);

    ~CellList();

    size_t get_cell_index(size_t x, size_t y, size_t z);

    bool in_cell(std::array<double, 3> pos, size_t x, size_t y, size_t z);

    std::array<double, 3> get_corner_vector();

    void create_list(std::vector<Particle>& particles);

    void iterate_boundary_cells(std::function<index_it> iterator);

    void loop_boundary_cells(std::function<particle_it> iterator, std::vector<Particle>& particles);

    void iterate_halo_cells(std::function<index_it> iterator);

    void clear_halo_cells();

    void loop_cell_pairs(std::function<particle_pair_it> iterator, std::vector<Particle>& particles);

    void move_particles(const std::vector<Particle>& particles);

    double getRC();
};

class Cell {
private:
    std::list<size_t> list;
    CellList* cells;
    size_t x, y, z;

public:
    Cell();

    Cell(CellList* new_cells, size_t new_x, size_t new_y, size_t new_z);

    ~Cell();

    void add_particle(size_t add);

    void remove_particle(size_t remove);

    void iterate_particle_indicies(std::function<index_it> iterator);

    void iterate_particle_pairs(std::function<particle_pair_it> iterator, std::vector<Particle>& particles);

    std::list<size_t>::iterator begin();

    std::list<size_t>::iterator end();

    void clear();
};
