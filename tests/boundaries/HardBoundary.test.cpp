#include <boundaries/HardBoundary.h>
#include <gtest/gtest.h>
#include <physicsCalculator/LJCalculator.h>

// Test the hard boundary in x direction for a single near particle
TEST(HardBoundary, ParticleReflectCorrectly) {
    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ -1.0, 0.0, 2.0 }, { 2.0, 1.0, 1.0 }, 1.0),
        Particle({ 1.0, 12.0, 2.0 }, { 1.0, -3.0, -2.0 }, 1.0),
        Particle({ 3.0, -2.0, 13.0 }, { 2.0, 2.0, -1.0 }, 1.0),
        Particle({ 2.0, 5.0, -2.0 }, { 1.0, 3.0, -2.0 }, 1.0),
    };

    particles[0].setF({ 1.0, 4.0, 2.0 });
    particles[0].setOldF({ -3.0, 2.0, 2.0 });
    particles[1].setF({ 2.0, -4.0, 1.0 });
    particles[1].setOldF({ -6.0, 3.0, -3.0 });
    particles[2].setF({ -2.0, 1.0, 23.0 });
    particles[2].setOldF({ 1.0, 3.0, 2.0 });
    particles[3].setF({ 3.0, 21.0, -2.0 });
    particles[3].setOldF({ -6.0, 1.0, -6.0 });

    std::vector<Particle> reflected = {
        Particle({ 1.0, 0.0, 2.0 }, { -2.0, 1.0, 1.0 }, 1.0),
        Particle({ 1.0, 8.0, 2.0 }, { 1.0, 3.0, -2.0 }, 1.0),
        Particle({ 3.0, 2.0, 7.0 }, { 2.0, -2.0, 1.0 }, 1.0),
        Particle({ 2.0, 5.0, 2.0 }, { 1.0, 3.0, 2.0 }, 1.0),
    };

    reflected[0].setF({ 1.0, 4.0, 2.0 });
    reflected[0].setOldF({ -3.0, 2.0, 2.0 });
    reflected[1].setF({ 2.0, -4.0, 1.0 });
    reflected[1].setOldF({ -6.0, 3.0, -3.0 });
    reflected[2].setF({ -2.0, 1.0, 23.0 });
    reflected[2].setOldF({ 1.0, 3.0, 2.0 });
    reflected[3].setF({ 3.0, 21.0, -2.0 });
    reflected[3].setOldF({ -6.0, 1.0, -6.0 });

    HardBoundary boundary_x_near(0.0, 0);
    HardBoundary boundary_y_near(0.0, 1);
    HardBoundary boundary_z_near(0.0, 2);
    HardBoundary boundary_x_far(10.0, 0);
    HardBoundary boundary_y_far(10.0, 1);
    HardBoundary boundary_z_far(10.0, 2);

    for (size_t i = 0; i < particles.size(); i++) {
        boundary_x_near.postX(particles[i]);
        boundary_y_near.postX(particles[i]);
        boundary_z_near.postX(particles[i]);
        boundary_x_far.postX(particles[i]);
        boundary_y_far.postX(particles[i]);
        boundary_z_far.postX(particles[i]);
        EXPECT_TRUE(particles[i] == reflected[i]) << "The particle " << i << " was not reflected correctly.";
    }
}

// Test that post f does not affect the fore calculation
TEST(HardBoundary, ParticleNoFChange) {
    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 2.0, -1.0, -3.0 }, { 1.0, -4.0, 2.0 }, 0.5, 0),
        Particle({ 7.0, 3.0, 6.0 }, { -2.0, -2.0, 1.0 }, 2.5, 1),
        Particle({ 14.0, 15.0, 16.0 }, { -5.0, 2.0, -4.0 }, 2.0, 1),
        Particle({ 12, 61.0, 23.0 }, { -21.0, 3.0, 2.0 }, 1.0, 2),
    };

    particles[0].setF({ 1.0, 4.0, 2.0 });
    particles[0].setOldF({ -3.0, 2.0, 2.0 });
    particles[1].setF({ 2.0, -4.0, 1.0 });
    particles[1].setOldF({ -6.0, 3.0, -3.0 });
    particles[2].setF({ -2.0, 1.0, 23.0 });
    particles[2].setOldF({ 1.0, 3.0, 2.0 });
    particles[3].setF({ 3.0, 21.0, -2.0 });
    particles[3].setOldF({ -6.0, 1.0, -6.0 });

    const std::vector<Particle> compare = particles;

    // Initialize the simulation environment
    const char* argv[] = {
        "./MolSim",
        "path/to/input.txt",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);
    Environment env;

    ASSERT_NO_THROW(env = Environment(argc, argv));

    // Initialize the Calculator
    physicsCalculator::LJCalculator calc(env, particles, false);

    HardBoundary boundary_x_near(0.0, 0);
    HardBoundary boundary_y_near(0.0, 1);
    HardBoundary boundary_z_near(0.0, 2);
    HardBoundary boundary_x_far(10.0, 0);
    HardBoundary boundary_y_far(13.0, 1);
    HardBoundary boundary_z_far(9.0, 2);

    for (size_t i = 0; i < particles.size(); i++) {
        boundary_x_near.postF(particles[i], calc);
        EXPECT_TRUE(particles[i] == compare[i]) << "The particle " << i << " must not have changed.";
        boundary_y_near.postF(particles[i], calc);
        EXPECT_TRUE(particles[i] == compare[i]) << "The particle " << i << " must not have changed.";
        boundary_z_near.postF(particles[i], calc);
        EXPECT_TRUE(particles[i] == compare[i]) << "The particle " << i << " must not have changed.";
        boundary_x_far.postF(particles[i], calc);
        EXPECT_TRUE(particles[i] == compare[i]) << "The particle " << i << " must not have changed.";
        boundary_y_far.postF(particles[i], calc);
        EXPECT_TRUE(particles[i] == compare[i]) << "The particle " << i << " must not have changed.";
        boundary_z_far.postF(particles[i], calc);
        EXPECT_TRUE(particles[i] == compare[i]) << "The particle " << i << " must not have changed.";
    }
}

// Test that post x does not affect a particle within the domain
TEST(HardBoundary, ParticleInDomain) {
    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 2.0, 1.0, 3.0 }, { 1.0, -4.0, 2.0 }, 0.5, 0),
        Particle({ 7.0, 3.0, 6.0 }, { -2.0, -2.0, 1.0 }, 2.5, 1),
        Particle({ 6.0, 12.0, 8.0 }, { -5.0, 2.0, -4.0 }, 2.0, 1),
        Particle({ 4.0, 1.0, 7.0 }, { -21.0, 3.0, 2.0 }, 1.0, 2),
        Particle({ 0.0, 0.0, 0.0 }, { -1.0, 3.0, -3.0 }, 1.0, 2),
    };

    particles[0].setF({ 1.0, 4.0, 2.0 });
    particles[0].setOldF({ -3.0, 2.0, 2.0 });
    particles[1].setF({ 2.0, -4.0, 1.0 });
    particles[1].setOldF({ -6.0, 3.0, -3.0 });
    particles[2].setF({ -2.0, 1.0, 23.0 });
    particles[2].setOldF({ 1.0, 3.0, 2.0 });
    particles[3].setF({ 3.0, 21.0, -2.0 });
    particles[3].setOldF({ -6.0, 1.0, -6.0 });
    particles[3].setF({ 2.0, 1.0, -1.0 });
    particles[3].setOldF({ -2.0, -1.0, -3.0 });

    const std::vector<Particle> compare = particles;

    // Initialize the simulation environment
    const char* argv[] = {
        "./MolSim",
        "path/to/input.txt",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);
    Environment env;

    ASSERT_NO_THROW(env = Environment(argc, argv));

    // Initialize the Calculator
    physicsCalculator::LJCalculator calc(env, particles, false);

    HardBoundary boundary_x_near(0.0, 0);
    HardBoundary boundary_y_near(0.0, 1);
    HardBoundary boundary_z_near(0.0, 2);
    HardBoundary boundary_x_far(10.0, 0);
    HardBoundary boundary_y_far(13.0, 1);
    HardBoundary boundary_z_far(9.0, 2);

    for (size_t i = 0; i < particles.size(); i++) {
        boundary_x_near.postX(particles[i]);
        EXPECT_TRUE(particles[i] == compare[i]) << "The particle " << i << " must not have changed.";
        boundary_y_near.postX(particles[i]);
        EXPECT_TRUE(particles[i] == compare[i]) << "The particle " << i << " must not have changed.";
        boundary_z_near.postX(particles[i]);
        EXPECT_TRUE(particles[i] == compare[i]) << "The particle " << i << " must not have changed.";
        boundary_x_far.postX(particles[i]);
        EXPECT_TRUE(particles[i] == compare[i]) << "The particle " << i << " must not have changed.";
        boundary_y_far.postX(particles[i]);
        EXPECT_TRUE(particles[i] == compare[i]) << "The particle " << i << " must not have changed.";
        boundary_z_far.postX(particles[i]);
        EXPECT_TRUE(particles[i] == compare[i]) << "The particle " << i << " must not have changed.";
    }
}
