#include "CellList.h"

#include "utils/ArrayUtils.h"

#include <algorithm>
#include <cmath>

#include <spdlog/spdlog.h>

CellList::CellList(const double rc, const size_t n_x, const size_t n_y, const size_t n_z) {
    this->rc = rc;
    this->n_x = n_x + 2;
    this->n_y = n_y + 2;
    this->n_z = n_z + 2;

    cells.resize(this->n_x * this->n_y * this->n_z);
}

void CellList::create_list(const std::vector<Particle>& particles) {
    for (std::list<size_t>& l : cells) {
        l.clear();
    }

    for (size_t i = 0; i < particles.size(); i++) {
        size_t x = std::floor(particles[i].getX()[0] / rc) + 1;
        size_t y = std::floor(particles[i].getX()[1] / rc) + 1;
        size_t z = std::floor(particles[i].getX()[2] / rc) + 1;

        if (x < 0 || x >= n_x) [[unlikely]] {
            spdlog::critical("Tried to add a particle out of bounds.");
            std::exit(EXIT_FAILURE);
        }

        if (y < 0 || y >= n_y) [[unlikely]] {
            spdlog::critical("Tried to add a particle out of bounds.");
            std::exit(EXIT_FAILURE);
        }

        if (z < 0 || z >= n_z) [[unlikely]] {
            spdlog::critical("Tried to add a particle out of bounds.");
            std::exit(EXIT_FAILURE);
        }

        cells[get_cell_index(x, y, z)].push_back(i);
    }
}

size_t CellList::get_cell_index(const size_t x, const size_t y, const size_t z) { return z + y * n_z + x * n_y * n_z; }

std::array<double, 3> CellList::get_corner_vector() {
    return {
        rc * (n_x - 2),
        rc * (n_y - 2),
        rc * (n_z - 2),
    };
}

void CellList::loop_cell_pairs(std::function<particle_pair_it> iterator, std::vector<Particle>& particles) {
    // Loop through the cells using the indices, ignore halo cells
    for (size_t i = 1; i < n_x - 1; i++) {
        for (size_t j = 1; j < n_y - 1; j++) {
            for (size_t k = 1; k < n_z - 1; k++) {
                const size_t idx = get_cell_index(i, j, k);

                for (std::list<size_t>::iterator l1_it = cells[idx].begin(); l1_it != cells[idx].end(); l1_it++) {
                    std::list<size_t>::iterator l2_it = l1_it;
                    l2_it++;
                    for (; l2_it != cells[idx].end(); l2_it++) {
                        if (ArrayUtils::L2Norm(particles[*l1_it].getX() - particles[*l2_it].getX()) <= rc) {
                            iterator(particles[*l1_it], particles[*l2_it]);
                        }
                    }
                }

                // Loop through the direct neighbors
                for (size_t l : cells[idx]) {
                    Particle& self = particles[l];
                    for (size_t m : cells[get_cell_index(i + 1, j, k)]) {
                        if (ArrayUtils::L2Norm(self.getX() - particles[m].getX()) <= rc) {
                            iterator(self, particles[m]);
                        }
                    }

                    for (size_t m : cells[get_cell_index(i, j + 1, k)]) {
                        if (ArrayUtils::L2Norm(self.getX() - particles[m].getX()) <= rc) {
                            iterator(self, particles[m]);
                        }
                    }

                    for (size_t m : cells[get_cell_index(i, j, k + 1)]) {
                        if (ArrayUtils::L2Norm(self.getX() - particles[m].getX()) <= rc) {
                            iterator(self, particles[m]);
                        }
                    }

                    // Loop through the neighbors with shared edge
                    for (size_t m : cells[get_cell_index(i + 1, j + 1, k)]) {
                        if (ArrayUtils::L2Norm(self.getX() - particles[m].getX()) <= rc) {
                            iterator(self, particles[m]);
                        }
                    }

                    for (size_t m : cells[get_cell_index(i + 1, j, k + 1)]) {
                        if (ArrayUtils::L2Norm(self.getX() - particles[m].getX()) <= rc) {
                            iterator(self, particles[m]);
                        }
                    }

                    for (size_t m : cells[get_cell_index(i, j + 1, k + 1)]) {
                        if (ArrayUtils::L2Norm(self.getX() - particles[m].getX()) <= rc) {
                            iterator(self, particles[m]);
                        }
                    }

                    // Loop through the neighbors with shared corners
                    for (size_t m : cells[get_cell_index(i + 1, j + 1, k + 1)]) {
                        if (ArrayUtils::L2Norm(self.getX() - particles[m].getX()) <= rc) {
                            iterator(self, particles[m]);
                        }
                    }

                    // Loop backwards particles
                    for (size_t m : cells[get_cell_index(i + 1, j - 1, k)]) {
                        if (ArrayUtils::L2Norm(self.getX() - particles[m].getX()) <= rc) {
                            iterator(self, particles[m]);
                        }
                    }

                    for (size_t m : cells[get_cell_index(i + 1, j, k - 1)]) {
                        if (ArrayUtils::L2Norm(self.getX() - particles[m].getX()) <= rc) {
                            iterator(self, particles[m]);
                        }
                    }

                    for (size_t m : cells[get_cell_index(i + 1, j - 1, k - 1)]) {
                        if (ArrayUtils::L2Norm(self.getX() - particles[m].getX()) <= rc) {
                            iterator(self, particles[m]);
                        }
                    }

                    // Loop sidewards particles
                    for (size_t m : cells[get_cell_index(i - 1, j - 1, k + 1)]) {
                        if (ArrayUtils::L2Norm(self.getX() - particles[m].getX()) <= rc) {
                            iterator(self, particles[m]);
                        }
                    }

                    for (size_t m : cells[get_cell_index(i, j - 1, k + 1)]) {
                        if (ArrayUtils::L2Norm(self.getX() - particles[m].getX()) <= rc) {
                            iterator(self, particles[m]);
                        }
                    }

                    for (size_t m : cells[get_cell_index(i + 1, j - 1, k + 1)]) {
                        if (ArrayUtils::L2Norm(self.getX() - particles[m].getX()) <= rc) {
                            iterator(self, particles[m]);
                        }
                    }
                }
            }
        }
    }
}

double CellList::getRC() { return rc; }
