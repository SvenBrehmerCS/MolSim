#include "ParticleContainer.h"

ParticleContainer::ParticleContainer() = default;

ParticleContainer::ParticleContainer(const std::vector<Particle>& new_particles) { particles = new_particles; }

ParticleContainer::~ParticleContainer() = default;

Particle& ParticleContainer::operator[](const size_t idx) { return particles[idx]; }

std::vector<Particle>::iterator ParticleContainer::begin() { return particles.begin(); }
std::vector<Particle>::const_iterator ParticleContainer::begin() const { return particles.cbegin(); }


std::vector<Particle>::iterator ParticleContainer::end() { return particles.end(); }
std::vector<Particle>::const_iterator ParticleContainer::end() const { return particles.cend(); }


size_t ParticleContainer::size() const { return particles.size(); }

void ParticleContainer::resize(size_t count) { particles.resize(count); }
