#include "Container.h"

#include "utils/ArrayUtils.h"

#include <algorithm>
#include <cmath>

#include <spdlog/spdlog.h>

CellList::CellList() = default;

CellList::CellList(const double rc, const size_t n_x, const size_t n_y, const size_t n_z) {
    this->rc = rc;
    this->n_x = n_x + 2;
    this->n_y = n_y + 2;
    this->n_z = n_z + 2;
}

CellList::~CellList() = default;

void CellList::create_list(std::vector<Particle>& particles) {
    for (size_t i = 0; i < particles.size(); i++) {
        double cutoff = rc;
        double x = std::modf(particles[i].getX()[0], &cutoff);
        cutoff = rc;
        double y = std::modf(particles[i].getX()[1], &cutoff);
        cutoff = rc;
        double z = std::modf(particles[i].getX()[2], &cutoff);

        // TODO: Is round correct here?
        size_t a = std::round(x), b = std::round(y), c = std::round(z);

        if (a < 0 || a >= n_x) {
            spdlog::critical("Tried to add a particle out of bounds.");
            std::exit(EXIT_FAILURE);
        }

        if (b < 0 || b >= n_y) {
            spdlog::critical("Tried to add a particle out of bounds.");
            std::exit(EXIT_FAILURE);
        }

        if (c < 0 || c >= n_z) {
            spdlog::critical("Tried to add a particle out of bounds.");
            std::exit(EXIT_FAILURE);
        }

        cells[get_cell_index(a, b, c)].add_particle(i);
    }
}

size_t CellList::get_cell_index(size_t x, size_t y, size_t z) { return z + y * n_z + x * n_y * n_z; }

bool CellList::in_cell(std::array<double, 3> pos, size_t x, size_t y, size_t z) {
    if (pos[0] < x * rc || pos[0] >= (x + 1) * rc) {
        return false;
    }

    if (pos[1] < y * rc || pos[1] >= (y + 1) * rc) {
        return false;
    }

    if (pos[2] < z * rc || pos[2] >= (z + 1) * rc) {
        return false;
    }

    return true;
}

std::array<double, 3> CellList::get_corner_vector() {
    return {
        rc * n_x,
        rc * n_y,
        rc * n_z,
    };
}

void CellList::iterate_boundary_cells(std::function<index_it> iterator) {
    // x = 1
    for (size_t j = 1; j < n_y - 1; j++) {
        for (size_t k = 1; k < n_z - 1; k++) {
            cells[get_cell_index(1, j, k)].iterate_particle_indicies(iterator);
        }
    }

    // x = n_x - 1
    for (size_t j = 1; j < n_y - 1; j++) {
        for (size_t k = 1; k < n_z - 1; k++) {
            cells[get_cell_index(n_x - 1, j, k)].iterate_particle_indicies(iterator);
        }
    }

    // y = 1
    for (size_t i = 2; i < n_x - 2; i++) {
        for (size_t k = 1; k < n_z - 1; k++) {
            cells[get_cell_index(i, 1, k)].iterate_particle_indicies(iterator);
        }
    }

    // y = n_y - 1
    for (size_t i = 2; i < n_x - 2; i++) {
        for (size_t k = 1; k < n_z - 1; k++) {
            cells[get_cell_index(i, n_y - 1, k)].iterate_particle_indicies(iterator);
        }
    }

    // z = 1
    for (size_t i = 2; i < n_x - 2; i++) {
        for (size_t j = 2; j < n_y - 2; j++) {
            cells[get_cell_index(i, j, 1)].iterate_particle_indicies(iterator);
        }
    }

    // z = n_z - 1
    for (size_t i = 2; i < n_x - 2; i++) {
        for (size_t j = 2; j < n_y - 2; j++) {
            cells[get_cell_index(i, j, n_z - 1)].iterate_particle_indicies(iterator);
        }
    }
}

void CellList::loop_boundary_cells(std::function<particle_it> iterator, std::vector<Particle>& particles) {
    // x = 1
    for (size_t j = 1; j < n_y - 1; j++) {
        for (size_t k = 1; k < n_z - 1; k++) {
            for (size_t idx : cells[get_cell_index(1, j, k)]) {
                iterator(particles[idx]);
            }
        }
    }

    // x = n_x - 1
    for (size_t j = 1; j < n_y - 1; j++) {
        for (size_t k = 1; k < n_z - 1; k++) {
            for (size_t idx : cells[get_cell_index(n_x - 2, j, k)]) {
                iterator(particles[idx]);
            }
        }
    }


    // y = 1
    for (size_t i = 2; i < n_x - 2; i++) {
        for (size_t k = 1; k < n_z - 1; k++) {
            for (size_t idx : cells[get_cell_index(i, 1, k)]) {
                iterator(particles[idx]);
            }
        }
    }

    // y = n_y - 1
    for (size_t i = 2; i < n_x - 2; i++) {
        for (size_t k = 1; k < n_z - 1; k++) {
            for (size_t idx : cells[get_cell_index(i, n_y - 2, k)]) {
                iterator(particles[idx]);
            }
        }
    }

    // z = 1
    for (size_t i = 2; i < n_x - 2; i++) {
        for (size_t j = 2; j < n_y - 2; j++) {
            for (size_t idx : cells[get_cell_index(i, j, 1)]) {
                iterator(particles[idx]);
            }
        }
    }

    // z = n_z - 1
    for (size_t i = 2; i < n_x - 2; i++) {
        for (size_t j = 2; j < n_y - 2; j++) {
            for (size_t idx : cells[get_cell_index(i, j, n_z - 2)]) {
                iterator(particles[idx]);
            }
        }
    }
}

void CellList::iterate_halo_cells(std::function<index_it> iterator) {
    // x = 0
    for (size_t j = 0; j < n_y; j++) {
        for (size_t k = 0; k < n_z; k++) {
            for (size_t idx : cells[get_cell_index(0, j, k)]) {
                iterator(idx);
            }
        }
    }

    // x = n_x - 1
    for (size_t j = 0; j < n_y; j++) {
        for (size_t k = 0; k < n_z; k++) {
            for (size_t idx : cells[get_cell_index(n_x - 1, j, k)]) {
                iterator(idx);
            }
        }
    }


    // y = 0
    for (size_t i = 1; i < n_x - 1; i++) {
        for (size_t k = 0; k < n_z; k++) {
            for (size_t idx : cells[get_cell_index(i, 0, k)]) {
                iterator(idx);
            }
        }
    }

    // y = n_y - 1
    for (size_t i = 1; i < n_x - 1; i++) {
        for (size_t k = 0; k < n_z; k++) {
            for (size_t idx : cells[get_cell_index(i, n_y - 1, k)]) {
                iterator(idx);
            }
        }
    }

    // z = 0
    for (size_t i = 1; i < n_x - 1; i++) {
        for (size_t j = 1; j < n_y - 1; j++) {
            for (size_t idx : cells[get_cell_index(i, j, 0)]) {
                iterator(idx);
            }
        }
    }

    // z = n_z - 1
    for (size_t i = 1; i < n_x - 1; i++) {
        for (size_t j = 1; j < n_y - 1; j++) {
            for (size_t idx : cells[get_cell_index(i, j, n_z - 1)]) {
                iterator(idx);
            }
        }
    }
}

void CellList::clear_halo_cells() {
    // x = 0
    for (size_t j = 0; j < n_y; j++) {
        for (size_t k = 0; k < n_z; k++) {
            cells[get_cell_index(0, j, k)].clear();
        }
    }

    // x = n_x - 1
    for (size_t j = 0; j < n_y; j++) {
        for (size_t k = 0; k < n_z; k++) {
            cells[get_cell_index(n_x - 1, j, k)].clear();
        }
    }


    // y = 0
    for (size_t i = 1; i < n_x - 1; i++) {
        for (size_t k = 0; k < n_z; k++) {
            cells[get_cell_index(i, 0, k)].clear();
        }
    }

    // y = n_y - 1
    for (size_t i = 1; i < n_x - 1; i++) {
        for (size_t k = 0; k < n_z; k++) {
            cells[get_cell_index(i, n_y - 1, k)].clear();
        }
    }

    // z = 0
    for (size_t i = 1; i < n_x - 1; i++) {
        for (size_t j = 1; j < n_y - 1; j++) {
            cells[get_cell_index(i, j, 0)].clear();
        }
    }

    // z = n_z - 1
    for (size_t i = 1; i < n_x - 1; i++) {
        for (size_t j = 1; j < n_y - 1; j++) {
            cells[get_cell_index(i, j, n_z - 1)].clear();
        }
    }
}

void CellList::loop_cell_pairs(std::function<particle_pair_it> iterator, std::vector<Particle>& particles) {
    // Loop through the cells using the indices, ignore halo cells
    for (size_t i = 1; i < n_x - 1; i++) {
        for (size_t j = 1; j < n_y - 1; j++) {
            for (size_t k = 1; k < n_z - 1; k++) {
                // Loop through the cell itself
                cells[get_cell_index(i, j, k)].iterate_particle_pairs(iterator, particles);

                // Loop through the direct neighbors
                for (size_t l : cells[get_cell_index(i, j, k)]) {
                    for (size_t m : cells[get_cell_index(i + 1, j, k)]) {
                        if (ArrayUtils::L2Norm(particles[l].getX() - particles[m].getX()) <= rc) {
                            iterator(particles[l], particles[m]);
                        }
                    }

                    for (size_t m : cells[get_cell_index(i, j + 1, k)]) {
                        if (ArrayUtils::L2Norm(particles[l].getX() - particles[m].getX()) <= rc) {
                            iterator(particles[l], particles[m]);
                        }
                    }

                    for (size_t m : cells[get_cell_index(i, j, k + 1)]) {
                        if (ArrayUtils::L2Norm(particles[l].getX() - particles[m].getX()) <= rc) {
                            iterator(particles[l], particles[m]);
                        }
                    }

                    // TODO: Don't loop if not required (~50% less loops)
                    for (size_t m : cells[get_cell_index(i + 1, j + 1, k)]) {
                        if (ArrayUtils::L2Norm(particles[l].getX() - particles[m].getX()) <= rc) {
                            iterator(particles[l], particles[m]);
                        }
                    }

                    for (size_t m : cells[get_cell_index(i + 1, j, k + 1)]) {
                        if (ArrayUtils::L2Norm(particles[l].getX() - particles[m].getX()) <= rc) {
                            iterator(particles[l], particles[m]);
                        }
                    }

                    for (size_t m : cells[get_cell_index(i, j + 1, k + 1)]) {
                        if (ArrayUtils::L2Norm(particles[l].getX() - particles[m].getX()) <= rc) {
                            iterator(particles[l], particles[m]);
                        }
                    }
                }
            }
        }
    }
}

void CellList::move_particles(const std::vector<Particle>& particles) {
    for (size_t i = 1; i < n_x - 1; i++) {
        for (size_t j = 1; j < n_y - 1; j++) {
            for (size_t k = 1; k < n_z - 1; k++) {
                for (size_t idx : cells[get_cell_index(i, j, k)]) {
                    if (in_cell(particles[idx].getX(), i, j, k)) {
                        continue;
                    }

                    double cutoff = rc;
                    double x = std::modf(particles[idx].getX()[0], &cutoff);
                    cutoff = rc;
                    double y = std::modf(particles[idx].getX()[1], &cutoff);
                    cutoff = rc;
                    double z = std::modf(particles[idx].getX()[2], &cutoff);

                    // TODO: Is round correct here?
                    size_t a = std::round(x), b = std::round(y), c = std::round(z);

                    if (a < 0 || a >= n_x) {
                        spdlog::critical("Tried to add a particle out of bounds.");
                        std::exit(EXIT_FAILURE);
                    }

                    if (b < 0 || b >= n_y) {
                        spdlog::critical("Tried to add a particle out of bounds.");
                        std::exit(EXIT_FAILURE);
                    }

                    if (c < 0 || c >= n_z) {
                        spdlog::critical("Tried to add a particle out of bounds.");
                        std::exit(EXIT_FAILURE);
                    }

                    cells[get_cell_index(a, b, c)].add_particle(idx);
                    cells[get_cell_index(i, j, k)].remove_particle(idx);
                }
            }
        }
    }
}

double CellList::getRC() { return rc; }

Cell::Cell() = default;

Cell::Cell(CellList* new_cells, size_t new_x, size_t new_y, size_t new_z) {
    cells = new_cells;
    x = new_x;
    y = new_y;
    z = new_z;
}

Cell::~Cell() = default;

void Cell::add_particle(size_t add) { list.push_back(add); }

void Cell::remove_particle(size_t remove) { list.remove(remove); }

void Cell::iterate_particle_indicies(std::function<index_it> iterator) {
    for (size_t i : list) {
        iterator(i);
    }
}

void Cell::iterate_particle_pairs(std::function<particle_pair_it> iterator, std::vector<Particle>& particles) {
    for (auto i = list.begin(); i != list.end(); i++) {
        for (auto j = i; j != list.end(); j++) {
            if (ArrayUtils::L2Norm(particles[*i].getX() - particles[*j].getX()) <= cells->getRC()) {
                iterator(particles[*i], particles[*j]);
            }
        }
    }
}

std::list<size_t>::iterator Cell::begin() { return list.begin(); }

std::list<size_t>::iterator Cell::end() { return list.end(); }

void Cell::clear() { list.clear(); }