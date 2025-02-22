#include "ParticleContainer.h"

ParticleContainer::ParticleContainer() = default;

ParticleContainer::ParticleContainer(const std::vector<Particle>& new_particles, const std::vector<TypeDesc>& new_desc)
    : particles { new_particles } {
    build_type_table(new_desc);
}

ParticleContainer::ParticleContainer(const Vec<double>& new_domain)
    : domain { new_domain } { }

ParticleContainer::ParticleContainer(const std::vector<Particle>& new_particles, const Vec<double>& new_domain, const std::vector<TypeDesc>& new_desc)
    : particles { new_particles }
    , domain { new_domain } {
    build_type_table(new_desc);
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

const Vec<double>& ParticleContainer::get_corner_vector() const { return domain; }

std::vector<TypeDesc> ParticleContainer::get_types() const { return types; }

void ParticleContainer::build_type_table(const std::vector<TypeDesc>& new_types) {
    types = new_types;
    type_pairs.resize(types.size() * types.size());

    for (size_t i = 0; i < types.size(); i++) {
        for (size_t j = 0; j < types.size(); j++) {
            type_pairs[i * types.size() + j] = TypePairDesc(
                types[i].get_mass(), types[i].get_sigma(), types[i].get_epsilon(), types[j].get_mass(), types[j].get_sigma(), types[j].get_epsilon());
        }
    }
}

void ParticleContainer::set_particle_type(std::vector<TypeDesc> ptypes) { types = ptypes; }
