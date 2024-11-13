#pragma once

#include <gtest/gtest.h>
#include <physicsCalculator/GravityCalculator.h>

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
    physicsCalculator::GravityCalculator calc(env, particles, false);

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
    physicsCalculator::GravityCalculator calc(env, particles, false);

    // Initialize the positions to the expected values
    const std::vector<std::array<double, 3>> expected_v = {
        { 6.9625, 1.975, -0.425 },
        { -2.875, 1.625, -1.575 },
        { 1.025, -1.95, 0.35 },
        { 3.08125, 2.04375, -0.45625 },
    };

    // Perform a single calculateV
    ASSERT_NO_THROW(calc.calculateV());

    // Make sure that there are no unwanted changes
    ASSERT_EQ(particles.size(), calc.get_container().size()) << "The number of particles must not change when updating the particles velocity.";

    auto pi = calc.get_container().begin();

    for (size_t i = 0; i < particles.size(); i++) {
        EXPECT_TRUE(pi->getX() == particles[i].getX()) << "The positions must not change when updating the velocity.";
        EXPECT_TRUE(pi->getF() == particles[i].getF()) << "The force must not change when updating the velocity.";
        EXPECT_TRUE(pi->getOldF() == particles[i].getOldF()) << "The old force must not change when updating the velocity.";
        EXPECT_FLOAT_EQ(pi->getM(), particles[i].getM()) << "The mass must not change when updating the velocity.";
        EXPECT_EQ(pi->getType(), particles[i].getType()) << "The type must not change when updating the velocity.";

        // Test if the new position is correct
        EXPECT_LT(ArrayUtils::L2Norm(pi->getV() - expected_v[i]), error_margine)
            << "The velocity was not correct. (expected: " << ArrayUtils::to_string(expected_v[i]) << ", got: " << ArrayUtils::to_string(pi->getV())
            << ")";

        pi++;
    }
}

// Test if update old f works for handcrafted values
TEST(Calculator, UpdateOldF1) {
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
    physicsCalculator::GravityCalculator calc(env, particles, false);

    // Perform a single calculateOldF
    ASSERT_NO_THROW(calc.calculateOldF());

    // Make sure that there are no unwanted changes
    ASSERT_EQ(particles.size(), calc.get_container().size()) << "The number of particles must not change when updating the particles old force.";

    auto pi = calc.get_container().begin();
    const std::array<double, 3> zero_v = { 0.0, 0.0, 0.0 };

    for (size_t i = 0; i < particles.size(); i++) {
        EXPECT_TRUE(pi->getX() == particles[i].getX()) << "The position must not change when updating the old force.";
        EXPECT_TRUE(pi->getV() == particles[i].getV()) << "The velocity must not change when updating the old force.";
        EXPECT_FLOAT_EQ(pi->getM(), particles[i].getM()) << "The mass must not change when updating the old force.";
        EXPECT_EQ(pi->getType(), particles[i].getType()) << "The type must not change when updating the old force.";

        // Test if the new forces are correct
        EXPECT_TRUE(pi->getF() == zero_v) << "The current force should be reset.";
        EXPECT_TRUE(pi->getOldF() == particles[i].getF()) << "The old force should be updated.";

        pi++;
    }
}

// Test if update f works for handcrafted values
TEST(GravityCalculator, UpdateF1) {
    // TODO:
}

// Test if step works for handcrafted values
TEST(GravityCalculator, Step1) {
    // TODO:
}

// Test if step works for an analytical solution using a single moving body
TEST(GravityCalculator, Analytical1) {
    // Set the margine for the maximum floatingpoint error
    const double error_margine = 1E-8;

    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 0.5, 2.0, -31.0 }, { 2.0, -0.5, 2.5 }, 2.0),
    };

    // Initialize the simulation environment
    const char* argv[] = {
        "./MolSim",
        "path/to/input.txt",
        "-delta_t=0.0001",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);
    Environment env;

    ASSERT_NO_THROW(env = Environment(argc, argv));

    // Initialize the Calculator
    physicsCalculator::GravityCalculator calc(env, particles);
    double total_time = 0.0;

    // Perform the steps for 50 time units
    for (size_t i = 0; i <= 500000; i++) {
        // Test that the position is correct
        const std::array<double, 3> expected_pos = { 0.5 + total_time * 2.0, 2.0 + total_time * -0.5, -31.0 + total_time * 2.5 };
        ASSERT_LT(ArrayUtils::L2Norm(calc.get_container()[0].getX() - expected_pos), error_margine)
            << "The calculation diverged at time step " << i << " (" << total_time << ")";

        ASSERT_NO_THROW(calc.step());
        total_time += 0.0001;
    }
}

// Test if step works for an analytical solution using two bodies dancing around a shared center of mass, while the center of mass is moving
TEST(GravityCalculator, Analytical2) {
    // Set the margine for the maximum floatingpoint error
    const double error_margine = 1E-6;

    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 3.0, 1.0, 3.0 }, { 0.1, 1.0, -0.05 }, 4.0),
        Particle({ 1.0, 1.0, 3.0 }, { 0.1, -1.0, -0.05 }, 4.0),
    };

    // Initialize the simulation environment
    const char* argv[] = {
        "./MolSim",
        "path/to/input.txt",
        "-delta_t=0.0001",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);
    Environment env;

    ASSERT_NO_THROW(env = Environment(argc, argv));

    // Initialize the Calculator
    physicsCalculator::GravityCalculator calc(env, particles);
    double total_time = 0.0;
    double time_mod_two_pi = 0.0;
    constexpr double two_pi = static_cast<double>(M_PIl * 2.0L);

    // Perform the steps for 100 time units
    for (size_t i = 0; i <= 1000000; i++) {
        // Test that the position is correct
        const std::array<double, 3> expected_pos_1 = {
            2.0 + std::cos(time_mod_two_pi) + total_time * 0.1,
            1.0 + std::sin(time_mod_two_pi),
            3.0 + total_time * -0.05,
        };

        const std::array<double, 3> expected_pos_2 = {
            2.0 + -std::cos(time_mod_two_pi) + total_time * 0.1,
            1.0 + -std::sin(time_mod_two_pi),
            3.0 + total_time * -0.05,
        };

        ASSERT_LT(ArrayUtils::L2Norm(calc.get_container()[0].getX() - expected_pos_1), error_margine)
            << "The calculation diverged at time step " << i << " (" << total_time << ") for particle 1.";
        ASSERT_LT(ArrayUtils::L2Norm(calc.get_container()[1].getX() - expected_pos_2), error_margine)
            << "The calculation diverged at time step " << i << " (" << total_time << ") for particle 2.";

        ASSERT_NO_THROW(calc.step());
        total_time += 0.0001;
        time_mod_two_pi += 0.0001;

        if (time_mod_two_pi >= two_pi) {
            time_mod_two_pi -= two_pi;
        }
    }
}

// Test if step works for an analytical solution using two bodies dancing around a third body in the center of mass, while the center of mass is
// moving
TEST(GravityCalculator, Analytical3) {
    // Set the margine for the maximum floatingpoint error
    const double error_margine = 1E-4;

    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 2.0, -1.0, 2.0 }, { 0.01, 1.05, -0.05 }, 1.0),
        Particle({ 0.0, -1.0, 2.0 }, { 0.01, -0.95, -0.05 }, 1.0),
        Particle({ 1.0, -1.0, 2.0 }, { 0.01, 0.05, -0.05 }, 0.75),
    };

    // Initialize the simulation environment
    const char* argv[] = {
        "./MolSim",
        "path/to/input.txt",
        "-delta_t=0.0001",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);
    Environment env;

    ASSERT_NO_THROW(env = Environment(argc, argv));

    // Initialize the Calculator
    physicsCalculator::GravityCalculator calc(env, particles);
    double total_time = 0.0;
    double time_mod_two_pi = 0.0;
    constexpr double two_pi = static_cast<double>(M_PIl * 2.0L);

    // Perform the steps for 10 time units
    for (size_t i = 0; i <= 100000; i++) {
        // Test that the position is correct
        const std::array<double, 3> expected_pos_1 = {
            1.0 + std::cos(time_mod_two_pi) + total_time * 0.01,
            -1.0 + std::sin(time_mod_two_pi) + total_time * 0.05,
            2.0 + total_time * -0.05,
        };

        const std::array<double, 3> expected_pos_2 = {
            1.0 + -std::cos(time_mod_two_pi) + total_time * 0.01,
            -1.0 + -std::sin(time_mod_two_pi) + total_time * 0.05,
            2.0 + total_time * -0.05,
        };

        const std::array<double, 3> expected_pos_3 = {
            1.0 + total_time * 0.01,
            -1.0 + total_time * 0.05,
            2.0 + total_time * -0.05,
        };

        ASSERT_LT(ArrayUtils::L2Norm(calc.get_container()[0].getX() - expected_pos_1), error_margine)
            << "The calculation diverged at time step " << i << " (" << total_time << ") for particle 1.";
        ASSERT_LT(ArrayUtils::L2Norm(calc.get_container()[1].getX() - expected_pos_2), error_margine)
            << "The calculation diverged at time step " << i << " (" << total_time << ") for particle 2.";
        ASSERT_LT(ArrayUtils::L2Norm(calc.get_container()[2].getX() - expected_pos_3), error_margine)
            << "The calculation diverged at time step " << i << " (" << total_time << ") for particle 3.";

        ASSERT_NO_THROW(calc.step());
        total_time += 0.0001;
        time_mod_two_pi += 0.0001;

        if (time_mod_two_pi >= two_pi) {
            time_mod_two_pi -= two_pi;
        }
    }
}

// Test if step works for an analytical solution using two bodies dancing around a shared center of mass, while the center of mass is moving. The two
// bodies have different masses and therefore a different circle radius
TEST(GravityCalculator, Analytical4) {
    // Set the margine for the maximum floatingpoint error
    const double error_margine = 1E-6;

    // Initialize the list of particles
    std::vector<Particle> particles = {
        //  1  0  0                 0.1  1  0             18
        // -2  0  0                 0.1 -2  0             9
        Particle({ 1.0, -2.0, 1.0 }, { 1.05, 0.05, 0.01 }, 18),
        Particle({ 1.0, -2.0, -2.0 }, { -1.95, 0.05, 0.01 }, 9),
    };

    // Initialize the simulation environment
    const char* argv[] = {
        "./MolSim",
        "path/to/input.txt",
        "-delta_t=0.0001",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);
    Environment env;

    ASSERT_NO_THROW(env = Environment(argc, argv));

    // Initialize the Calculator
    physicsCalculator::GravityCalculator calc(env, particles);
    double total_time = 0.0;
    double time_mod_two_pi = 0.0;
    constexpr double two_pi = static_cast<double>(M_PIl * 2.0L);

    // Perform the steps for 100 time units
    for (size_t i = 0; i <= 1000000; i++) {
        // Test that the position is correct
        const std::array<double, 3> expected_pos_1 = {
            1.0 + std::sin(time_mod_two_pi) + total_time * 0.05,
            -2.0 + total_time * 0.05,
            0.0 + std::cos(time_mod_two_pi) + total_time * 0.01,
        };

        const std::array<double, 3> expected_pos_2 = {
            1.0 + -2.0 * std::sin(time_mod_two_pi) + total_time * 0.05,
            -2.0 + total_time * 0.05,
            0.0 + -2.0 * std::cos(time_mod_two_pi) + total_time * 0.01,
        };

        ASSERT_LT(ArrayUtils::L2Norm(calc.get_container()[0].getX() - expected_pos_1), error_margine)
            << "The calculation diverged at time step " << i << " (" << total_time << ") for particle 1.";
        ASSERT_LT(ArrayUtils::L2Norm(calc.get_container()[1].getX() - expected_pos_2), error_margine)
            << "The calculation diverged at time step " << i << " (" << total_time << ") for particle 2.";

        ASSERT_NO_THROW(calc.step());
        total_time += 0.0001;
        time_mod_two_pi += 0.0001;

        if (time_mod_two_pi >= two_pi) {
            time_mod_two_pi -= two_pi;
        }
    }
}
