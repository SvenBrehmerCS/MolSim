#include "ParticleContainer.h"

ParticleContainer::ParticleContainer() = default;

ParticleContainer::ParticleContainer(const std::vector<Particle>& new_particles) { particles = new_particles; }

ParticleContainer::ParticleContainer(const std::vector<Particle>& new_particles, const std::array<double, 3>& new_domain) {
    particles = new_particles;
    domain = new_domain;
}

ParticleContainer::~ParticleContainer() = default;

Particle& ParticleContainer::operator[](const size_t idx) { return particles[idx]; }

std::vector<Particle>::iterator ParticleContainer::begin() { return particles.begin(); }
std::vector<Particle>::const_iterator ParticleContainer::begin() const { return particles.cbegin(); }

std::vector<Particle>::iterator ParticleContainer::end() { return particles.end(); }
std::vector<Particle>::const_iterator ParticleContainer::end() const { return particles.cend(); }

size_t ParticleContainer::size() const { return particles.size(); }

void ParticleContainer::resize(size_t new_size) { particles.resize(new_size); }

void ParticleContainer::remove_particles_out_of_domain() {
    for (size_t i = 0; i < particles.size(); i++) {
        bool removed = true;

        while (removed) {
            if (particles[i].getX()[0] < 0.0 || particles[i].getX()[0] >= domain[0]) {
                particles[i] = particles[particles.size() - 1];
                particles.resize(particles.size() - 1);
            } else if (particles[i].getX()[1] < 0.0 || particles[i].getX()[1] >= domain[1]) {
                particles[i] = particles[particles.size() - 1];
                particles.resize(particles.size() - 1);
            } else if (particles[i].getX()[2] < 0.0 || particles[i].getX()[2] >= domain[2]) {
                particles[i] = particles[particles.size() - 1];
                particles.resize(particles.size() - 1);
            } else {
                removed = false;
            }

            if (particles.size() == i) {
                return;
            }
        }
    }
}

const std::array<double, 3>& ParticleContainer::get_corner_vector() const { return domain; }
