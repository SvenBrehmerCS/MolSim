#include <boundaries/PeriodicBoundary.h>
#include <gtest/gtest.h>
#include <physicsCalculator/LJCalculator.h>

// Test that the periodic boundary updates all particle with post x correctly. This test covers particles inside and outside the domain, in positive
// and negative direction. It also covers particles outside in multiple directions.
TEST(PeriodicBoundary, ParticleUpdatedX) {
    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 2.0, 3.0, 11.0 }, { 2.0, 3.0, 2.0 }, 0),
        Particle({ 5.0, 1.0, 1.0 }, { 1.0, -2.0, -2.0 }, 0),
        Particle({ 0.0, 2.0, 11.0 }, { -2.0, 3.0, 2.0 }, 0),
        Particle({ -1.0, 4.0, 4.0 }, { -1.0, 1.0, 0.0 }, 0),
        Particle({ 8.0, 14.0, 7.0 }, { 0.0, -1.0, -2.0 }, 0),
        Particle({ 5.0, 2.0, -4.0 }, { 1.0, -2.0, -2.0 }, 0),
        Particle({ 12.0, 13.0, -4.0 }, { -1.0, 3.0, 1.0 }, 0),
    };

    const std::vector<Particle> compare = {
        Particle({ 2.0, 3.0, 11.0 }, { 2.0, 3.0, 2.0 }, 0),
        Particle({ 5.0, 1.0, 1.0 }, { 1.0, -2.0, -2.0 }, 0),
        Particle({ 0.0, 2.0, 11.0 }, { -2.0, 3.0, 2.0 }, 0),
        Particle({ 15.0, 4.0, 4.0 }, { -1.0, 1.0, 0.0 }, 0),
        Particle({ 8.0, 2.0, 7.0 }, { 0.0, -1.0, -2.0 }, 0),
        Particle({ 5.0, 2.0, 8.0 }, { 1.0, -2.0, -2.0 }, 0),
        Particle({ 12.0, 1.0, 8.0 }, { -1.0, 3.0, 1.0 }, 0),
    };

    // Initialize the simulation environment
    const char* argv[] = {
        "./MolSim",
        "path/to/input.txt",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);
    Environment env;

    ASSERT_NO_THROW(env = Environment(argc, argv));

    // Initialize the Calculator
    physicsCalculator::LJCalculator calc(env, particles, {}, false);

    PeriodicBoundary boundary_x_near(0.0, 0);
    PeriodicBoundary boundary_y_near(0.0, 1);
    PeriodicBoundary boundary_z_near(0.0, 2);
    PeriodicBoundary boundary_x_far(16.0, 0);
    PeriodicBoundary boundary_y_far(12.0, 1);
    PeriodicBoundary boundary_z_far(12.0, 2);

    for (size_t i = 0; i < particles.size(); i++) {
        boundary_x_near.postX(particles[i]);
        boundary_y_near.postX(particles[i]);
        boundary_z_near.postX(particles[i]);
        boundary_x_far.postX(particles[i]);
        boundary_y_far.postX(particles[i]);
        boundary_z_far.postX(particles[i]);
        EXPECT_TRUE(particles[i] == compare[i]) << "Particle " << i << " was not updated correctly.  (expected: " << compare[i]
                                                << ", got: " << particles[i] << ")";
    }
}

// Test the no boundary does not affect a particle with post F
TEST(PeriodicBoundary, ParticleUnaffectedF) {
    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 2.0, -1.0, -3.0 }, { 1.0, -4.0, 2.0 }, 0),
        Particle({ 7.0, 3.0, 6.0 }, { -2.0, -2.0, 1.0 }, 1),
        Particle({ 14.0, 15.0, 16.0 }, { -5.0, 2.0, -4.0 }, 1),
        Particle({ 12, 61.0, 23.0 }, { -21.0, 3.0, 2.0 }, 2),
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
    physicsCalculator::LJCalculator calc(env, particles, {}, false);

    PeriodicBoundary boundary_x_near(0.0, 0);
    PeriodicBoundary boundary_y_near(0.0, 1);
    PeriodicBoundary boundary_z_near(0.0, 2);
    PeriodicBoundary boundary_x_far(10.0, 0);
    PeriodicBoundary boundary_y_far(13.0, 1);
    PeriodicBoundary boundary_z_far(9.0, 2);

    for (size_t i = 0; i < particles.size(); i++) {
        boundary_x_near.postF(particles[i], calc);
        EXPECT_TRUE(particles[i] == compare[i]);
        boundary_y_near.postF(particles[i], calc);
        EXPECT_TRUE(particles[i] == compare[i]);
        boundary_z_near.postF(particles[i], calc);
        EXPECT_TRUE(particles[i] == compare[i]);
        boundary_x_far.postF(particles[i], calc);
        EXPECT_TRUE(particles[i] == compare[i]);
        boundary_y_far.postF(particles[i], calc);
        EXPECT_TRUE(particles[i] == compare[i]);
        boundary_z_far.postF(particles[i], calc);
        EXPECT_TRUE(particles[i] == compare[i]);
    }
}
