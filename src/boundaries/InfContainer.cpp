#include "InfContainer.h"

InfContainer::InfContainer()
    : ParticleContainer() { }

InfContainer::InfContainer(const std::vector<Particle>& new_particles)
    : ParticleContainer(new_particles) { }

InfContainer::InfContainer(const std::array<double, 3>& new_domain)
    : ParticleContainer(new_domain) { }

InfContainer::InfContainer(const std::vector<Particle>& new_particles, const std::array<double, 3>& new_domain)
    : ParticleContainer(new_particles, new_domain) { }

InfContainer::~InfContainer() = default;

void InfContainer::iterate_pairs(std::function<particle_pair_it> iterator) {
    for (auto i = begin(); i < end(); i++) {
        for (auto j = i + 1; j < end(); j++) {
            iterator(*i, *j);
        }
    }
}

void InfContainer::update_positions() { }
