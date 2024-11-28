#include "InfContainer.h"

InfContainer::InfContainer()
    : ParticleContainer() { }

InfContainer::InfContainer(std::vector<Particle> new_particles)
    : ParticleContainer(new_particles) {};

InfContainer::~InfContainer() = default;


void InfContainer::iterate_pairs(std::function<particle_pair_it> iterator) {
    for (auto i = begin(); i < end(); i++) {
        for (auto j = i + 1; j < end(); j++) {
            iterator(*i, *j);
        }
    }
}
