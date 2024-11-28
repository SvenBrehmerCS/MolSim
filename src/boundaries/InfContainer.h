#pragma once

#include "ParticleContainer.h"

class InfContainer : public ParticleContainer {
private:
public:
    InfContainer();

    InfContainer(std::vector<Particle> new_particles);

    ~InfContainer();

    virtual void iterate_pairs(std::function<particle_pair_it> iterator);
};
