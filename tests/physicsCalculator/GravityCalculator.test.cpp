#include <gtest/gtest.h>
#include <physicsCalculator/GravityCalculator.h>

// Test if update x works for handcrafted values
TEST(Calculator, UpdateX1) {
    // Set the margin for the maximum floatingpoint error
    const double error_margin = 1E-9;

    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 0.1, 1.3, -2.0 }, { 7.0, 2.0, -0.5 }, 2),
        Particle({ 2.5, -3.0, -0.5 }, { -3.0, 1.5, -1.5 }, 1),
        Particle({ -2.0, 3.0, 3.5 }, { 1.0, -2.0, 0.5 }, 1),
        Particle({ 0.1, -1.5, 2.0 }, { 3.0, 2.0, -0.5 }, 0),
    };
    // Initialise the list of type descriptors
    std::vector<TypeDesc> ptypes = {
        TypeDesc { 4.0, 1.0, 5.0, 0.1, 0.0 },
        TypeDesc { 1.0, 1.0, 5.0, 0.1, 0.0 },
        TypeDesc { 2.0, 1.0, 5.0, 0.1, 0.0 },
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
    physicsCalculator::GravityCalculator calc(env, particles, ptypes, false);

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
        EXPECT_FLOAT_EQ(calc.get_container().get_type_descriptor(i).get_mass(), ptypes[i].get_mass())
            << "The mass must not change when updating the position.";
        EXPECT_EQ(pi->getType(), particles[i].getType()) << "The type must not change when updating the position.";

        // Test if the new position is correct
        EXPECT_LT(ArrayUtils::L2Norm(pi->getX() - expected_x[i]), error_margin)
            << "The position was not correct. (expected: " << ArrayUtils::to_string(expected_x[i]) << ", got: " << ArrayUtils::to_string(pi->getX())
            << ")";

        pi++;
    }
}

// Test if update v works for handcrafted values
TEST(Calculator, UpdateV1) {
    // Set the margin for the maximum floatingpoint error
    const double error_margin = 1E-9;

    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 0.1, 1.3, -2.0 }, { 7.0, 2.0, -0.5 }, 2),
        Particle({ 2.5, -3.0, -0.5 }, { -3.0, 1.5, -1.5 }, 1),
        Particle({ -2.0, 3.0, 3.5 }, { 1.0, -2.0, 0.5 }, 1),
        Particle({ 0.1, -1.5, 2.0 }, { 3.0, 2.0, -0.5 }, 0),
    };
    // Initialise the list of type descriptors
    std::vector<TypeDesc> ptypes = {
        TypeDesc { 4.0, 1.0, 5.0, 0.1, 0.0 },
        TypeDesc { 1.0, 1.0, 5.0, 0.1, 0.0 },
        TypeDesc { 2.0, 1.0, 5.0, 0.1, 0.0 },
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
    physicsCalculator::GravityCalculator calc(env, particles, ptypes, false);

    // Initialize the velocity to the expected values
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
        EXPECT_FLOAT_EQ(calc.get_container().get_type_descriptor(i).get_mass(), ptypes[i].get_mass())
            << "The mass must not change when updating the velocity.";
        EXPECT_EQ(pi->getType(), particles[i].getType()) << "The type must not change when updating the velocity.";

        // Test if the new velocity is correct
        EXPECT_LT(ArrayUtils::L2Norm(pi->getV() - expected_v[i]), error_margin)
            << "The velocity was not correct. (expected: " << ArrayUtils::to_string(expected_v[i]) << ", got: " << ArrayUtils::to_string(pi->getV())
            << ")";

        pi++;
    }
}

// Test if update old f works for handcrafted values
TEST(Calculator, UpdateOldF1) {
    // Set the margin for the maximum floatingpoint error
    const double error_margin = 1E-9;

    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 0.1, 1.3, -2.0 }, { 7.0, 2.0, -0.5 }, 2),
        Particle({ 2.5, -3.0, -0.5 }, { -3.0, 1.5, -1.5 }, 1),
        Particle({ -2.0, 3.0, 3.5 }, { 1.0, -2.0, 0.5 }, 1),
        Particle({ 0.1, -1.5, 2.0 }, { 3.0, 2.0, -0.5 }, 0),
    };
    // Initialise the list of type descriptors
    std::vector<TypeDesc> ptypes = {
        TypeDesc { 4.0, 1.0, 5.0, 0.1, 0.0 },
        TypeDesc { 1.0, 1.0, 5.0, 0.1, 0.0 },
        TypeDesc { 2.0, 1.0, 5.0, 0.1, 0.0 },
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
    physicsCalculator::GravityCalculator calc(env, particles, ptypes, false);

    // Perform a single calculateOldF
    ASSERT_NO_THROW(calc.calculateOldF());

    // Make sure that there are no unwanted changes
    ASSERT_EQ(particles.size(), calc.get_container().size()) << "The number of particles must not change when updating the particles old force.";

    auto pi = calc.get_container().begin();
    const std::array<double, 3> zero_v = { 0.0, 0.0, 0.0 };

    for (size_t i = 0; i < particles.size(); i++) {
        EXPECT_TRUE(pi->getX() == particles[i].getX()) << "The position must not change when updating the old force.";
        EXPECT_TRUE(pi->getV() == particles[i].getV()) << "The velocity must not change when updating the old force.";
        EXPECT_FLOAT_EQ(calc.get_container().get_type_descriptor(i).get_mass(), ptypes[i].get_mass())
            << "The mass must not change when updating the old force.";
        EXPECT_EQ(pi->getType(), particles[i].getType()) << "The type must not change when updating the old force.";

        // Test if the new forces are correct
        EXPECT_TRUE(pi->getF() == zero_v) << "The current force should be reset.";
        EXPECT_TRUE(pi->getOldF() == particles[i].getF()) << "The old force should be updated.";

        pi++;
    }
}

// Test if update f works for a single mass point
TEST(GravityCalculator, UpdateF1) {
    //  Set the margin for the maximum floatingpoint error
    const double error_margin = 1E-9;

    // // Initialize the list of particles
    std::vector<Particle> particles = { Particle({ 0.1, 1.3, -2.0 }, { 7.0, 2.0, -0.5 }, 0) };
    // Initialise the list of type descriptors
    std::vector<TypeDesc> ptypes = { TypeDesc { 2.0, 1.0, 5.0, 0.1, 0.0 } };

    particles[0].setF({ -1.0, 2.0, 1.0 });
    particles[0].setOldF({ -0.5, -3.0, 2.0 });

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
    physicsCalculator::GravityCalculator calc(env, particles, ptypes, false);

    const std::array<double, 3> zero_v = { 0.0, 0.0, 0.0 };

    // Perform a single calculateF
    ASSERT_NO_THROW(calc.calculateF());

    // Make sure that there are no unwanted changes
    ASSERT_EQ(particles.size(), calc.get_container().size()) << "The number of particles must not change when updating the particles force.";

    auto pi = calc.get_container().begin();

    EXPECT_TRUE(pi->getX() == particles[0].getX()) << "The positions must not change when updating the force.";
    EXPECT_TRUE(pi->getV() == particles[0].getV()) << "The velocity must not change when updating the force.";
    EXPECT_TRUE(pi->getOldF() == particles[0].getOldF()) << "The old force must not change when updating the force.";
    EXPECT_FLOAT_EQ(calc.get_container().get_type_descriptor(0).get_mass(), ptypes[0].get_mass())
        << "The mass must not change when updating the force.";
    EXPECT_EQ(pi->getType(), particles[0].getType()) << "The type must not change when updating the force.";

    // Test if the new force is the same as the force before calculateF(), as the sum of the new forces are zero but the force value has not been
    // reset by setOldF()
    EXPECT_TRUE(pi->getF() == particles[0].getF()) << "The current force should not have changed.";
}

TEST(Calculator, UpdateF2) {
    //  Set the margin for the maximum floatingpoint error
    const double error_margin = 1E-9;

    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 0.0, 0.0, 0.0 }, { 7.0, 2.0, -0.5 }, 1),
        Particle({ 1.0, 1.0, 1.0 }, { -3.0, 1.5, -1.5 }, 0),
        Particle({ -1.0, -1.0, -1.0 }, { 1.0, -2.0, 0.5 }, 0),
    };
    // Initialise the list of type descriptors
    std::vector<TypeDesc> ptypes = {
        TypeDesc { 1.0, 1.0, 5.0, 0.1, 0.0 },
        TypeDesc { 2.0, 1.0, 5.0, 0.1, 0.0 },
    };

    particles[0].setF({ 0.0, 0.0, 0.0 });
    particles[0].setOldF({ -0.5, -3.0, 2.0 });

    particles[1].setF({ 1.0, 1.0, 1.0 });
    particles[1].setOldF({ 0.5, 1.5, -2.0 });

    particles[2].setF({ -1.0, -1.0, -1.0 });
    particles[2].setOldF({ -1.5, -1.5, -2.0 });

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
    physicsCalculator::GravityCalculator calc(env, particles, ptypes, false);

    // Initialize the force to the expected values
    const std::vector<std::array<double, 3>> expected_f = {
        { 0.0, 0.0, 0.0 },
        { 0.5669872981, 0.5669872981, 0.5669872981 },
        { -0.5669872981, -0.5669872981, -0.5669872981 },
    };

    // Perform a single calculateF
    ASSERT_NO_THROW(calc.calculateF());

    // Make sure that there are no unwanted changes
    ASSERT_EQ(particles.size(), calc.get_container().size()) << "The number of particles must not change when updating the particles force.";

    auto pi = calc.get_container().begin();

    for (size_t i = 0; i < particles.size(); i++) {
        EXPECT_TRUE(pi->getX() == particles[i].getX()) << "The positions must not change when updating the force.";
        EXPECT_TRUE(pi->getV() == particles[i].getV()) << "The velocity must not change when updating the force.";
        EXPECT_TRUE(pi->getOldF() == particles[i].getOldF()) << "The old force must not change when updating the force.";
        EXPECT_FLOAT_EQ(calc.get_container().get_type_descriptor(i).get_mass(), ptypes[i].get_mass())
            << "The mass must not change when updating the force.";
        EXPECT_EQ(pi->getType(), particles[i].getType()) << "The type must not change when updating the force.";

        // Test if the new force is correct
        EXPECT_LT(ArrayUtils::L2Norm(pi->getF() - expected_f[i]), error_margin)
            << "The force was not correct. (expected: " << ArrayUtils::to_string(expected_f[i]) << ", got: " << ArrayUtils::to_string(pi->getF())
            << ")";

        pi++;
    }
}
