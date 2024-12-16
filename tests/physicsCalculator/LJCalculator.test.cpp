
#include <gtest/gtest.h>
#include <physicsCalculator/LJCalculator.h>

// Test if update x works for handcrafted values
TEST(Calculator, UpdateX2) {
    // Set the margin for the maximum floatingpoint error
    const double error_margin = 1E-9;

    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 1.0, 2.0, 3.0 }, { 2.0, 2.0, 0.0 }, 1),
        Particle({ -2.0, -2.0, 1.0 }, { 1.0, -1.0, 2.0 }, 0),
        Particle({ -1.0, -1.0, 2.0 }, { -2.0, 0.0, 1.0 }, 0),
        Particle({ -2.0, 1.0, 1.0 }, { -1.0, -2.0, -1.0 }, 0),
    };
    // Initialise the list of type descriptors
    std::vector<TypeDesc> ptypes = {
        TypeDesc { 1.0, 1.0, 5.0, 0.1, 0.0 },
        TypeDesc { 2.0, 1.0, 5.0, 0.1, 0.0 },
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
    physicsCalculator::LJCalculator calc(env, particles, ptypes, false);

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
        EXPECT_FLOAT_EQ(calc.get_container().get_type_descriptor(particles[i].getType()).get_mass(), ptypes[particles[i].getType()].get_mass())
            << "The mass must not change when updating the position.";
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
    physicsCalculator::LJCalculator calc(env, {}, {}, false);

    ASSERT_NO_THROW(calc.calculateX());

    EXPECT_EQ(calc.get_container().size(), 0) << "Calculating the position on an empty container should not add a particle.";
}

// Test if update v works for handcrafted values
TEST(Calculator, UpdateV2) {
    // Set the margin for the maximum floatingpoint error
    const double error_margin = 1E-9;

    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ -3.0, 2.0, -1.0 }, { 1.0, 2.0, 1.0 }, 0),
        Particle({ 2.0, -1.0, -3.0 }, { 0.0, -1.0, 1.0 }, 1),
        Particle({ 2.0, 2.0, -1.0 }, { 1.0, -2.0, -1.0 }, 0),
        Particle({ 2.0, 0.0, -2.0 }, { -2.0, 1.0, 1.0 }, 0),
    };
    // Initialise the list of type descriptors
    std::vector<TypeDesc> ptypes = {
        TypeDesc { 1.0, 1.0, 5.0, 0.1, 0.0 },
        TypeDesc { 2.0, 1.0, 5.0, 0.1, 0.0 },
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
    physicsCalculator::LJCalculator calc(env, particles, ptypes, false);

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
        EXPECT_FLOAT_EQ(calc.get_container().get_type_descriptor(particles[i].getType()).get_mass(), ptypes[particles[i].getType()].get_mass())
            << "The mass must not change when updating the velocity.";
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
    physicsCalculator::LJCalculator calc(env, {}, {}, false);

    ASSERT_NO_THROW(calc.calculateV());

    EXPECT_EQ(calc.get_container().size(), 0) << "Calculating the velocity on an empty container should not add a particle.";
}

// Test if update old f works for handcrafted values
TEST(Calculator, UpdateOldF2) {
    // Set the margin for the maximum floatingpoint error
    const double error_margin = 1E-9;

    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 2.0, -1.0, -3.0 }, { 2.0, -1.0, -2.0 }, 1),
        Particle({ 3.0, 3.0, -1.0 }, { 1.0, 1.0, -3.0 }, 2),
        Particle({ 1.0, -1.0, 1.0 }, { -2.0, 2.0, -1.0 }, 2),
        Particle({ 2.0, -1.0, 2.0 }, { 3.0, -2.0, 2.0 }, 0),
    };
    // Initialise the list of type descriptors
    std::vector<TypeDesc> ptypes = {
        TypeDesc { 0.5, 1.0, 5.0, 0.1, 0.0 },
        TypeDesc { 1.0, 1.0, 5.0, 0.1, 0.0 },
        TypeDesc { 2.0, 1.0, 5.0, 0.1, 0.0 },
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
    physicsCalculator::LJCalculator calc(env, particles, ptypes, false);

    // Perform a single calculateOldF
    ASSERT_NO_THROW(calc.calculateOldF());

    // Make sure that there are no unwanted changes
    ASSERT_EQ(particles.size(), calc.get_container().size()) << "The number of particles must not change when updating the particles old force.";

    auto pi = calc.get_container().begin();
    const std::array<double, 3> zero_v = { 0.0, 0.0, 0.0 };

    for (size_t i = 0; i < particles.size(); i++) {
        EXPECT_TRUE(pi->getX() == particles[i].getX()) << "The position must not change when updating the old force.";
        EXPECT_TRUE(pi->getV() == particles[i].getV()) << "The velocity must not change when updating the old force.";
        EXPECT_FLOAT_EQ(calc.get_container().get_type_descriptor(particles[i].getType()).get_mass(), ptypes[particles[i].getType()].get_mass())
            << "The mass must not change when updating the old force.";
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
    physicsCalculator::LJCalculator calc(env, {}, {}, false);

    ASSERT_NO_THROW(calc.calculateOldF());

    EXPECT_EQ(calc.get_container().size(), 0) << "Calculating the velocity on an empty container should not add a particle.";
}

// Test if update f works for handcrafted values
TEST(LJCalculator, UpdateF1) {
    // Set the margin for the maximum floatingpoint error
    const double error_margin = 1E-9;

    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 1.0, 2.0, -1.0 }, { 2.0, 3.0, -3.0 }, 1),
        Particle({ 1.0, 4.0, -1.0 }, { 3.0, -2.0, -1.0 }, 0),
    };
    // Initialise the list of type descriptors
    std::vector<TypeDesc> ptypes = {
        TypeDesc { 3.0, 1.0, 5.0, 0.1, 0.0 },
        TypeDesc { 2.0, 1.0, 5.0, 0.1, 0.0 },
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
    physicsCalculator::LJCalculator calc(env, particles, ptypes, false);

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
        EXPECT_FLOAT_EQ(calc.get_container().get_type_descriptor(particles[i].getType()).get_mass(), ptypes[particles[i].getType()].get_mass())
            << "The mass must not change when updating the forces.";
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
        Particle({ 1.0, 1.0, 1.0 }, { 1.0, 2.0, -2.0 }, 0),
        Particle({ 5.0, 1.0, 1.0 }, { 2.0, 2.0, 1.0 }, 1),
    };
    // Initialise the list of type descriptors
    std::vector<TypeDesc> ptypes = {
        TypeDesc { 4.0, 2.0, 4.0, 0.1, 0.0 },
        TypeDesc { 2.0, 2.0, 4.0, 0.1, 0.0 },
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
    physicsCalculator::LJCalculator calc(env, particles, ptypes, false);

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
        EXPECT_FLOAT_EQ(calc.get_container().get_type_descriptor(particles[i].getType()).get_mass(), ptypes[particles[i].getType()].get_mass())
            << "The mass must not change when updating the forces.";
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
        Particle({ 0.0, 2.0, -1.0 }, { 3.0, 1.0, -2.0 }, 1),
        Particle({ 0.0, 2.0, 0.0 }, { 1.0, 1.0, 0.0 }, 0),
    };
    // Initialise the list of type descriptors
    std::vector<TypeDesc> ptypes = {
        TypeDesc { 5.0, 2.0, 5.0, 0.1, 0.0 },
        TypeDesc { 2.0, 2.0, 5.0, 0.1, 0.0 },
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
    physicsCalculator::LJCalculator calc(env, particles, ptypes, false);

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
        EXPECT_FLOAT_EQ(calc.get_container().get_type_descriptor(particles[i].getType()).get_mass(), ptypes[particles[i].getType()].get_mass())
            << "The mass must not change when updating the forces.";
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
        Particle({ 1.0, 2.0, 1.0 }, { 3.0, 1.0, -2.0 }, 0),
        Particle({ 2.0, 3.0, 2.0 }, { 2.0, -1.0, 2.0 }, 1),
    };
    // Initialise the list of type descriptors
    std::vector<TypeDesc> ptypes = {
        TypeDesc { 1.0, 1.0, 3.0, 0.1, 0.0 },
        TypeDesc { 2.0, 1.0, 3.0, 0.1, 0.0 },
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
    physicsCalculator::LJCalculator calc(env, particles, ptypes, false);

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
        EXPECT_FLOAT_EQ(calc.get_container().get_type_descriptor(particles[i].getType()).get_mass(), ptypes[particles[i].getType()].get_mass())
            << "The mass must not change when updating the forces.";
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
    physicsCalculator::LJCalculator calc(env, {}, {}, false);

    ASSERT_NO_THROW(calc.calculateF());

    EXPECT_EQ(calc.get_container().size(), 0) << "Calculating the force on an empty container should not add a particle.";
}

// Test if update f works for a single particle
TEST(LJCalculator, UpdateFSingle) {
    // Set the margin for the maximum floatingpoint error
    const double error_margin = 1E-9;

    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 2.0, -1.0, 2.0 }, { 3.0, -2.0, 2.0 }, 0),
    };
    // Initialise the list of type descriptors
    std::vector<TypeDesc> ptypes = {
        TypeDesc { 0.5, 1.0, 5.0, 0.1, 0.0 },
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
    physicsCalculator::LJCalculator calc(env, particles, ptypes, false);

    // Perform a single calculateF
    ASSERT_NO_THROW(calc.calculateF());

    // Make sure that there are no unwanted changes
    ASSERT_EQ(particles.size(), calc.get_container().size()) << "The number of particles must not change when updating the particles forces.";

    const std::array<double, 3> zero_v = { 0.0, 0.0, 0.0 };

    EXPECT_TRUE(calc.get_container()[0].getX() == particles[0].getX()) << "The position must not change when updating the force.";
    EXPECT_TRUE(calc.get_container()[0].getV() == particles[0].getV()) << "The velocity must not change when updating the force.";
    EXPECT_TRUE(calc.get_container()[0].getOldF() == particles[0].getOldF()) << "The old force must not change when updating the force.";
    EXPECT_FLOAT_EQ(calc.get_container().get_type_descriptor(particles[0].getType()).get_mass(), ptypes[particles[0].getType()].get_mass())
        << "The mass must not change when updating the force.";
    EXPECT_EQ(calc.get_container()[0].getType(), particles[0].getType()) << "The type must not change when updating the force.";

    // Test if the new forces are correct
    EXPECT_TRUE(calc.get_container()[0].getF() == zero_v) << "The current force should remain zero.";
}
