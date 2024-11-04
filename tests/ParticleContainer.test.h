#pragma once

#include <ParticleContainer.h>
#include <gtest/gtest.h>

// Test if the particle container constructor works correctly
TEST(ParticleContainerSonstructor, CreateFromVector) {
    std::vector<Particle> pv = {
        Particle({ 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, 1.0, 1),
        Particle({ -1.0, 0.0, 0.0 }, { 0.0, -1.0, 0.0 }, 1.0, 1),
        Particle({ 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, 0.75, 2),
    };

    ParticleContainer pc;

    ASSERT_NO_THROW(pc = ParticleContainer(pv));

    ASSERT_TRUE(pc.get_particles() == pv) << "The particle container must be constructed from the correct particles.";
}
