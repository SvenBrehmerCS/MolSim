#include "DSContainer.h"

DSContainer::DSContainer()
    : ParticleContainer() { }

DSContainer::DSContainer(const std::vector<Particle>& new_particles, const std::vector<TypeDesc>& new_desc)
    : ParticleContainer(new_particles, new_desc) { }

DSContainer::DSContainer(const std::array<double, 3>& new_domain)
    : ParticleContainer(new_domain) { }

DSContainer::DSContainer(const std::vector<Particle>& new_particles, const std::array<double, 3>& new_domain, const std::vector<TypeDesc>& new_desc)
    : ParticleContainer(new_particles, new_domain, new_desc) { }

DSContainer::~DSContainer() = default;

void DSContainer::iterate_pairs(const std::function<particle_pair_it>& iterator) {
    for (auto i = begin(); i < end(); i++) {
        for (auto j = i + 1; j < end(); j++) {
            iterator(*i, *j);
        }
    }
}

void DSContainer::update_positions() { }
