#include "CellList.h"

#include "utils/Vec.h"

#include <algorithm>
#include <cmath>

#include <spdlog/spdlog.h>

CellList::CellList(const double rc, const Vec<double>& domain) {
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

    dom = domain;
    domain_x = { domain[0], 0.0, 0.0 };
    domain_y = { 0.0, domain[1], 0.0 };
    domain_z = { 0.0, 0.0, domain[2] };
    domain_xy = { domain[0], domain[1], 0.0 };
    domain_xz = { domain[0], 0.0, domain[2] };
    domain_yz = { 0.0, domain[1], domain[2] };
}

void CellList::create_list(const std::vector<Particle>& particles) {
    for (auto& l : cells) {
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

Vec<double> CellList::get_corner_vector() {
    return {
        rc * (n_x - 2),
        rc * (n_y - 2),
        rc * (n_z - 2),
    };
}
void CellList::loop_cell_pairs(const std::function<particle_pair_it>& iterator, std::vector<Particle>& particles) {
    loop_cell_pairs_slices(iterator, particles);
}

void CellList::loop_cell_pairs_slices(const std::function<particle_pair_it>& iterator, std::vector<Particle>& particles) {
#pragma omp parallel for collapse(3)
    for (size_t i = 1; i < n_x - 1; i++) {
        for (size_t j = 1; j < n_y - 1; j++) {
            for (size_t k = 1; k < n_z - 1; k++) {
                const size_t idx = get_cell_index(i, j, k);

                for (auto l1_it = cells[idx].begin(); l1_it != cells[idx].end(); l1_it++) {
                    auto l2_it = l1_it;
                    l2_it++;
                    for (; l2_it != cells[idx].end(); l2_it++) {
                        if ((particles[*l1_it].getX() - particles[*l2_it].getX()).len_squ() <= rc_squ) {
                            iterator(particles[*l1_it], particles[*l2_it]);
                        }
                    }
                }
            }
        }
    }

    // Update
    slice_loop_helper(iterator, particles, 0, 0, X);
    slice_loop_helper(iterator, particles, 0, -1, X);
    slice_loop_helper(iterator, particles, 1, -1, X);
    slice_loop_helper(iterator, particles, 1, 0, X);
    slice_loop_helper(iterator, particles, 1, 1, X);
    slice_loop_helper(iterator, particles, 0, 1, X);
    slice_loop_helper(iterator, particles, 0, 1, Y);
    slice_loop_helper(iterator, particles, 0, -1, Y);
    slice_loop_helper(iterator, particles, -1, -1, Y);
    slice_loop_helper(iterator, particles, -1, 1, Y);
    slice_loop_helper(iterator, particles, -1, 0, Y);
    slice_loop_helper(iterator, particles, 0, 0, Y);
    slice_loop_helper(iterator, particles, 0, 0, Z);
}

void CellList::slice_loop_helper(
    const std::function<particle_pair_it>& iterator, std::vector<Particle>& particles, const int offs_1, const int offs_2, const Direction dir) {
    // Loop through the cells as slices, ignore halo cells
    switch (dir) {
    case X:
#pragma omp parallel for collapse(3)
        for (size_t i = 1; i < n_x; i += 2) {
            for (size_t j = 1; j < n_y - 1; j++) {
                for (size_t k = 1; k < n_z - 1; k++) {

                    for (size_t l : cells[get_cell_index(i - 1, j, k)]) {
                        Particle& self = particles[l];
                        for (size_t m : cells[get_cell_index(i, j + offs_1, k + offs_2)]) {
                            if ((self.getX() - particles[m].getX()).len_squ() <= rc_squ) {
                                iterator(self, particles[m]);
                            }
                        }
                    }
                }
            }
        }
#pragma omp parallel for collapse(3)
        for (size_t i = 2; i < n_x; i += 2) {
            for (size_t j = 1; j < n_y - 1; j++) {
                for (size_t k = 1; k < n_z - 1; k++) {

                    for (size_t l : cells[get_cell_index(i - 1, j, k)]) {
                        Particle& self = particles[l];
                        for (size_t m : cells[get_cell_index(i, j + offs_1, k + offs_2)]) {
                            if ((self.getX() - particles[m].getX()).len_squ() <= rc_squ) {
                                iterator(self, particles[m]);
                            }
                        }
                    }
                }
            }
        }
        break;
    case Y:
#pragma omp parallel for collapse(3)
        for (size_t i = 1; i < n_x - 1; i++) {
            for (size_t j = 1; j < n_y; j += 2) {
                for (size_t k = 1; k < n_z - 1; k++) {

                    for (size_t l : cells[get_cell_index(i, j - 1, k)]) {
                        Particle& self = particles[l];
                        for (size_t m : cells[get_cell_index(i + offs_1, j, k + offs_2)]) {
                            if ((self.getX() - particles[m].getX()).len_squ() <= rc_squ) {
                                iterator(self, particles[m]);
                            }
                        }
                    }
                }
            }
        }
#pragma omp parallel for collapse(3)
        for (size_t i = 1; i < n_x - 1; i++) {
            for (size_t j = 2; j < n_y; j += 2) {
                for (size_t k = 1; k < n_z - 1; k++) {

                    for (size_t l : cells[get_cell_index(i, j - 1, k)]) {
                        Particle& self = particles[l];
                        for (size_t m : cells[get_cell_index(i + offs_1, j, k + offs_2)]) {
                            if ((self.getX() - particles[m].getX()).len_squ() <= rc_squ) {
                                iterator(self, particles[m]);
                            }
                        }
                    }
                }
            }
        }
        break;
    case Z:
#pragma omp parallel for collapse(3)
        for (size_t i = 1; i < n_x - 1; i++) {
            for (size_t j = 1; j < n_y - 1; j++) {
                for (size_t k = 1; k < n_z; k += 2) {

                    for (size_t l : cells[get_cell_index(i, j, k - 1)]) {
                        Particle& self = particles[l];
                        for (size_t m : cells[get_cell_index(i, j, k)]) {
                            if ((self.getX() - particles[m].getX()).len_squ() <= rc_squ) {
                                iterator(self, particles[m]);
                            }
                        }
                    }
                }
            }
        }
#pragma omp parallel for collapse(3)
        for (size_t i = 1; i < n_x - 1; i++) {
            for (size_t j = 1; j < n_y - 1; j++) {
                for (size_t k = 2; k < n_z; k += 2) {

                    for (size_t l : cells[get_cell_index(i, j, k - 1)]) {
                        Particle& self = particles[l];
                        for (size_t m : cells[get_cell_index(i, j, k)]) {
                            if ((self.getX() - particles[m].getX()).len_squ() <= rc_squ) {
                                iterator(self, particles[m]);
                            }
                        }
                    }
                }
            }
        }
        break;
    default:
        SPDLOG_CRITICAL("Tried to create cell slices in a higher dimension.");
        std::exit(EXIT_FAILURE);
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

void CellList::loop_xy_pairs(const std::function<particle_pair_it>& iterator, std::vector<Particle>& particles) {
    for (size_t i = 1; i < n_x - 1; i++) {
        for (size_t j = 1; j < n_y - 1; j++) {
            for (size_t k : cells[get_cell_index(i, j, 1)]) {
                // Loop over the cells using the newton optimization
                for (size_t l : cells[get_cell_index(i - 1, j - 1, n_z - 2)]) {
                    if ((particles[k].getX() - particles[l].getX() + domain_z).len_squ() <= rc_squ) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(i, j - 1, n_z - 2)]) {
                    if ((particles[k].getX() - particles[l].getX() + domain_z).len_squ() <= rc_squ) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(i + 1, j - 1, n_z - 2)]) {
                    if ((particles[k].getX() - particles[l].getX() + domain_z).len_squ() <= rc_squ) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(i - 1, j, n_z - 2)]) {
                    if ((particles[k].getX() - particles[l].getX() + domain_z).len_squ() <= rc_squ) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(i, j, n_z - 2)]) {
                    if ((particles[k].getX() - particles[l].getX() + domain_z).len_squ() <= rc_squ) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(i + 1, j, n_z - 2)]) {
                    if ((particles[k].getX() - particles[l].getX() + domain_z).len_squ() <= rc_squ) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(i - 1, j + 1, n_z - 2)]) {
                    if ((particles[k].getX() - particles[l].getX() + domain_z).len_squ() <= rc_squ) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(i, j + 1, n_z - 2)]) {
                    if ((particles[k].getX() - particles[l].getX() + domain_z).len_squ() <= rc_squ) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(i + 1, j + 1, n_z - 2)]) {
                    if ((particles[k].getX() - particles[l].getX() + domain_z).len_squ() <= rc_squ) {
                        iterator(particles[k], particles[l]);
                    }
                }
            }
        }
    }
}

void CellList::loop_xz_pairs(const std::function<particle_pair_it>& iterator, std::vector<Particle>& particles) {
    for (size_t i = 1; i < n_x - 1; i++) {
        for (size_t j = 1; j < n_z - 1; j++) {
            for (size_t k : cells[get_cell_index(i, 1, j)]) {
                // Loop over the cells using the newton optimization
                for (size_t l : cells[get_cell_index(i - 1, n_y - 2, j - 1)]) {
                    if ((particles[k].getX() - particles[l].getX() + domain_y).len_squ() <= rc_squ) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(i, n_y - 2, j - 1)]) {
                    if ((particles[k].getX() - particles[l].getX() + domain_y).len_squ() <= rc_squ) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(i + 1, n_y - 2, j - 1)]) {
                    if ((particles[k].getX() - particles[l].getX() + domain_y).len_squ() <= rc_squ) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(i - 1, n_y - 2, j)]) {
                    if ((particles[k].getX() - particles[l].getX() + domain_y).len_squ() <= rc_squ) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(i, n_y - 2, j)]) {
                    if ((particles[k].getX() - particles[l].getX() + domain_y).len_squ() <= rc_squ) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(i + 1, n_y - 2, j)]) {
                    if ((particles[k].getX() - particles[l].getX() + domain_y).len_squ() <= rc_squ) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(i - 1, n_y - 2, j + 1)]) {
                    if ((particles[k].getX() - particles[l].getX() + domain_y).len_squ() <= rc_squ) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(i, n_y - 2, j + 1)]) {
                    if ((particles[k].getX() - particles[l].getX() + domain_y).len_squ() <= rc_squ) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(i + 1, n_y - 2, j + 1)]) {
                    if ((particles[k].getX() - particles[l].getX() + domain_y).len_squ() <= rc_squ) {
                        iterator(particles[k], particles[l]);
                    }
                }
            }
        }
    }
}

void CellList::loop_yz_pairs(const std::function<particle_pair_it>& iterator, std::vector<Particle>& particles) {
    for (size_t i = 1; i < n_y - 1; i++) {
        for (size_t j = 1; j < n_z - 1; j++) {
            for (size_t k : cells[get_cell_index(1, i, j)]) {
                // Loop over the cells using the newton optimization
                for (size_t l : cells[get_cell_index(n_x - 2, i - 1, j - 1)]) {
                    if ((particles[k].getX() - particles[l].getX() + domain_x).len_squ() <= rc_squ) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(n_x - 2, i, j - 1)]) {
                    if ((particles[k].getX() - particles[l].getX() + domain_x).len_squ() <= rc_squ) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(n_x - 2, i + 1, j - 1)]) {
                    if ((particles[k].getX() - particles[l].getX() + domain_x).len_squ() <= rc_squ) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(n_x - 2, i - 1, j)]) {
                    if ((particles[k].getX() - particles[l].getX() + domain_x).len_squ() <= rc_squ) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(n_x - 2, i, j)]) {
                    if ((particles[k].getX() - particles[l].getX() + domain_x).len_squ() <= rc_squ) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(n_x - 2, i + 1, j)]) {
                    if ((particles[k].getX() - particles[l].getX() + domain_x).len_squ() <= rc_squ) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(n_x - 2, i - 1, j + 1)]) {
                    if ((particles[k].getX() - particles[l].getX() + domain_x).len_squ() <= rc_squ) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(n_x - 2, i, j + 1)]) {
                    if ((particles[k].getX() - particles[l].getX() + domain_x).len_squ() <= rc_squ) {
                        iterator(particles[k], particles[l]);
                    }
                }

                for (size_t l : cells[get_cell_index(n_x - 2, i + 1, j + 1)]) {
                    if ((particles[k].getX() - particles[l].getX() + domain_x).len_squ() <= rc_squ) {
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
                if ((particles[l].getX() - particles[m].getX() + domain_yz).len_squ() <= rc_squ) {
                    iterator(particles[l], particles[m]);
                }
            }

            for (size_t m : cells[get_cell_index(i, n_y - 2, n_z - 2)]) {
                if ((particles[l].getX() - particles[m].getX() + domain_yz).len_squ() <= rc_squ) {
                    iterator(particles[l], particles[m]);
                }
            }

            for (size_t m : cells[get_cell_index(i + 1, n_y - 2, n_z - 2)]) {
                if ((particles[l].getX() - particles[m].getX() + domain_yz).len_squ() <= rc_squ) {
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
                if ((particles[l].getX() - particles[m].getX() - domain_y + domain_z).len_squ() <= rc_squ) {
                    iterator(particles[l], particles[m]);
                }
            }

            for (size_t m : cells[get_cell_index(i, 1, n_z - 2)]) {
                if ((particles[l].getX() - particles[m].getX() - domain_y + domain_z).len_squ() <= rc_squ) {
                    iterator(particles[l], particles[m]);
                }
            }

            for (size_t m : cells[get_cell_index(i + 1, 1, n_z - 2)]) {
                if ((particles[l].getX() - particles[m].getX() - domain_y + domain_z).len_squ() <= rc_squ) {
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
                if ((particles[l].getX() - particles[m].getX() + domain_xz).len_squ() <= rc_squ) {
                    iterator(particles[l], particles[m]);
                }
            }

            for (size_t m : cells[get_cell_index(n_x - 2, i, n_z - 2)]) {
                if ((particles[l].getX() - particles[m].getX() + domain_xz).len_squ() <= rc_squ) {
                    iterator(particles[l], particles[m]);
                }
            }

            for (size_t m : cells[get_cell_index(n_x - 2, i + 1, n_z - 2)]) {
                if ((particles[l].getX() - particles[m].getX() + domain_xz).len_squ() <= rc_squ) {
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
                if ((particles[l].getX() - particles[m].getX() - domain_x + domain_z).len_squ() <= rc_squ) {
                    iterator(particles[l], particles[m]);
                }
            }

            for (size_t m : cells[get_cell_index(1, i, n_z - 2)]) {
                if ((particles[l].getX() - particles[m].getX() - domain_x + domain_z).len_squ() <= rc_squ) {
                    iterator(particles[l], particles[m]);
                }
            }

            for (size_t m : cells[get_cell_index(1, i + 1, n_z - 2)]) {
                if ((particles[l].getX() - particles[m].getX() - domain_x + domain_z).len_squ() <= rc_squ) {
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
                if ((particles[l].getX() - particles[m].getX() + domain_xy).len_squ() <= rc_squ) {
                    iterator(particles[l], particles[m]);
                }
            }

            for (size_t m : cells[get_cell_index(n_x - 2, n_y - 2, i)]) {
                if ((particles[l].getX() - particles[m].getX() + domain_xy).len_squ() <= rc_squ) {
                    iterator(particles[l], particles[m]);
                }
            }

            for (size_t m : cells[get_cell_index(n_x - 2, n_y - 2, i + 1)]) {
                if ((particles[l].getX() - particles[m].getX() + domain_xy).len_squ() <= rc_squ) {
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
                if ((particles[l].getX() - particles[m].getX() - domain_x + domain_y).len_squ() <= rc_squ) {
                    iterator(particles[l], particles[m]);
                }
            }

            for (size_t m : cells[get_cell_index(1, n_y - 2, i)]) {
                if ((particles[l].getX() - particles[m].getX() - domain_x + domain_y).len_squ() <= rc_squ) {
                    iterator(particles[l], particles[m]);
                }
            }

            for (size_t m : cells[get_cell_index(1, n_y - 2, i + 1)]) {
                if ((particles[l].getX() - particles[m].getX() - domain_x + domain_y).len_squ() <= rc_squ) {
                    iterator(particles[l], particles[m]);
                }
            }
        }
    }
}

void CellList::loop_origin_corner(const std::function<particle_pair_it>& iterator, std::vector<Particle>& particles) {
    for (size_t l : cells[get_cell_index(1, 1, 1)]) {
        for (size_t m : cells[get_cell_index(n_x - 2, n_y - 2, n_z - 2)]) {
            if ((particles[l].getX() - particles[m].getX() + dom).len_squ() <= rc_squ) {
                iterator(particles[l], particles[m]);
            }
        }
    }
}

void CellList::loop_x_corner(const std::function<particle_pair_it>& iterator, std::vector<Particle>& particles) {
    for (size_t l : cells[get_cell_index(n_x - 2, 1, 1)]) {
        for (size_t m : cells[get_cell_index(1, n_y - 2, n_z - 2)]) {
            if ((particles[l].getX() - particles[m].getX() - domain_x + domain_yz).len_squ() <= rc_squ) {
                iterator(particles[l], particles[m]);
            }
        }
    }
}

void CellList::loop_y_corner(const std::function<particle_pair_it>& iterator, std::vector<Particle>& particles) {
    for (size_t l : cells[get_cell_index(1, n_y - 2, 1)]) {
        for (size_t m : cells[get_cell_index(n_x - 2, 1, n_z - 2)]) {
            if ((particles[l].getX() - particles[m].getX() - domain_y + domain_xz).len_squ() <= rc_squ) {
                iterator(particles[l], particles[m]);
            }
        }
    }
}

void CellList::loop_xy_corner(const std::function<particle_pair_it>& iterator, std::vector<Particle>& particles) {
    for (size_t l : cells[get_cell_index(n_x - 2, n_y - 2, 1)]) {
        for (size_t m : cells[get_cell_index(1, 1, n_z - 2)]) {
            if ((particles[l].getX() - particles[m].getX() - domain_xy + domain_z).len_squ() <= rc_squ) {
                iterator(particles[l], particles[m]);
            }
        }
    }
}

double CellList::getRC() { return rc; }
