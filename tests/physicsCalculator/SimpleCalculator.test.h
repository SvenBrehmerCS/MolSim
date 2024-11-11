#pragma once

#include <gtest/gtest.h>
#include <physicsCalculator/SimpleCalculator.h>

// Test if update x works for handcrafted values
TEST(Calculator, UpdateX1) {
    // Set the margine for the maximum floatingpoint error
    const double error_margine = 1E-9;

    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 0.1, 1.3, -2.0 }, { 7.0, 2.0, -0.5 }, 2.0, 2),
        Particle({ 2.5, -3.0, -0.5 }, { -3.0, 1.5, -1.5 }, 1.0, 2),
        Particle({ -2.0, 3.0, 3.5 }, { 1.0, -2.0, 0.5 }, 1.0, 1),
        Particle({ 0.1, -1.5, 2.0 }, { 3.0, 2.0, -0.5 }, 4.0, 0),
    };

    particles[0].setF({ -1.0, 2.0, 1.0 });
    particles[0].setOldF({ -0.5, -3.0, 2.0 });

    particles[1].setF({ 2.0, 1.0, 0.5 });
    particles[1].setOldF({ 0.5, 1.5, -2.0 });

    particles[2].setF({ 2.0, 2.5, -1.0 });
    particles[2].setOldF({ -1.5, -1.5, -2.0 });

    particles[3].setF({ 3.5, 1.5, 2.0 });
    particles[3].setOldF({ 3.0, 2.0, 1.5 });

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
    physicsCalculator::SimpleCalculator calc(env, particles, false);

    // Initialize the positions to the expected values
    const std::vector<std::array<double, 3>> expected_x = {
        { 0.7975, 1.505, -2.0475 },
        { 2.21, -2.845, -0.6475 },
        { -1.89, 2.8125, 3.545 },
        { 0.404375, -1.298125, 1.9525 },
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

// Test if update v works for handcrafted values
TEST(Calculator, UpdateV1) {
    // TODO:
}

// Test if update old f works for handcrafted values
TEST(Calculator, UpdateOldF1) {
    // TODO:
}

// Test if update f works for handcrafted values
TEST(SimpleCalculator, UpdateF1) {
    // TODO:
}

// Test if step works for handcrafted values
TEST(SimpleCalculator, Step1) {
    // TODO:
}

// Test if step works for an analytical solution
TEST(SimpleCalculator, Analytical1) {
    // TODO:
}
