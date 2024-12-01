
#include <gtest/gtest.h>
#include <physicsCalculator/LJCalculator.h>

// Test if update x works for handcrafted values
TEST(Calculator, UpdateX2) {
    // Set the margin for the maximum floatingpoint error
    const double error_margin = 1E-9;

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
        EXPECT_LT(ArrayUtils::L2Norm(pi->getX() - expected_x[i]), error_margin)
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
    // Set the margin for the maximum floatingpoint error
    const double error_margin = 1E-9;

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
    ASSERT_EQ(particles.size(), calc.get_container().size()) << "The number of particles must not change when updating the particles velocity.";

    auto pi = calc.get_container().begin();

    for (size_t i = 0; i < particles.size(); i++) {
        EXPECT_TRUE(pi->getX() == particles[i].getX()) << "The position must not change when updating the velocity.";
        EXPECT_TRUE(pi->getF() == particles[i].getF()) << "The force must not change when updating the velocity.";
        EXPECT_TRUE(pi->getOldF() == particles[i].getOldF()) << "The old force must not change when updating the velocity.";
        EXPECT_FLOAT_EQ(pi->getM(), particles[i].getM()) << "The mass must not change when updating the velocity.";
        EXPECT_EQ(pi->getType(), particles[i].getType()) << "The type must not change when updating the velocity.";

        // Test if the new velocity is correct
        EXPECT_LT(ArrayUtils::L2Norm(pi->getV() - expected_v[i]), error_margin)
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

    EXPECT_EQ(calc.get_container().size(), 0) << "Calculating the velocity on an empty container should not add a particle.";
}

// Test if update old f works for handcrafted values
TEST(Calculator, UpdateOldF2) {
    // Set the margin for the maximum floatingpoint error
    const double error_margin = 1E-9;

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

// Test if update OldF works for an empty particle container
TEST(Calculator, UpdateOldFNo) {
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

    ASSERT_NO_THROW(calc.calculateOldF());

    EXPECT_EQ(calc.get_container().size(), 0) << "Calculating the velocity on an empty container should not add a particle.";
}

// Test if update f works for handcrafted values
TEST(LJCalculator, UpdateF1) {
    // Set the margin for the maximum floatingpoint error
    const double error_margin = 1E-9;

    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 1.0, 2.0, -1.0 }, { 2.0, 3.0, -3.0 }, 2.0),
        Particle({ 1.0, 4.0, -1.0 }, { 3.0, -2.0, -1.0 }, 3.0),
    };

    // The forces must always be set to zero before calculateF is called.
    particles[0].setF({ 0.0, 0.0, 0.0 });
    particles[0].setOldF({ -2.0, -3.0, 2.0 });

    particles[1].setF({ 0.0, 0.0, 0.0 });
    particles[1].setOldF({ 2.0, -1.0, 3.0 });

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
    physicsCalculator::LJCalculator calc(env, particles, false);

    // Initialize the positions to the expected values
    const std::vector<std::array<double, 3>> expected_F = {
        { 0.0, 465.0 / 512.0, 0.0 },
        { 0.0, -465.0 / 512.0, 0.0 },
    };

    // Perform a single calculateF
    ASSERT_NO_THROW(calc.calculateF());

    // Make sure that there are no unwanted changes
    ASSERT_EQ(particles.size(), calc.get_container().size()) << "The number of particles must not change when updating the particles forces.";

    auto pi = calc.get_container().begin();

    for (size_t i = 0; i < particles.size(); i++) {
        EXPECT_TRUE(pi->getX() == particles[i].getX()) << "The position must not change when updating the forces.";
        EXPECT_TRUE(pi->getV() == particles[i].getV()) << "The velocity must not change when updating the forces.";
        EXPECT_TRUE(pi->getOldF() == particles[i].getOldF()) << "The old force must not change when updating the forces.";
        EXPECT_FLOAT_EQ(pi->getM(), particles[i].getM()) << "The mass must not change when updating the forces.";
        EXPECT_EQ(pi->getType(), particles[i].getType()) << "The type must not change when updating the forces.";

        // Test if the new force is correct
        EXPECT_LT(ArrayUtils::L2Norm(pi->getF() - expected_F[i]), error_margin)
            << "The force was not correct. (expected: " << ArrayUtils::to_string(expected_F[i]) << ", got: " << ArrayUtils::to_string(pi->getF())
            << ")";

        pi++;
    }
}

// Test if update f works for handcrafted values
TEST(LJCalculator, UpdateF2) {
    // Set the margin for the maximum floatingpoint error
    const double error_margin = 1E-9;

    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 1.0, 1.0, 1.0 }, { 1.0, 2.0, -2.0 }, 4.0),
        Particle({ 5.0, 1.0, 1.0 }, { 2.0, 2.0, 1.0 }, 2.0),
    };

    // The forces must always be set to zero before calculateF is called.
    particles[0].setF({ 0.0, 0.0, 0.0 });
    particles[0].setOldF({ 1.0, 2.0, -1.0 });

    particles[1].setF({ 0.0, 0.0, 0.0 });
    particles[1].setOldF({ -2.0, -3.0, 3.0 });

    // Initialize the simulation environment
    const char* argv[] = {
        "./MolSim",
        "path/to/input.txt",
        "-sigma=2.0",
        "-epsilon=4.0",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);
    Environment env;

    ASSERT_NO_THROW(env = Environment(argc, argv));

    // Initialize the Calculator
    physicsCalculator::LJCalculator calc(env, particles, false);

    // Initialize the positions to the expected values
    const std::vector<std::array<double, 3>> expected_F = {
        { 93.0 / 256.0, 0.0, 0.0 },
        { -93.0 / 256.0, 0.0, 0.0 },
    };

    // Perform a single calculateF
    ASSERT_NO_THROW(calc.calculateF());

    // Make sure that there are no unwanted changes
    ASSERT_EQ(particles.size(), calc.get_container().size()) << "The number of particles must not change when updating the particles forces.";

    auto pi = calc.get_container().begin();

    for (size_t i = 0; i < particles.size(); i++) {
        EXPECT_TRUE(pi->getX() == particles[i].getX()) << "The position must not change when updating the forces.";
        EXPECT_TRUE(pi->getV() == particles[i].getV()) << "The velocity must not change when updating the forces.";
        EXPECT_TRUE(pi->getOldF() == particles[i].getOldF()) << "The old force must not change when updating the forces.";
        EXPECT_FLOAT_EQ(pi->getM(), particles[i].getM()) << "The mass must not change when updating the forces.";
        EXPECT_EQ(pi->getType(), particles[i].getType()) << "The type must not change when updating the forces.";

        // Test if the new force is correct
        EXPECT_LT(ArrayUtils::L2Norm(pi->getF() - expected_F[i]), error_margin)
            << "The force was not correct. (expected: " << ArrayUtils::to_string(expected_F[i]) << ", got: " << ArrayUtils::to_string(pi->getF())
            << ")";

        pi++;
    }
}

// Test if update f works for handcrafted values
TEST(LJCalculator, UpdateF3) {
    // Set the margin for the maximum floatingpoint error
    const double error_margin = 1E-9;

    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 0.0, 2.0, -1.0 }, { 3.0, 1.0, -2.0 }, 2.0),
        Particle({ 0.0, 2.0, 0.0 }, { 1.0, 1.0, 0.0 }, 5.0),
    };

    // The forces must always be set to zero before calculateF is called.
    particles[0].setF({ 0.0, 0.0, 0.0 });
    particles[0].setOldF({ 2.0, 4.0, -2.0 });

    particles[1].setF({ 0.0, 0.0, 0.0 });
    particles[1].setOldF({ -1.0, 2.0, 2.0 });

    // Initialize the simulation environment
    const char* argv[] = {
        "./MolSim",
        "path/to/input.txt",
        "-sigma=2.0",
        "-epsilon=5.0",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);
    Environment env;

    ASSERT_NO_THROW(env = Environment(argc, argv));

    // Initialize the Calculator
    physicsCalculator::LJCalculator calc(env, particles, false);

    // Initialize the positions to the expected values
    const std::vector<std::array<double, 3>> expected_F = {
        { 0.0, 0.0, -975360.0 },
        { 0.0, 0.0, 975360.0 },
    };

    // Perform a single calculateF
    ASSERT_NO_THROW(calc.calculateF());

    // Make sure that there are no unwanted changes
    ASSERT_EQ(particles.size(), calc.get_container().size()) << "The number of particles must not change when updating the particles forces.";

    auto pi = calc.get_container().begin();

    for (size_t i = 0; i < particles.size(); i++) {
        EXPECT_TRUE(pi->getX() == particles[i].getX()) << "The position must not change when updating the forces.";
        EXPECT_TRUE(pi->getV() == particles[i].getV()) << "The velocity must not change when updating the forces.";
        EXPECT_TRUE(pi->getOldF() == particles[i].getOldF()) << "The old force must not change when updating the forces.";
        EXPECT_FLOAT_EQ(pi->getM(), particles[i].getM()) << "The mass must not change when updating the forces.";
        EXPECT_EQ(pi->getType(), particles[i].getType()) << "The type must not change when updating the forces.";

        // Test if the new force is correct
        EXPECT_LT(ArrayUtils::L2Norm(pi->getF() - expected_F[i]), error_margin)
            << "The force was not correct. (expected: " << ArrayUtils::to_string(expected_F[i]) << ", got: " << ArrayUtils::to_string(pi->getF())
            << ")";

        pi++;
    }
}

// Test if update f works for handcrafted values
TEST(LJCalculator, UpdateF4) {
    // Set the margin for the maximum floatingpoint error
    const double error_margin = 1E-9;

    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 1.0, 2.0, 1.0 }, { 3.0, 1.0, -2.0 }, 1.0),
        Particle({ 2.0, 3.0, 2.0 }, { 2.0, -1.0, 2.0 }, 2.0),
    };

    // The forces must always be set to zero before calculateF is called.
    particles[0].setF({ 0.0, 0.0, 0.0 });
    particles[0].setOldF({ 4.0, 2.0, 1.0 });

    particles[1].setF({ 0.0, 0.0, 0.0 });
    particles[1].setOldF({ -2.0, -2.0, -3.0 });

    // Initialize the simulation environment
    const char* argv[] = {
        "./MolSim",
        "path/to/input.txt",
        "-sigma=1.0",
        "-epsilon=3.0",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);
    Environment env;

    ASSERT_NO_THROW(env = Environment(argc, argv));

    // Initialize the Calculator
    physicsCalculator::LJCalculator calc(env, particles, false);

    // Initialize the positions to the expected values
    const std::vector<std::array<double, 3>> expected_F = {
        { 200.0 / 243.0, 200.0 / 243.0, 200.0 / 243.0 },
        { -200.0 / 243.0, -200.0 / 243.0, -200.0 / 243.0 },
    };

    // Perform a single calculateF
    ASSERT_NO_THROW(calc.calculateF());

    // Make sure that there are no unwanted changes
    ASSERT_EQ(particles.size(), calc.get_container().size()) << "The number of particles must not change when updating the particles forces.";

    auto pi = calc.get_container().begin();

    for (size_t i = 0; i < particles.size(); i++) {
        EXPECT_TRUE(pi->getX() == particles[i].getX()) << "The position must not change when updating the forces.";
        EXPECT_TRUE(pi->getV() == particles[i].getV()) << "The velocity must not change when updating the forces.";
        EXPECT_TRUE(pi->getOldF() == particles[i].getOldF()) << "The old force must not change when updating the forces.";
        EXPECT_FLOAT_EQ(pi->getM(), particles[i].getM()) << "The mass must not change when updating the forces.";
        EXPECT_EQ(pi->getType(), particles[i].getType()) << "The type must not change when updating the forces.";

        // Test if the new force is correct
        EXPECT_LT(ArrayUtils::L2Norm(pi->getF() - expected_F[i]), error_margin)
            << "The force was not correct. (expected: " << ArrayUtils::to_string(expected_F[i]) << ", got: " << ArrayUtils::to_string(pi->getF())
            << ")";

        pi++;
    }
}

// Test if update f works for an empty particle container
TEST(LJCalculator, UpdateFNo) {
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

    ASSERT_NO_THROW(calc.calculateF());

    EXPECT_EQ(calc.get_container().size(), 0) << "Calculating the force on an empty container should not add a particle.";
}

// Test if update f works for a single particle
TEST(LJCalculator, UpdateFSingle) {
    // Set the margin for the maximum floatingpoint error
    const double error_margin = 1E-9;

    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 2.0, -1.0, 2.0 }, { 3.0, -2.0, 2.0 }, 0.5, 1),
    };

    // When calling calculateF the current force must be zero
    particles[0].setF({ 0.0, 0.0, 0.0 });
    particles[0].setOldF({ -2.0, 2.0, 2.0 });

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

    // Perform a single calculateF
    ASSERT_NO_THROW(calc.calculateF());

    // Make sure that there are no unwanted changes
    ASSERT_EQ(particles.size(), calc.get_container().size()) << "The number of particles must not change when updating the particles forces.";

    const std::array<double, 3> zero_v = { 0.0, 0.0, 0.0 };

    EXPECT_TRUE(calc.get_container()[0].getX() == particles[0].getX()) << "The position must not change when updating the force.";
    EXPECT_TRUE(calc.get_container()[0].getV() == particles[0].getV()) << "The velocity must not change when updating the force.";
    EXPECT_TRUE(calc.get_container()[0].getOldF() == particles[0].getOldF()) << "The old force must not change when updating the force.";
    EXPECT_FLOAT_EQ(calc.get_container()[0].getM(), particles[0].getM()) << "The mass must not change when updating the force.";
    EXPECT_EQ(calc.get_container()[0].getType(), particles[0].getType()) << "The type must not change when updating the force.";

    // Test if the new forces are correct
    EXPECT_TRUE(calc.get_container()[0].getF() == zero_v) << "The current force should remain zero.";
}

// // Test if a single step computes the correct values
// TEST(LJCalculator, Step1) {
//     // Set the margin for the maximum floatingpoint error
//     const double error_margin = 1E-9;

//     // Initialize the list of particles
//     std::vector<Particle> particles = {
//         Particle({ 4.0, 0.0, 0.0 }, { -1.0, 0.0, 0.0 }, 1.0),
//         Particle({ -4.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 }, 1.0),
//     };

//     particles[0].setF({ -0.1, 0.0, 0.0 });
//     particles[0].setOldF({ 2.0, 1.0, 2.0 });

//     particles[1].setF({ 0.1, 0.0, 0.0 });
//     particles[1].setOldF({ -2.0, 1.0, 1.0 });

//     // Initialize the simulation environment
//     const char* argv[] = {
//         "./MolSim",
//         "path/to/input.txt",
//         "-delta_t=1.0",
//         "-sigma=1",
//         "-epsilon=4",
//     };

//     constexpr int argc = sizeof(argv) / sizeof(argv[0]);
//     Environment env;

//     ASSERT_NO_THROW(env = Environment(argc, argv));

//     // Initialize the Calculator
//     physicsCalculator::LJCalculator calc(env, particles, false);

//     // Initialize the positions to the expected values
//     std::vector<Particle> expected = {
//         Particle({ 2.95, 0.0, 0.0 }, { -1.0501928663952, 0.0, 0.0 }, 1.0),
//         Particle({ -2.95, 0.0, 0.0 }, { 1.0501928663952, 0.0, 0.0 }, 1.0),
//     };

//     expected[0].setF({ -0.0003857327903, 0.0, 0.0 });
//     expected[0].setOldF({ -0.1, 0.0, 0.0 });

//     expected[1].setF({ 0.0003857327903, 0.0, 0.0 });
//     expected[1].setOldF({ 0.1, 0.0, 0.0 });

//     // Perform a single step
//     ASSERT_NO_THROW(calc.step());

//     // Make sure that there are no unwanted changes
//     ASSERT_EQ(expected.size(), calc.get_container().size()) << "The number of particles must not change when updating the particles forces.";

//     auto pi = calc.get_container().begin();

//     for (size_t i = 0; i < expected.size(); i++) {
//         EXPECT_FLOAT_EQ(pi->getM(), expected[i].getM()) << "The particle mass must not change.";
//         EXPECT_EQ(pi->getType(), expected[i].getType()) << "The particle type must not change.";

//         EXPECT_LT(ArrayUtils::L2Norm(pi->getX() - expected[i].getX()), error_margin) << "The particle must have the new position.";
//         EXPECT_LT(ArrayUtils::L2Norm(pi->getV() - expected[i].getV()), error_margin) << "The particle must have the new velocity.";
//         EXPECT_LT(ArrayUtils::L2Norm(pi->getF() - expected[i].getF()), error_margin) << "The particle must have the new force.";
//         EXPECT_LT(ArrayUtils::L2Norm(pi->getOldF() - expected[i].getOldF()), error_margin) << "The particle must have the new old force.";

//         pi++;
//     }
// }

// // Test if step works for an empty particle container
// TEST(Calculator, StepNo) {
//     // Initialize the simulation environment
//     const char* argv[] = {
//         "./MolSim",
//         "path/to/input.txt",
//     };

//     constexpr int argc = sizeof(argv) / sizeof(argv[0]);
//     Environment env;

//     ASSERT_NO_THROW(env = Environment(argc, argv));

//     // Initialize the Calculator
//     physicsCalculator::LJCalculator calc(env, {}, false);

//     ASSERT_NO_THROW(calc.step());

//     EXPECT_EQ(calc.get_container().size(), 0) << "Calculating the velocity on an empty container should not add a particle.";
// }

// // Test if step works for an analytical solution using a single moving body
// TEST(LJCalculator, Analytical1) {
//     // Set the margin for the maximum floatingpoint error
//     const double error_margin = 1E-9;

//     // Initialize the list of particles
//     std::vector<Particle> particles = {
//         Particle({ 1.0, 10.0, -25.0 }, { 0.5, -0.5, 0.2 }, 3.3),
//     };

//     // Initialize the simulation environment
//     const char* argv[] = {
//         "./MolSim",
//         "path/to/input.txt",
//         "-delta_t=0.0001",
//     };

//     constexpr int argc = sizeof(argv) / sizeof(argv[0]);
//     Environment env;

//     ASSERT_NO_THROW(env = Environment(argc, argv));

//     // Initialize the Calculator
//     physicsCalculator::LJCalculator calc(env, particles);
//     double total_time = 0.0;

//     // Perform the steps for 50 time units
//     for (size_t i = 0; i <= 500000; i++) {
//         // Test that the position is correct
//         const std::array<double, 3> expected_pos = { 1.0 + total_time * 0.5, 10.0 + total_time * -0.5, -25.0 + total_time * 0.2 };
//         ASSERT_LT(ArrayUtils::L2Norm(calc.get_container()[0].getX() - expected_pos), error_margin)
//             << "The calculation diverged at time step " << i << " (" << total_time << ") (Expected: " << ArrayUtils::to_string(expected_pos)
//             << ", Got: " << ArrayUtils::to_string(calc.get_container()[0].getX()) << ")";

//         ASSERT_NO_THROW(calc.step());
//         total_time += 0.0001;
//     }
// }

// // Test if step works for an analytical solution using two bodies circling around their shared center of gravity on the same circle. Meanwhile the
// // center of gravity is slowly moving.
// TEST(LJCalculator, Analytical2) {
//     // Set the margin for the maximum floatingpoint error
//     const double error_margin = 1E-6;

//     // Initialize the list of particles
//     std::vector<Particle> particles = {
//         Particle({ 6.3, 2.2, -1.5 }, { 0.02, 0.99, 0.03 }, 365346816.0),
//         Particle({ -3.7, 2.2, -1.5 }, { 0.02, -1.01, 0.03 }, 365346816.0),
//     };

//     // Initialize the simulation environment
//     const char* argv[] = {
//         "./MolSim",
//         "path/to/input.txt",
//         "-delta_t=0.0001",
//         "-sigma=8.0",
//         "-epsilon=244140625.0",
//     };

//     constexpr int argc = sizeof(argv) / sizeof(argv[0]);
//     Environment env;

//     ASSERT_NO_THROW(env = Environment(argc, argv));

//     // Initialize the Calculator
//     physicsCalculator::LJCalculator calc(env, particles);
//     double total_time = 0.0;

//     // Perform the steps for 100 time units
//     for (size_t i = 0; i <= 1000000; i++) {
//         // Test that the position is correct
//         const std::array<double, 3> expected_pos_0 = {
//             5.0 * std::cos(total_time / 5.0) + total_time * 0.02 + 1.3,
//             5.0 * std::sin(total_time / 5.0) + total_time * -0.01 + 2.2,
//             -1.5 + total_time * 0.03,
//         };

//         const std::array<double, 3> expected_pos_1 = {
//             -5.0 * std::cos(total_time / 5.0) + total_time * 0.02 + 1.3,
//             -5.0 * std::sin(total_time / 5.0) + total_time * -0.01 + 2.2,
//             -1.5 + total_time * 0.03,
//         };

//         ASSERT_LT(ArrayUtils::L2Norm(calc.get_container()[0].getX() - expected_pos_0), error_margin)
//             << "The calculation diverged at time step " << i << " (" << total_time << ") (Expected: " << ArrayUtils::to_string(expected_pos_0)
//             << ", Got: " << ArrayUtils::to_string(calc.get_container()[0].getX()) << ")";
//         ASSERT_LT(ArrayUtils::L2Norm(calc.get_container()[1].getX() - expected_pos_1), error_margin)
//             << "The calculation diverged at time step " << i << " (" << total_time << ") (Expected: " << ArrayUtils::to_string(expected_pos_1)
//             << ", Got: " << ArrayUtils::to_string(calc.get_container()[1].getX()) << ")";

//         ASSERT_NO_THROW(calc.step());
//         total_time += 0.0001;
//     }
// }

// // Test if step works for an analytical solution using two bodies circling around their shared center of gravity. However this time the two circles
// // have two different radiuses.
// TEST(LJCalculator, Analytical3) {
//     // Set the margin for the maximum floatingpoint error
//     const double error_margin = 1E-6;

//     // Initialize the list of particles
//     std::vector<Particle> particles = {
//         Particle({ 0.0, 0.0, 5.0 }, { 0.0, 1.0, 0.0 }, 243564544.0),
//         Particle({ 0.0, 0.0, -10.0 }, { 0.0, -2.0, 0.0 }, 121782272.5),
//     };

//     // Initialize the simulation environment
//     const char* argv[] = {
//         "./MolSim",
//         "path/to/input.txt",
//         "-delta_t=0.0001",
//         "-sigma=12.0",
//         "-epsilon=244140625.0",
//     };

//     constexpr int argc = sizeof(argv) / sizeof(argv[0]);
//     Environment env;

//     ASSERT_NO_THROW(env = Environment(argc, argv));

//     // Initialize the Calculator
//     physicsCalculator::LJCalculator calc(env, particles);
//     double total_time = 0.0;

//     // Perform the steps for 50 time units
//     for (size_t i = 0; i <= 500000; i++) {
//         // Test that the position is correct
//         const std::array<double, 3> expected_pos_0 = {
//             0.0,
//             5.0 * std::sin(total_time / 5.0),
//             5.0 * std::cos(total_time / 5.0),
//         };

//         const std::array<double, 3> expected_pos_1 = {
//             0.0,
//             -10.0 * std::sin(total_time / 5.0),
//             -10.0 * std::cos(total_time / 5.0),
//         };

//         ASSERT_LT(ArrayUtils::L2Norm(calc.get_container()[0].getX() - expected_pos_0), error_margin)
//             << "The calculation diverged at time step " << i << " (" << total_time << ") (Expected: " << ArrayUtils::to_string(expected_pos_0)
//             << ", Got: " << ArrayUtils::to_string(calc.get_container()[0].getX()) << ")";
//         ASSERT_LT(ArrayUtils::L2Norm(calc.get_container()[1].getX() - expected_pos_1), error_margin)
//             << "The calculation diverged at time step " << i << " (" << total_time << ") (Expected: " << ArrayUtils::to_string(expected_pos_1)
//             << ", Got: " << ArrayUtils::to_string(calc.get_container()[1].getX()) << ")";

//         ASSERT_NO_THROW(calc.step());
//         total_time += 0.0001;
//     }
// }

// // Test if step works for an analytical solution using three bodies which are completely stationary relative to each other. The shared center of
// // gravity is also moving.
// TEST(LJCalculator, Analytical4) {
//     // Set the margin for the maximum floatingpoint error
//     const double error_margin = 1E-5;

//     // Initialize the list of particles
//     std::vector<Particle> particles = {
//         Particle(
//             {
//                 1.7480669497636142554225485204397801807855281376745671279006682579,
//                 -1.351933050236385744577451479560219819214471862325432872099331742,
//                 1.1480669497636142554225485204397801807855281376745671279006682579,
//             },
//             { 0.001, 0.001, -0.002 }, 1.0),
//         Particle({ 1.1, -2.0, 0.5 }, { 0.001, 0.001, -0.002 }, 1.5),
//         Particle(
//             {
//                 0.4519330502363857445774514795602198192144718623254328720993317420,
//                 -2.6480669497636142554225485204397801807855281376745671279006682579,
//                 -0.148066949763614255422548520439780180785528137674567127900668257,
//             },
//             { 0.001, 0.001, -0.002 }, 2.0),
//     };

//     // Initialize the simulation environment
//     const char* argv[] = {
//         "./MolSim",
//         "path/to/input.txt",
//         "-delta_t=0.0001",
//         "-sigma=1.0012978649056434718130942322111430509313330669648478791579796678",
//         "-epsilon=4.0",
//     };

//     constexpr int argc = sizeof(argv) / sizeof(argv[0]);
//     Environment env;

//     ASSERT_NO_THROW(env = Environment(argc, argv));

//     // Initialize the Calculator
//     physicsCalculator::LJCalculator calc(env, particles);
//     double total_time = 0.0;

//     // Perform the steps for 20 time units
//     for (size_t i = 0; i <= 200000; i++) {
//         // Test that the position is correct
//         const std::array<double, 3> expected_pos_0 = {
//             1.7480669497636142554225485204397801807855281376745671279006682579 + total_time * 0.001,
//             -1.351933050236385744577451479560219819214471862325432872099331742 + total_time * 0.001,
//             1.1480669497636142554225485204397801807855281376745671279006682579 + total_time * -0.002,
//         };

//         const std::array<double, 3> expected_pos_1 = {
//             1.1 + total_time * 0.001,
//             -2.0 + total_time * 0.001,
//             0.5 + total_time * -0.002,
//         };

//         const std::array<double, 3> expected_pos_2 = {
//             0.4519330502363857445774514795602198192144718623254328720993317420 + total_time * 0.001,
//             -2.6480669497636142554225485204397801807855281376745671279006682579 + total_time * 0.001,
//             -0.148066949763614255422548520439780180785528137674567127900668257 + total_time * -0.002,
//         };

//         ASSERT_LT(ArrayUtils::L2Norm(calc.get_container()[0].getX() - expected_pos_0), error_margin)
//             << "The calculation diverged at time step " << i << " (" << total_time << ") (Expected: " << ArrayUtils::to_string(expected_pos_0)
//             << ", Got: " << ArrayUtils::to_string(calc.get_container()[0].getX()) << ")";
//         ASSERT_LT(ArrayUtils::L2Norm(calc.get_container()[1].getX() - expected_pos_1), error_margin)
//             << "The calculation diverged at time step " << i << " (" << total_time << ") (Expected: " << ArrayUtils::to_string(expected_pos_1)
//             << ", Got: " << ArrayUtils::to_string(calc.get_container()[1].getX()) << ")";
//         ASSERT_LT(ArrayUtils::L2Norm(calc.get_container()[2].getX() - expected_pos_2), error_margin)
//             << "The calculation diverged at time step " << i << " (" << total_time << ") (Expected: " << ArrayUtils::to_string(expected_pos_2)
//             << ", Got: " << ArrayUtils::to_string(calc.get_container()[2].getX()) << ")";

//         ASSERT_NO_THROW(calc.step());
//         total_time += 0.0001;
//     }
// }
