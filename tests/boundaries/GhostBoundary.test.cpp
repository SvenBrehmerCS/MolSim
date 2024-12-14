#include <boundaries/GhostBoundary.h>
#include <gtest/gtest.h>
#include <physicsCalculator/LJCalculator.h>

// Test that the forces near the boundaries are calculated correctly
TEST(GhostBoundary, ForceNearBoundary) {
    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 5.0, 5.0, 0.1 }, {}, 0),
        Particle({ 5.0, 9.8, 5.0 }, {}, 0),
    };
    // Initialise the list of type descriptors
    std::vector<TypeDesc> ptypes = {
        TypeDesc { 1.0, 1.0, 5.0, 0.1, 0.0 },
    };

    particles[0].setF({});
    particles[0].setOldF({ 1.0, 3.0, -2.0 });
    particles[1].setF({});
    particles[1].setOldF({ 2.0, -1.0, -1.0 });

    // Initialize the simulation environment
    const char* argv[] = {
        "./MolSim",
        "path/to/input.txt",
        "-sigma=1.0",
        "-epsilon=5.0",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);
    Environment env;

    ASSERT_NO_THROW(env = Environment(argc, argv));

    // Initialize the Calculator
    physicsCalculator::LJCalculator calc(env, particles, ptypes, false);

    // Initialize the boundaries
    GhostBoundary boundary_x_near(0.0, 0);
    GhostBoundary boundary_y_near(0.0, 1);
    GhostBoundary boundary_z_near(0.0, 2);
    GhostBoundary boundary_x_far(10.0, 0);
    GhostBoundary boundary_y_far(10.0, 1);
    GhostBoundary boundary_z_far(10.0, 2);

    const std::vector<std::array<double, 3>> expectedF = {
        { 0.0, 0.0, 292959375000 },
        { 0.0, -35689544.677734375, 0.0 },
    };

    for (size_t i = 0; i < particles.size(); i++) {
        boundary_x_near.postF(particles[i], calc);
        boundary_y_near.postF(particles[i], calc);
        boundary_z_near.postF(particles[i], calc);
        boundary_x_far.postF(particles[i], calc);
        boundary_y_far.postF(particles[i], calc);
        boundary_z_far.postF(particles[i], calc);
        EXPECT_LT(ArrayUtils::L2Norm(particles[i].getF() - expectedF[i]), 1E-3) << "The particle " << i << " must have the correct force.";
    }
}

// Test that particles that should not be affected are not affected
TEST(GhostBoundary, ParticleAtCenter) {
    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 2.0, 1.0, 2.0 }, { 3.0, -2.0, 2.0 }, 0),
        Particle({ 7.0, 2.0, 4.0 }, { -1.0, 2.0, -1.0 }, 1),
        Particle({ 13.0, 15.0, 16.0 }, { -1.0, 2.0, -1.0 }, 1),
        Particle({ 13, 7.0, 9.0 }, { -1.0, 2.0, -1.0 }, 1),
    };
    // Initialise the list of type descriptors
    std::vector<TypeDesc> ptypes = {
        TypeDesc { 0.5, 1.0, 5.0, 0.1, 0.0 },
        TypeDesc { 2.5, 1.0, 5.0, 0.1, 0.0 },
    };

    particles[0].setF({ 1.0, 4.0, -1.0 });
    particles[0].setOldF({ -2.0, 2.0, 2.0 });
    particles[1].setF({ 2.0, -4.0, 1.0 });
    particles[1].setOldF({ -6.0, 2.0, 3.0 });
    particles[2].setF({ -2.0, 1.0, 2.0 });
    particles[2].setOldF({ 1.0, 3.0, 2.0 });
    particles[3].setF({ 3.0, 2.0, -2.0 });
    particles[3].setOldF({ 6.0, 5.0, -6.0 });

    const std::vector<Particle> compare = particles;

    // Initialize the simulation environment
    const char* argv[] = {
        "./MolSim",
        "path/to/input.txt",
        "-sigma=1.0",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);
    Environment env;

    ASSERT_NO_THROW(env = Environment(argc, argv));

    // Initialize the Calculator
    physicsCalculator::LJCalculator calc(env, particles, ptypes, false);


    GhostBoundary boundary_x_near(0.0, 0);
    GhostBoundary boundary_y_near(0.0, 1);
    GhostBoundary boundary_z_near(0.0, 2);
    GhostBoundary boundary_x_far(20.0, 0);
    GhostBoundary boundary_y_far(20.0, 1);
    GhostBoundary boundary_z_far(20.0, 2);

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

// Test the no boundary does not affect a particle with post x
TEST(GhostBoundary, ParticleUnaffectedX) {
    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 2.0, -1.0, -2.0 }, { 3.0, -2.0, 2.0 }, 0),
        Particle({ 7.0, 2.0, 4.0 }, { -1.0, 2.0, -1.0 }, 1),
        Particle({ 13.0, 15.0, 16.0 }, { -1.0, 2.0, -1.0 }, 1),
        Particle({ 13, 62.0, 22.0 }, { -1.0, 2.0, -1.0 }, 1),
    };
    // Initialise the list of type descriptors
    std::vector<TypeDesc> ptypes = {
        TypeDesc { 0.5, 1.0, 5.0, 0.1, 0.0 },
        TypeDesc { 2.5, 1.0, 5.0, 0.1, 0.0 },
    };

    particles[0].setF({ 1.0, 4.0, -1.0 });
    particles[0].setOldF({ -2.0, 2.0, 2.0 });
    particles[1].setF({ 2.0, -4.0, 1.0 });
    particles[1].setOldF({ -6.0, 2.0, 3.0 });
    particles[2].setF({ -2.0, 1.0, 2.0 });
    particles[2].setOldF({ 1.0, 3.0, 2.0 });
    particles[3].setF({ 3.0, 2.0, -2.0 });
    particles[3].setOldF({ 6.0, 5.0, -6.0 });

    const std::vector<Particle> compare = particles;

    GhostBoundary boundary_x_near(0.0, 0);
    GhostBoundary boundary_y_near(0.0, 1);
    GhostBoundary boundary_z_near(0.0, 2);
    GhostBoundary boundary_x_far(10.0, 0);
    GhostBoundary boundary_y_far(13.0, 1);
    GhostBoundary boundary_z_far(9.0, 2);

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
