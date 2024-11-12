#pragma once

#include <gtest/gtest.h>
#include <physicsCalculator/LJCalculator.h>

// Test if update x works for handcrafted values
TEST(Calculator, UpdateX2) {
    // Set the margine for the maximum floatingpoint error
    const double error_margine = 1E-9;

    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 1.0, 2.0, 3.0 }, { 2.0, 2.0, 0.0 }, 2.0, 0),
        Particle({ -2.0, -2.0, 1.0 }, { 1.0, -1.0, 2.0 }, 1.0, 1),
        Particle({ -1.0, -1.0, 2.0 }, { -2.0, 0.0, 1.0 }, 1.0, 1),
        Particle({ -2.0, 1.0, 1.0 }, { -1.0, -2.0, -1.0 }, 1.0, 2),
    };

    particles[0].setF({ 1.0, -1.0, 2.0 });
    particles[0].setOldF({ 3.0, 1.0, -1.0 });

    particles[1].setF({ 2.0, 0.0, -1.0 });
    particles[1].setOldF({ 2.0, -1.0, -2.0 });

    particles[2].setF({ 2.0, -1.0, 0.0 });
    particles[2].setOldF({ -1.0, -1.0, 2.0 });

    particles[3].setF({ 0.0, 1.0, 0.0 });
    particles[3].setOldF({ 2.0, -1.0, -2.0 });

    // Initialize the simulation environment
    const char* argv[] = {
        "./MolSim",
        "path/to/input.txt",
        "-delta_t=0.1",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);
    Environment env;

    ASSERT_NO_THROW(env = Environment(argc, argv));

    // Initialize the Calculator
    physicsCalculator::LJCalculator calc(env, particles, false);

    // Initialize the positions to the expected values
    const std::vector<std::array<double, 3>> expected_x = {
        { 1.2025, 2.1975, 3.005 },
        { -1.89, -2.1, 1.195 },
        { -1.19, -1.005, 2.1 },
        { -2.1, 0.805, 0.9 },
    };

    // Perform a single calculateX
    ASSERT_NO_THROW(calc.calculateX());

    // Make sure that there are no unwanted changes
    ASSERT_EQ(particles.size(), calc.get_container().size()) << "The number of particles must not change when updating the particles positions.";

    auto pi = calc.get_container().begin();

    for (size_t i = 0; i < particles.size(); i++) {
        EXPECT_TRUE(pi->getV() == particles[i].getV()) << "The velocity must not change when updating the position.";
        EXPECT_TRUE(pi->getF() == particles[i].getF()) << "The force must not change when updating the position.";
        EXPECT_TRUE(pi->getOldF() == particles[i].getOldF()) << "The old force must not change when updating the position.";
        EXPECT_FLOAT_EQ(pi->getM(), particles[i].getM()) << "The mass must not change when updating the position.";
        EXPECT_EQ(pi->getType(), particles[i].getType()) << "The type must not change when updating the position.";

        // Test if the new position is correct
        EXPECT_LT(ArrayUtils::L2Norm(pi->getX() - expected_x[i]), error_margine)
            << "The position was not correct. (expected: " << ArrayUtils::to_string(expected_x[i]) << ", got: " << ArrayUtils::to_string(pi->getX())
            << ")";

        pi++;
    }
}

// Test if update X works for an empty particle container
TEST(Calculator, UpdateXNo) {
    // Initialize the simulation environment
    const char* argv[] = {
        "./MolSim",
        "path/to/input.txt",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);
    Environment env;

    ASSERT_NO_THROW(env = Environment(argc, argv));

    // Initialize the Calculator
    physicsCalculator::LJCalculator calc(env, {}, false);

    ASSERT_NO_THROW(calc.calculateX());

    EXPECT_EQ(calc.get_container().size(), 0) << "Calculating the position on an empty container should not add a particle.";
}

// Test if update v works for handcrafted values
TEST(Calculator, UpdateV2) {
    // Set the margine for the maximum floatingpoint error
    const double error_margine = 1E-9;

    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ -3.0, 2.0, -1.0 }, { 1.0, 2.0, 1.0 }, 1.0, 0),
        Particle({ 2.0, -1.0, -3.0 }, { 0.0, -1.0, 1.0 }, 2.0, 0),
        Particle({ 2.0, 2.0, -1.0 }, { 1.0, -2.0, -1.0 }, 1.0, 1),
        Particle({ 2.0, 0.0, -2.0 }, { -2.0, 1.0, 1.0 }, 1.0, 1),
    };

    particles[0].setF({ 1.0, -2.0, 2.0 });
    particles[0].setOldF({ -1.0, -2.0, -1.0 });

    particles[1].setF({ 2.0, -1.0, -1.0 });
    particles[1].setOldF({ 1.0, 2.0, -1.0 });

    particles[2].setF({ 2.0, 1.0, 2.0 });
    particles[2].setOldF({ -1.0, 1.0, -1.0 });

    particles[3].setF({ -2.0, -2.0, 1.0 });
    particles[3].setOldF({ 1.0, 2.0, -2.0 });

    // Initialize the simulation environment
    const char* argv[] = {
        "./MolSim",
        "path/to/input.txt",
        "-delta_t=0.1",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);
    Environment env;

    ASSERT_NO_THROW(env = Environment(argc, argv));

    // Initialize the Calculator
    physicsCalculator::LJCalculator calc(env, particles, false);

    // Initialize the positions to the expected values
    const std::vector<std::array<double, 3>> expected_v = {
        { 1.0, 1.8, 1.05 },
        { 0.075, -0.975, 0.95 },
        { 1.05, -1.9, -0.95 },
        { -2.05, 1, 0.95 },
    };

    // Perform a single calculateV
    ASSERT_NO_THROW(calc.calculateV());

    // Make sure that there are no unwanted changes
    ASSERT_EQ(particles.size(), calc.get_container().size()) << "The number of particles must not change when updating the particles positions.";

    auto pi = calc.get_container().begin();

    for (size_t i = 0; i < particles.size(); i++) {
        EXPECT_TRUE(pi->getX() == particles[i].getX()) << "The position must not change when updating the velocity.";
        EXPECT_TRUE(pi->getF() == particles[i].getF()) << "The force must not change when updating the velocity.";
        EXPECT_TRUE(pi->getOldF() == particles[i].getOldF()) << "The old force must not change when updating the velocity.";
        EXPECT_FLOAT_EQ(pi->getM(), particles[i].getM()) << "The mass must not change when updating the velocity.";
        EXPECT_EQ(pi->getType(), particles[i].getType()) << "The type must not change when updating the velocity.";

        // Test if the new velocity is correct
        EXPECT_LT(ArrayUtils::L2Norm(pi->getV() - expected_v[i]), error_margine)
            << "The velocity was not correct. (expected: " << ArrayUtils::to_string(expected_v[i]) << ", got: " << ArrayUtils::to_string(pi->getV())
            << ")";

        pi++;
    }
}

// Test if update V works for an empty particle container
TEST(Calculator, UpdateVNo) {
    // Initialize the simulation environment
    const char* argv[] = {
        "./MolSim",
        "path/to/input.txt",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);
    Environment env;

    ASSERT_NO_THROW(env = Environment(argc, argv));

    // Initialize the Calculator
    physicsCalculator::LJCalculator calc(env, {}, false);

    ASSERT_NO_THROW(calc.calculateV());

    EXPECT_EQ(calc.get_container().size(), 0) << "Calculating the position on an empty container should not add a particle.";
}

// Test if update old f works for handcrafted values
TEST(Calculator, UpdateOldF2) {
    // Set the margine for the maximum floatingpoint error
    const double error_margine = 1E-9;

    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 2.0, -1.0, -3.0 }, { 2.0, -1.0, -2.0 }, 1.0, 0),
        Particle({ 3.0, 3.0, -1.0 }, { 1.0, 1.0, -3.0 }, 2.0, 0),
        Particle({ 1.0, -1.0, 1.0 }, { -2.0, 2.0, -1.0 }, 2.0, 1),
        Particle({ 2.0, -1.0, 2.0 }, { 3.0, -2.0, 2.0 }, 0.5, 1),
    };

    particles[0].setF({ -2.0, 1.0, 1.0 });
    particles[0].setOldF({ -2.0, 2.0, 2.0 });

    particles[1].setF({ 2.0, -3.0, 1.0 });
    particles[1].setOldF({ -3.0, 1.0, -1.0 });

    particles[2].setF({ -2.0, 2.0, 0.0 });
    particles[2].setOldF({ -3.0, 1.0, -1.0 });

    particles[3].setF({ 0.0, 2.0, 1.0 });
    particles[3].setOldF({ -1.0, 2.0, -2.0 });

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

    // Perform a single calculateOldF
    ASSERT_NO_THROW(calc.calculateOldF());

    // Make sure that there are no unwanted changes
    ASSERT_EQ(particles.size(), calc.get_container().size()) << "The number of particles must not change when updating the particles positions.";

    auto pi = calc.get_container().begin();
    const std::array<double, 3> zero_v = { 0.0, 0.0, 0.0 };

    for (size_t i = 0; i < particles.size(); i++) {
        EXPECT_TRUE(pi->getX() == particles[i].getX()) << "The position must not change when updating the old force.";
        EXPECT_TRUE(pi->getV() == particles[i].getV()) << "The velocity must not change when updating the old force.";
        EXPECT_FLOAT_EQ(pi->getM(), particles[i].getM()) << "The mass must not change when updating the old force.";
        EXPECT_EQ(pi->getType(), particles[i].getType()) << "The type must not change when updating the old force.";

        // Test if the new forces are correct
        EXPECT_TRUE(pi->getF() == zero_v) << "The current force should be reseted.";
        EXPECT_TRUE(pi->getOldF() == particles[i].getF()) << "The old force should be updated.";

        pi++;
    }
}

// Test if update f works for handcrafted values
TEST(LJCalculator, UpdateFNo) {
    // TODO:
}

// Test if update f works for handcrafted values
TEST(LJCalculator, UpdateFSingle) {
    // TODO:
}

// Test if update f works for handcrafted values
TEST(LJCalculator, UpdateF1) {
    // TODO:
}

// TODO: Four analytical tests
