#include <gtest/gtest.h>
#include <physicsCalculator/LJSmoothCalculator.h>

// Test if the forces between two particles within the unscaled part are computed correctly
TEST(LJSmoothCalculatorTest, Unscaled) {
    //  Set the margin for the maximum floatingpoint error
    const double error_margin = 1E-9;

    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 0.0, 0.0, 0.0 }, { 7.0, 2.0, -0.5 }, 0),
        Particle({ 1.0, 0.0, 0.0 }, { -3.0, 1.5, -1.5 }, 0),
    };

    // Initialise the list of type descriptors
    std::vector<TypeDesc> ptypes = {
        TypeDesc(1.0, 1.0, 1.0, 1.0, 0.0),
    };

    particles[0].setF({ 0.0, 0.0, 0.0 });
    particles[0].setOldF({ -0.5, -3.0, 2.0 });

    particles[1].setF({ 1.0, 1.0, 1.0 });
    particles[1].setOldF({ 0.5, 1.5, -2.0 });

    // Initialize the simulation environment
    const char* argv[] = {
        "./MolSim",
        "path/to/input.txt",
        "-delta_t=1.0",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);
    Environment env;

    ASSERT_NO_THROW(env = Environment(argc, argv));

    // Initialize the Calculator
    physicsCalculator::LJSmoothCalculator calc(env, particles, 1.1, ptypes, false);

    // Initialize the force to the expected values
    const std::vector<Vec<double>> expected_f = {
        { -24.0, 0.0, 0.0 },
        { 25.0, 1.0, 1.0 },
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
        EXPECT_FLOAT_EQ(calc.get_container().get_type_descriptor(particles[i].getType()).get_mass(), ptypes[particles[i].getType()].get_mass())
            << "The mass must not change when updating the force.";
        EXPECT_EQ(pi->getType(), particles[i].getType()) << "The type must not change when updating the force.";

        // Test if the new force is correct
        EXPECT_LT((pi->getF() - expected_f[i]).len(), error_margin)
            << "The force was not correct. (expected: " << expected_f[i] << ", got: " << pi->getF() << ")";

        pi++;
    }
}

// Test if the forces between two particles within the scaled part are computed correctly
TEST(LJSmoothCalculatorTest, Scaled) {
    //  Set the margin for the maximum floatingpoint error
    const double error_margin = 1E-9;

    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 0.0, 0.0, 0.0 }, { 7.0, 2.0, -0.5 }, 0),
        Particle({ 1.0, 0.0, 0.0 }, { -3.0, 1.5, -1.5 }, 0),
    };

    // Initialise the list of type descriptors
    std::vector<TypeDesc> ptypes = {
        TypeDesc(1.0, 1.0, 1.0, 1.0, 0.0),
    };

    particles[0].setF({ 0.0, 0.0, 0.0 });
    particles[0].setOldF({ -0.5, -3.0, 2.0 });

    particles[1].setF({ 1.0, 1.0, 1.0 });
    particles[1].setOldF({ 0.5, 1.5, -2.0 });

    // Initialize the simulation environment
    const char* argv[] = {
        "./MolSim",
        "path/to/input.txt",
        "-delta_t=1.0",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);
    Environment env;

    ASSERT_NO_THROW(env = Environment(argc, argv));
    env.set_r_cutoff(10.0);

    ASSERT_EQ(env.get_r_cutoff(), 10.0) << "The r_cutoff must be set correctly.";

    // Initialize the Calculator
    physicsCalculator::LJSmoothCalculator calc(env, particles, 0.1, ptypes, false);

    // Initialize the force to the expected values
    const std::vector<Vec<double>> expected_f = {
        { 23.441021788129226145755071, 0.0, 0.0 },
        { -22.441021788129226145755071, 1.0, 1.0 },
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
        EXPECT_FLOAT_EQ(calc.get_container().get_type_descriptor(particles[i].getType()).get_mass(), ptypes[particles[i].getType()].get_mass())
            << "The mass must not change when updating the force.";
        EXPECT_EQ(pi->getType(), particles[i].getType()) << "The type must not change when updating the force.";

        // Test if the new force is correct
        EXPECT_LT((pi->getF() - expected_f[i]).len(), error_margin)
            << "The force was not correct. (expected: " << expected_f[i] << ", got: " << pi->getF() << ")";

        pi++;
    }
}

// Test if the forces between two particles within the zero part are computed correctly
TEST(LJSmoothCalculatorTest, Scaled2) {
    //  Set the margin for the maximum floatingpoint error
    const double error_margin = 1E-9;

    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 3.0, 0.0, 0.0 }, { 7.0, 2.0, -0.5 }, 0),
        Particle({ 14.0, 0.0, 0.0 }, { -3.0, 1.5, -1.5 }, 0),
    };

    // Initialise the list of type descriptors
    std::vector<TypeDesc> ptypes = {
        TypeDesc(2.0, 1.0, 0.5, 1.0, 0.0),
    };

    particles[0].setF({ 0.0, 0.0, 0.0 });
    particles[0].setOldF({ -0.5, -3.0, 2.0 });

    particles[1].setF({ 1.0, 1.0, 1.0 });
    particles[1].setOldF({ 0.5, 1.5, -2.0 });

    // Initialize the simulation environment
    const char* argv[] = {
        "./MolSim",
        "path/to/input.txt",
        "-delta_t=1.0",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);
    Environment env;

    ASSERT_NO_THROW(env = Environment(argc, argv));
    env.set_r_cutoff(10.0);

    ASSERT_EQ(env.get_r_cutoff(), 10.0) << "The r_cutoff must be set correctly.";

    // Initialize the Calculator
    physicsCalculator::LJSmoothCalculator calc(env, particles, 0.1, ptypes, false);

    // Initialize the force to the expected values
    const std::vector<Vec<double>> expected_f = {
        { 0.0, 0.0, 0.0 },
        { 1.0, 1.0, 1.0 },
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
        EXPECT_FLOAT_EQ(calc.get_container().get_type_descriptor(particles[i].getType()).get_mass(), ptypes[particles[i].getType()].get_mass())
            << "The mass must not change when updating the force.";
        EXPECT_EQ(pi->getType(), particles[i].getType()) << "The type must not change when updating the force.";

        // Test if the new force is correct
        EXPECT_LT((pi->getF() - expected_f[i]).len(), error_margin)
            << "The force was not correct. (expected: " << expected_f[i] << ", got: " << pi->getF() << ")";

        pi++;
    }
}
