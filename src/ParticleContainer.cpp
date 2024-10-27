#include "ParticleContainer.h"

ParticleContainer::ParticleContainer() = default;

ParticleContainer::ParticleContainer(std::vector<Particle>& new_particles) { particles = new_particles; }

ParticleContainer::~ParticleContainer() = default;

std::vector<Particle>& ParticleContainer::get_particles() { return particles; }
