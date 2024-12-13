#include "CellList.h"

#include "utils/ArrayUtils.h"

#include <algorithm>
#include <cmath>

#include <spdlog/spdlog.h>

CellList::CellList(const double rc, const std::array<double, 3>& domain) {
    this->rc = rc;
    rc_squ = rc * rc;
    n_x = std::ceil(domain[0] / rc) + 2;
    n_y = std::ceil(domain[1] / rc) + 2;
    n_z = std::ceil(domain[2] / rc) + 2;

    cell_size = {
        domain[0] / static_cast<double>(n_x - 2),
        domain[1] / static_cast<double>(n_y - 2),
        domain[2] / static_cast<double>(n_z - 2),
    };

    cells.resize(this->n_x * this->n_y * this->n_z);
}

void CellList::create_list(const std::vector<Particle>& particles) {
    for (std::list<size_t>& l : cells) {
        l.clear();
    }

    for (size_t i = 0; i < particles.size(); i++) {
        size_t x = std::floor(particles[i].getX()[0] / cell_size[0]) + 1;
        size_t y = std::floor(particles[i].getX()[1] / cell_size[1]) + 1;
        size_t z = std::floor(particles[i].getX()[2] / cell_size[2]) + 1;

        if (x < 0 || x >= n_x) [[unlikely]] {
            SPDLOG_CRITICAL("Tried to add a particle out of bounds.");
            std::exit(EXIT_FAILURE);
        }

        if (y < 0 || y >= n_y) [[unlikely]] {
            SPDLOG_CRITICAL("Tried to add a particle out of bounds.");
            std::exit(EXIT_FAILURE);
        }

        if (z < 0 || z >= n_z) [[unlikely]] {
            SPDLOG_CRITICAL("Tried to add a particle out of bounds.");
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

void CellList::loop_cell_pairs(const std::function<particle_pair_it>& iterator, std::vector<Particle>& particles) {
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

void CellList::loop_halo(const std::function<particle_it>& iterator, std::vector<Particle>& particles) {
    for (size_t i = 0; i < n_x; i++) {
        for (size_t j = 0; j < n_y; j++) {
            for (size_t k : cells[get_cell_index(i, j, 0)]) {
                iterator(particles[k]);
            }

            for (size_t k : cells[get_cell_index(i, j, n_z - 1)]) {
                iterator(particles[k]);
            }
        }
    }

    for (size_t i = 0; i < n_x; i++) {
        for (size_t j = 1; j < n_z - 1; j++) {
            for (size_t k : cells[get_cell_index(i, 0, j)]) {
                iterator(particles[k]);
            }

            for (size_t k : cells[get_cell_index(i, n_y - 1, j)]) {
                iterator(particles[k]);
            }
        }
    }

    for (size_t i = 1; i < n_y - 1; i++) {
        for (size_t j = 1; j < n_z - 1; j++) {
            for (size_t k : cells[get_cell_index(0, i, j)]) {
                iterator(particles[k]);
            }

            for (size_t k : cells[get_cell_index(n_x - 1, i, j)]) {
                iterator(particles[k]);
            }
        }
    }
}

void CellList::loop_boundary(const std::function<particle_it>& iterator, std::vector<Particle>& particles) {
    for (size_t i = 1; i < n_x - 1; i++) {
        for (size_t j = 1; j < n_y - 1; j++) {
            for (size_t k : cells[get_cell_index(i, j, 1)]) {
                iterator(particles[k]);
            }

            for (size_t k : cells[get_cell_index(i, j, n_z - 2)]) {
                iterator(particles[k]);
            }
        }
    }

    for (size_t i = 1; i < n_x - 1; i++) {
        for (size_t j = 2; j < n_z - 2; j++) {
            for (size_t k : cells[get_cell_index(i, 1, j)]) {
                iterator(particles[k]);
            }

            for (size_t k : cells[get_cell_index(i, n_y - 2, j)]) {
                iterator(particles[k]);
            }
        }
    }

    for (size_t i = 2; i < n_y - 2; i++) {
        for (size_t j = 2; j < n_z - 2; j++) {
            for (size_t k : cells[get_cell_index(1, i, j)]) {
                iterator(particles[k]);
            }

            for (size_t k : cells[get_cell_index(n_x - 2, i, j)]) {
                iterator(particles[k]);
            }
        }
    }
}

void CellList::loop_inner(const std::function<particle_it>& iterator, std::vector<Particle>& particles) {
    for (size_t i = 1; i < n_x - 1; i++) {
        for (size_t j = 1; j < n_y - 1; j++) {
            for (size_t k = 1; k < n_z - 1; k++) {
                for (size_t l : cells[get_cell_index(i, j, k)]) {
                    iterator(particles[l]);
                }
            }
        }
    }
}


// TODO:  °°°     Test if the particles are within the correct distance.    °°°
void CellList::loop_xy_pairs(const std::function<particle_pair_it>& iterator, std::vector<Particle>& particles) {
    for (size_t i = 0; i < n_x; i++) {
        for (size_t j = 0; j < n_y; j++) {
            for (size_t k : cells[get_cell_index(i, j, 1)]) {
                // Loop over the cells using the newton optimization
                for (size_t l : cells[get_cell_index(i - 1, j - 1, n_z - 2)]) {
                    iterator(particles[k], particles[l]);
                }

                for (size_t l : cells[get_cell_index(i, j - 1, n_z - 2)]) {
                    iterator(particles[k], particles[l]);
                }

                for (size_t l : cells[get_cell_index(i + 1, j - 1, n_z - 2)]) {
                    iterator(particles[k], particles[l]);
                }

                for (size_t l : cells[get_cell_index(i - 1, j, n_z - 2)]) {
                    iterator(particles[k], particles[l]);
                }

                for (size_t l : cells[get_cell_index(i, j, n_z - 2)]) {
                    iterator(particles[k], particles[l]);
                }

                for (size_t l : cells[get_cell_index(i + 1, j, n_z - 2)]) {
                    iterator(particles[k], particles[l]);
                }

                for (size_t l : cells[get_cell_index(i - 1, j + 1, n_z - 2)]) {
                    iterator(particles[k], particles[l]);
                }

                for (size_t l : cells[get_cell_index(i, j + 1, n_z - 2)]) {
                    iterator(particles[k], particles[l]);
                }

                for (size_t l : cells[get_cell_index(i + 1, j + 1, n_z - 2)]) {
                    iterator(particles[k], particles[l]);
                }
            }
        }
    }
}

void CellList::loop_xz_pairs(const std::function<particle_pair_it>& iterator, std::vector<Particle>& particles) {
    for (size_t i = 0; i < n_x; i++) {
        for (size_t j = 0; j < n_z; j++) {
            for (size_t k : cells[get_cell_index(i, 1, j)]) {
                // Loop over the cells using the newton optimization
                for (size_t l : cells[get_cell_index(i - 1, n_y - 2, j - 1)]) {
                    if (ArrayUtils::L2Norm(particles[k].getX() - particles[l].getX()) <= rc) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(i, n_y - 2, j - 1)]) {
                    if (ArrayUtils::L2Norm(particles[k].getX() - particles[l].getX()) <= rc) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(i + 1, n_y - 2, j - 1)]) {
                    if (ArrayUtils::L2Norm(particles[k].getX() - particles[l].getX()) <= rc) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(i - n_y - 2, 1, j)]) {
                    if (ArrayUtils::L2Norm(particles[k].getX() - particles[l].getX()) <= rc) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(n_y - 2, i, j)]) {
                    if (ArrayUtils::L2Norm(particles[k].getX() - particles[l].getX()) <= rc) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(i + n_y - 2, 1, j)]) {
                    if (ArrayUtils::L2Norm(particles[k].getX() - particles[l].getX()) <= rc) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(i - 1, n_y - 2, j + 1)]) {
                    if (ArrayUtils::L2Norm(particles[k].getX() - particles[l].getX()) <= rc) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(i, n_y - 2, j + 1)]) {
                    if (ArrayUtils::L2Norm(particles[k].getX() - particles[l].getX()) <= rc) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(i + 1, n_y - 2, j + 1)]) {
                    if (ArrayUtils::L2Norm(particles[k].getX() - particles[l].getX()) <= rc) {
                        iterator(particles[k], particles[l]);
                    }
                }
            }
        }
    }
}

void CellList::loop_yz_pairs(const std::function<particle_pair_it>& iterator, std::vector<Particle>& particles) {
    for (size_t i = 0; i < n_y; i++) {
        for (size_t j = 0; j < n_z; j++) {
            for (size_t k : cells[get_cell_index(i, j, 1)]) {
                // Loop over the cells using the newton optimization
                for (size_t l : cells[get_cell_index(n_x - 2, i - 1, j - 1)]) {
                    if (ArrayUtils::L2Norm(particles[k].getX() - particles[l].getX()) <= rc) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(n_x - 2, i, j - 1)]) {
                    if (ArrayUtils::L2Norm(particles[k].getX() - particles[l].getX()) <= rc) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(n_x - 2, i + 1, j - 1)]) {
                    if (ArrayUtils::L2Norm(particles[k].getX() - particles[l].getX()) <= rc) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(n_x - 2, i - 1, j)]) {
                    if (ArrayUtils::L2Norm(particles[k].getX() - particles[l].getX()) <= rc) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(n_x - 2, i, j)]) {
                    if (ArrayUtils::L2Norm(particles[k].getX() - particles[l].getX()) <= rc) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(n_x - 2, i + 1, j)]) {
                    if (ArrayUtils::L2Norm(particles[k].getX() - particles[l].getX()) <= rc) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(n_x - 2, i - 1, j + 1)]) {
                    if (ArrayUtils::L2Norm(particles[k].getX() - particles[l].getX()) <= rc) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(n_x - 2, i, j + 1)]) {
                    if (ArrayUtils::L2Norm(particles[k].getX() - particles[l].getX()) <= rc) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(n_x - 2, i + 1, j + 1)]) {
                    if (ArrayUtils::L2Norm(particles[k].getX() - particles[l].getX()) <= rc) {
                        iterator(particles[k], particles[l]);
                    }
                }
            }
        }
    }
}

void CellList::loop_x_near(const std::function<particle_pair_it>& iterator, std::vector<Particle>& particles) {
    for (size_t i = 1; i < n_x - 1; i++) {
        for (size_t l : cells[get_cell_index(i, 1, 1)]) {
            // Loop over x axis
            for (size_t m : cells[get_cell_index(i - 1, n_y - 2, n_z - 2)]) {
                if (ArrayUtils::L2Norm(particles[l].getX() - particles[m].getX()) <= rc) {
                    iterator(particles[l], particles[m]);
                }
            }

            for (size_t m : cells[get_cell_index(i, n_y - 2, n_z - 2)]) {
                if (ArrayUtils::L2Norm(particles[l].getX() - particles[m].getX()) <= rc) {
                    iterator(particles[l], particles[m]);
                }
            }

            for (size_t m : cells[get_cell_index(i + 1, n_y - 2, n_z - 2)]) {
                if (ArrayUtils::L2Norm(particles[l].getX() - particles[m].getX()) <= rc) {
                    iterator(particles[l], particles[m]);
                }
            }
        }
    }
}

void CellList::loop_x_far(const std::function<particle_pair_it>& iterator, std::vector<Particle>& particles) {
    for (size_t i = 1; i < n_x - 1; i++) {
        for (size_t l : cells[get_cell_index(i, n_y - 2, 1)]) {
            // Loop over shifted x axis
            for (size_t m : cells[get_cell_index(i - 1, 1, n_z - 2)]) {
                if (ArrayUtils::L2Norm(particles[l].getX() - particles[m].getX()) <= rc) {
                    iterator(particles[l], particles[m]);
                }
            }

            for (size_t m : cells[get_cell_index(i, 1, n_z - 2)]) {
                if (ArrayUtils::L2Norm(particles[l].getX() - particles[m].getX()) <= rc) {
                    iterator(particles[l], particles[m]);
                }
            }

            for (size_t m : cells[get_cell_index(i + 1, 1, n_z - 2)]) {
                if (ArrayUtils::L2Norm(particles[l].getX() - particles[m].getX()) <= rc) {
                    iterator(particles[l], particles[m]);
                }
            }
        }
    }
}

void CellList::loop_y_near(const std::function<particle_pair_it>& iterator, std::vector<Particle>& particles) {
    for (size_t i = 1; i < n_y - 1; i++) {
        for (size_t l : cells[get_cell_index(1, i, 1)]) {
            // Loop over y axis
            for (size_t m : cells[get_cell_index(n_x - 2, i - 1, n_z - 2)]) {
                if (ArrayUtils::L2Norm(particles[l].getX() - particles[m].getX()) <= rc) {
                    iterator(particles[l], particles[m]);
                }
            }

            for (size_t m : cells[get_cell_index(n_x - 2, i, n_z - 2)]) {
                if (ArrayUtils::L2Norm(particles[l].getX() - particles[m].getX()) <= rc) {
                    iterator(particles[l], particles[m]);
                }
            }

            for (size_t m : cells[get_cell_index(n_x - 2, i + 1, n_z - 2)]) {
                if (ArrayUtils::L2Norm(particles[l].getX() - particles[m].getX()) <= rc) {
                    iterator(particles[l], particles[m]);
                }
            }
        }
    }
}

void CellList::loop_y_far(const std::function<particle_pair_it>& iterator, std::vector<Particle>& particles) {
    for (size_t i = 1; i < n_y - 1; i++) {
        for (size_t l : cells[get_cell_index(n_x - 2, i, 1)]) {
            // Loop over shifted y axis
            for (size_t m : cells[get_cell_index(1, i - 1, n_z - 2)]) {
                if (ArrayUtils::L2Norm(particles[l].getX() - particles[m].getX()) <= rc) {
                    iterator(particles[l], particles[m]);
                }
            }

            for (size_t m : cells[get_cell_index(1, i, n_z - 2)]) {
                if (ArrayUtils::L2Norm(particles[l].getX() - particles[m].getX()) <= rc) {
                    iterator(particles[l], particles[m]);
                }
            }

            for (size_t m : cells[get_cell_index(1, i + 1, n_z - 2)]) {
                if (ArrayUtils::L2Norm(particles[l].getX() - particles[m].getX()) <= rc) {
                    iterator(particles[l], particles[m]);
                }
            }
        }
    }
}

void CellList::loop_z_near(const std::function<particle_pair_it>& iterator, std::vector<Particle>& particles) {
    for (size_t i = 1; i < n_z - 1; i++) {
        for (size_t l : cells[get_cell_index(1, 1, i)]) {
            // Loop over z axis
            for (size_t m : cells[get_cell_index(n_x - 2, n_y - 2, i - 1)]) {
                if (ArrayUtils::L2Norm(particles[l].getX() - particles[m].getX()) <= rc) {
                    iterator(particles[l], particles[m]);
                }
            }


            for (size_t m : cells[get_cell_index(n_x - 2, n_y - 2, i)]) {
                if (ArrayUtils::L2Norm(particles[l].getX() - particles[m].getX()) <= rc) {
                    iterator(particles[l], particles[m]);
                }
            }

            for (size_t m : cells[get_cell_index(n_x - 2, n_y - 2, i + 1)]) {
                if (ArrayUtils::L2Norm(particles[l].getX() - particles[m].getX()) <= rc) {
                    iterator(particles[l], particles[m]);
                }
            }
        }
    }
}

void CellList::loop_z_far(const std::function<particle_pair_it>& iterator, std::vector<Particle>& particles) {
    for (size_t i = 1; i < n_z - 1; i++) {
        for (size_t l : cells[get_cell_index(n_x - 2, 1, i)]) {
            // Loop over shifted z axis
            for (size_t m : cells[get_cell_index(1, n_y - 2, i - 1)]) {
                if (ArrayUtils::L2Norm(particles[l].getX() - particles[m].getX()) <= rc) {
                    iterator(particles[l], particles[m]);
                }
            }

            for (size_t m : cells[get_cell_index(1, n_y - 2, i)]) {
                if (ArrayUtils::L2Norm(particles[l].getX() - particles[m].getX()) <= rc) {
                    iterator(particles[l], particles[m]);
                }
            }

            for (size_t m : cells[get_cell_index(1, n_y - 2, i + 1)]) {
                if (ArrayUtils::L2Norm(particles[l].getX() - particles[m].getX()) <= rc) {
                    iterator(particles[l], particles[m]);
                }
            }
        }
    }
}

void CellList::loop_origin_corner(const std::function<particle_pair_it>& iterator, std::vector<Particle>& particles) {
    for (size_t l : cells[get_cell_index(1, 1, 1)]) {
        for (size_t m : cells[get_cell_index(n_x - 2, n_y - 2, n_z - 2)]) {
            if (ArrayUtils::L2Norm(particles[l].getX() - particles[m].getX()) <= rc) {
                iterator(particles[l], particles[m]);
            }
        }
    }
}

void CellList::loop_x_corner(const std::function<particle_pair_it>& iterator, std::vector<Particle>& particles) {
    for (size_t l : cells[get_cell_index(n_x - 2, 1, 1)]) {
        for (size_t m : cells[get_cell_index(1, n_y - 2, n_z - 2)]) {
            if (ArrayUtils::L2Norm(particles[l].getX() - particles[m].getX()) <= rc) {
                iterator(particles[l], particles[m]);
            }
        }
    }
}

void CellList::loop_y_corner(const std::function<particle_pair_it>& iterator, std::vector<Particle>& particles) {
    for (size_t l : cells[get_cell_index(1, n_y - 2, 1)]) {
        for (size_t m : cells[get_cell_index(n_x - 2, 1, n_z - 2)]) {
            if (ArrayUtils::L2Norm(particles[l].getX() - particles[m].getX()) <= rc) {
                iterator(particles[l], particles[m]);
            }
        }
    }
}

void CellList::loop_xy_corner(const std::function<particle_pair_it>& iterator, std::vector<Particle>& particles) {
    for (size_t l : cells[get_cell_index(n_x - 2, n_y - 2, 1)]) {
        for (size_t m : cells[get_cell_index(1, 1, n_z - 2)]) {
            if (ArrayUtils::L2Norm(particles[l].getX() - particles[m].getX()) <= rc) {
                iterator(particles[l], particles[m]);
            }
        }
    }
}

double CellList::getRC() { return rc; }
