#include <ParticleGenerator.h>
#include <boundaries/GhostBoundary.h>
#include <boundaries/HardBoundary.h>
#include <boundaries/NoBoundary.h>
#include <boundaries/Stepper.h>
#include <gtest/gtest.h>
#include <physicsCalculator/GravityCalculator.h>
#include <physicsCalculator/LJCalculator.h>

// ==================================================================================================
//
//      Perform tests for a single step
//
// ==================================================================================================

// Test if step works for handcrafted values
TEST(GravityStepper, Step1) {
    // Set the margin for the maximum floatingpoint error
    const double error_margin = 1E-9;

    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ -0.1, -0.1, -0.1 }, { 1.0, 1.0, 1.0 }, 1),
        Particle({ 0.895, 0.995, 0.995 }, { 1.0, 0.0, 0.0 }, 0),
        Particle({ -0.895, -0.995, -0.995 }, { -1.0, 0.0, 0.0 }, 0),
    };
    // Initialise the list of type descriptors
    std::vector<TypeDesc> ptypes = {
        TypeDesc { 1.0, 1.0, 5.0, 0.1, 0.0 },
        TypeDesc { 2.0, 1.0, 5.0, 0.1, 0.0 },
    };

    particles[0].setF({ 0.0, 0.0, 0.0 });

    particles[1].setF({ 1.0, 1.0, 1.0 });

    particles[2].setF({ -1.0, -1.0, -1.0 });


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
    Stepper stepper({ INF_CONT, INF_CONT, INF_CONT, INF_CONT, INF_CONT, INF_CONT }, {});

    // Perform a single step
    ASSERT_NO_THROW(stepper.step(calc, 0));

    std::vector<Particle> exp = {
        Particle({ 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 }, 1),
        Particle({ 1.0, 1.0, 1.0 }, { 1.028349365, 0.02834936491, 0.02834936491 }, 0),
        Particle({ -1.0, -1.0, -1.0 }, { -1.028349365, -0.02834936491, -0.02834936491 }, 0),
    };
    // Initialise the list of type descriptors
    std::vector<TypeDesc> ext = {
        TypeDesc { 1.0, 1.0, 5.0, 0.1, 0.0 },
        TypeDesc { 2.0, 1.0, 5.0, 0.1, 0.0 },
    };

    exp[0].setF({ 0.0, 0.0, 0.0 });
    exp[0].setOldF({ 0.0, 0.0, 0.0 });

    exp[1].setF({ -0.4330127019, -0.4330127019, -0.4330127019 });
    exp[1].setOldF({ 1.0, 1.0, 1.0 });

    exp[2].setF({ 0.4330127019, 0.4330127019, 0.4330127019 });
    exp[2].setOldF({ -1.0, -1.0, -1.0 });


    // Make sure that there are no unwanted changes
    ASSERT_EQ(particles.size(), calc.get_container().size()) << "The number of particles must not change when updating the particles force.";

    auto pi = calc.get_container().begin();

    for (size_t i = 0; i < particles.size(); i++) {
        EXPECT_TRUE(pi->getOldF() == exp[i].getOldF()) << "The old force must not change when updating the force.";
        EXPECT_FLOAT_EQ(ptypes[pi->getType()].get_mass(), ext[exp[i].getType()].get_mass()) << "The mass must not change when updating the force.";
        EXPECT_EQ(pi->getType(), exp[i].getType()) << "The type must not change when updating the force.";

        // Test if the new positions is correct
        EXPECT_LT((pi->getX() - exp[i].getX()).len(), error_margin)
            << "The positions was not correct. (expected: " << exp[i].getX() << ", got: " << pi->getX() << ")";

        // Test if the new velocity is correct
        EXPECT_LT((pi->getV() - exp[i].getV()).len(), error_margin)
            << "The force was not correct. (expected: " << exp[i].getV() << ", got: " << pi->getV() << ")";

        // Test if the new force is correct
        EXPECT_LT((pi->getF() - exp[i].getF()).len(), error_margin)
            << "The force was not correct. (expected: " << exp[i].getF() << ", got: " << pi->getF() << ")";

        pi++;
    }
}

// Test if a single step computes the correct values
TEST(LJStepper, Step1) {
    // Set the margin for the maximum floatingpoint error
    const double error_margin = 1E-9;

    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 4.0, 0.0, 0.0 }, { -1.0, 0.0, 0.0 }, 0),
        Particle({ -4.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 }, 0),
    };
    // Initialise the list of type descriptors
    std::vector<TypeDesc> ptypes = { TypeDesc { 1.0, 1.0, 4.0, 1.0, 0.0 } };

    particles[0].setF({ -0.1, 0.0, 0.0 });
    particles[0].setOldF({ 2.0, 1.0, 2.0 });

    particles[1].setF({ 0.1, 0.0, 0.0 });
    particles[1].setOldF({ -2.0, 1.0, 1.0 });

    // Initialize the simulation environment
    const char* argv[] = {
        "./MolSim",
        "path/to/input.txt",
        "-delta_t=1.0",
        "-sigma=1",
        "-epsilon=4",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);
    Environment env;

    ASSERT_NO_THROW(env = Environment(argc, argv));

    // Initialize the Calculator
    physicsCalculator::LJCalculator calc(env, particles, ptypes, false);

    // Initialize the positions to the expected values
    std::vector<Particle> expected = {
        Particle({ 2.95, 0.0, 0.0 }, { -1.0501928663952, 0.0, 0.0 }, 0),
        Particle({ -2.95, 0.0, 0.0 }, { 1.0501928663952, 0.0, 0.0 }, 0),
    };

    expected[0].setF({ -0.0003857327903, 0.0, 0.0 });
    expected[0].setOldF({ -0.1, 0.0, 0.0 });

    expected[1].setF({ 0.0003857327903, 0.0, 0.0 });
    expected[1].setOldF({ 0.1, 0.0, 0.0 });

    Stepper stepper({ INF_CONT, INF_CONT, INF_CONT, INF_CONT, INF_CONT, INF_CONT }, {});

    // Perform a single step
    ASSERT_NO_THROW(stepper.step(calc, 0));

    // Make sure that there are no unwanted changes
    ASSERT_EQ(expected.size(), calc.get_container().size()) << "The number of particles must not change when updating the particles forces.";

    auto pi = calc.get_container().begin();

    for (size_t i = 0; i < expected.size(); i++) {
        EXPECT_FLOAT_EQ(ptypes[pi->getType()].get_mass(), 1.0) << "The particle mass must not change.";
        EXPECT_EQ(pi->getType(), expected[i].getType()) << "The particle type must not change.";

        EXPECT_LT((pi->getX() - expected[i].getX()).len(), error_margin) << "The particle must have the new position.";
        EXPECT_LT((pi->getV() - expected[i].getV()).len(), error_margin) << "The particle must have the new velocity.";
        EXPECT_LT((pi->getF() - expected[i].getF()).len(), error_margin) << "The particle must have the new force.";
        EXPECT_LT((pi->getOldF() - expected[i].getOldF()).len(), error_margin) << "The particle must have the new old force.";

        pi++;
    }
}

// Test if step works for an empty particle container
TEST(Calculator, StepNo) {
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
    Stepper stepper({ INF_CONT, INF_CONT, INF_CONT, INF_CONT, INF_CONT, INF_CONT }, {});

    // Perform a single step
    ASSERT_NO_THROW(stepper.step(calc, 0));

    EXPECT_EQ(calc.get_container().size(), 0) << "Calculating the velocity on an empty container should not add a particle.";
}

// ==================================================================================================
//
//      Perform analytical tests for the gravity calculator
//
// ==================================================================================================

// Test if step works for an analytical solution using a single moving body
TEST(GravityStepper, Analytical1) {
    // Set the margin for the maximum floatingpoint error
    const double error_margin = 1E-8;

    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 0.5, 2.0, -31.0 }, { 2.0, -0.5, 2.5 }, 0),
    };
    // Initialise the list of type descriptors
    std::vector<TypeDesc> ptypes = { TypeDesc { 2.0, 1.0, 5.0, 0.0001, 0.0 } };

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
    physicsCalculator::GravityCalculator calc(env, particles, ptypes);
    double total_time = 0.0;

    Stepper stepper({ INF_CONT, INF_CONT, INF_CONT, INF_CONT, INF_CONT, INF_CONT }, {});

    // Perform the steps for 50 time units
    for (size_t i = 0; i <= 500000; i++) {
        // Test that the position is correct
        const Vec<double> expected_pos = { 0.5 + total_time * 2.0, 2.0 + total_time * -0.5, -31.0 + total_time * 2.5 };
        ASSERT_LT((calc.get_container()[0].getX() - expected_pos).len(), error_margin)
            << "The calculation diverged at time step " << i << " (" << total_time << ")";

        ASSERT_NO_THROW(stepper.step(calc, 0));
        total_time += 0.0001;
    }
}

// Test if step works for an analytical solution using two bodies dancing around a shared center of mass, while the center of mass is moving
TEST(GravityStepper, Analytical2) {
    // Set the margin for the maximum floatingpoint error
    const double error_margin = 1E-6;

    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 3.0, 1.0, 3.0 }, { 0.1, 1.0, -0.05 }, 0),
        Particle({ 1.0, 1.0, 3.0 }, { 0.1, -1.0, -0.05 }, 0),
    };
    // Initialise the list of type descriptors
    std::vector<TypeDesc> ptypes = { TypeDesc { 4.0, 1.0, 5.0, 0.0001, 0.0 } };

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
    physicsCalculator::GravityCalculator calc(env, particles, ptypes);
    double total_time = 0.0;
    double time_mod_two_pi = 0.0;
    constexpr double two_pi = static_cast<double>(M_PIl * 2.0L);
    Stepper stepper({ INF_CONT, INF_CONT, INF_CONT, INF_CONT, INF_CONT, INF_CONT }, {});

    // Perform the steps for 100 time units
    for (size_t i = 0; i <= 1000000; i++) {
        // Test that the position is correct
        const Vec<double> expected_pos_1 = {
            2.0 + std::cos(time_mod_two_pi) + total_time * 0.1,
            1.0 + std::sin(time_mod_two_pi),
            3.0 + total_time * -0.05,
        };

        const Vec<double> expected_pos_2 = {
            2.0 + -std::cos(time_mod_two_pi) + total_time * 0.1,
            1.0 + -std::sin(time_mod_two_pi),
            3.0 + total_time * -0.05,
        };

        ASSERT_LT((calc.get_container()[0].getX() - expected_pos_1).len(), error_margin)
            << "The calculation diverged at time step " << i << " (" << total_time << ") for particle 1.";
        ASSERT_LT((calc.get_container()[1].getX() - expected_pos_2).len(), error_margin)
            << "The calculation diverged at time step " << i << " (" << total_time << ") for particle 2.";

        ASSERT_NO_THROW(stepper.step(calc, 0));
        total_time += 0.0001;
        time_mod_two_pi += 0.0001;

        if (time_mod_two_pi >= two_pi) {
            time_mod_two_pi -= two_pi;
        }
    }
}

// Test if step works for an analytical solution using two bodies dancing around a third body in the center of mass, while the center of mass is
// moving
TEST(GravityStepper, Analytical3) {
    // Set the margin for the maximum floatingpoint error
    const double error_margin = 1E-4;

    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 2.0, -1.0, 2.0 }, { 0.01, 1.05, -0.05 }, 0),
        Particle({ 0.0, -1.0, 2.0 }, { 0.01, -0.95, -0.05 }, 0),
        Particle({ 1.0, -1.0, 2.0 }, { 0.01, 0.05, -0.05 }, 1),
    };
    // Initialise the list of type descriptors
    std::vector<TypeDesc> ptypes = {
        TypeDesc { 1.0, 1.0, 5.0, 0.0001, 0.0 },
        TypeDesc { 0.75, 1.0, 5.0, 0.0001, 0.0 },
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
    physicsCalculator::GravityCalculator calc(env, particles, ptypes);
    double total_time = 0.0;
    double time_mod_two_pi = 0.0;
    constexpr double two_pi = static_cast<double>(M_PIl * 2.0L);
    Stepper stepper({ INF_CONT, INF_CONT, INF_CONT, INF_CONT, INF_CONT, INF_CONT }, {});

    // Perform the steps for 10 time units
    for (size_t i = 0; i <= 100000; i++) {
        // Test that the position is correct
        const Vec<double> expected_pos_1 = {
            1.0 + std::cos(time_mod_two_pi) + total_time * 0.01,
            -1.0 + std::sin(time_mod_two_pi) + total_time * 0.05,
            2.0 + total_time * -0.05,
        };

        const Vec<double> expected_pos_2 = {
            1.0 + -std::cos(time_mod_two_pi) + total_time * 0.01,
            -1.0 + -std::sin(time_mod_two_pi) + total_time * 0.05,
            2.0 + total_time * -0.05,
        };

        const Vec<double> expected_pos_3 = {
            1.0 + total_time * 0.01,
            -1.0 + total_time * 0.05,
            2.0 + total_time * -0.05,
        };

        ASSERT_LT((calc.get_container()[0].getX() - expected_pos_1).len(), error_margin)
            << "The calculation diverged at time step " << i << " (" << total_time << ") for particle 1.";
        ASSERT_LT((calc.get_container()[1].getX() - expected_pos_2).len(), error_margin)
            << "The calculation diverged at time step " << i << " (" << total_time << ") for particle 2.";
        ASSERT_LT((calc.get_container()[2].getX() - expected_pos_3).len(), error_margin)
            << "The calculation diverged at time step " << i << " (" << total_time << ") for particle 3.";

        ASSERT_NO_THROW(stepper.step(calc, 0));
        total_time += 0.0001;
        time_mod_two_pi += 0.0001;

        if (time_mod_two_pi >= two_pi) {
            time_mod_two_pi -= two_pi;
        }
    }
}

// Test if step works for an analytical solution using two bodies dancing around a shared center of mass, while the center of mass is moving. The
// two bodies have different masses and therefore a different circle radius
TEST(GravityStepper, Analytical4) {
    // Set the margin for the maximum floatingpoint error
    const double error_margin = 1E-6;

    // Initialize the list of particles
    std::vector<Particle> particles = {
        //  1  0  0                 0.1  1  0             18
        // -2  0  0                 0.1 -2  0             9
        Particle({ 1.0, -2.0, 1.0 }, { 1.05, 0.05, 0.01 }, 0),
        Particle({ 1.0, -2.0, -2.0 }, { -1.95, 0.05, 0.01 }, 1),
    };
    // Initialise the list of type descriptors
    std::vector<TypeDesc> ptypes = {
        TypeDesc { 18.0, 1.0, 5.0, 0.0001, 0.0 },
        TypeDesc { 9.0, 1.0, 5.0, 0.0001, 0.0 },
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
    physicsCalculator::GravityCalculator calc(env, particles, ptypes);
    double total_time = 0.0;
    double time_mod_two_pi = 0.0;
    constexpr double two_pi = static_cast<double>(M_PIl * 2.0L);
    Stepper stepper({ INF_CONT, INF_CONT, INF_CONT, INF_CONT, INF_CONT, INF_CONT }, {});

    // Perform the steps for 100 time units
    for (size_t i = 0; i <= 1000000; i++) {
        // Test that the position is correct
        const Vec<double> expected_pos_1 = {
            1.0 + std::sin(time_mod_two_pi) + total_time * 0.05,
            -2.0 + total_time * 0.05,
            0.0 + std::cos(time_mod_two_pi) + total_time * 0.01,
        };

        const Vec<double> expected_pos_2 = {
            1.0 + -2.0 * std::sin(time_mod_two_pi) + total_time * 0.05,
            -2.0 + total_time * 0.05,
            0.0 + -2.0 * std::cos(time_mod_two_pi) + total_time * 0.01,
        };

        ASSERT_LT((calc.get_container()[0].getX() - expected_pos_1).len(), error_margin)
            << "The calculation diverged at time step " << i << " (" << total_time << ") for particle 1.";
        ASSERT_LT((calc.get_container()[1].getX() - expected_pos_2).len(), error_margin)
            << "The calculation diverged at time step " << i << " (" << total_time << ") for particle 2.";

        ASSERT_NO_THROW(stepper.step(calc, 0));
        total_time += 0.0001;
        time_mod_two_pi += 0.0001;

        if (time_mod_two_pi >= two_pi) {
            time_mod_two_pi -= two_pi;
        }
    }
}

// ==================================================================================================
//
//      Perform analytical tests for the Lennard Jones calculator
//
// ==================================================================================================

// Test if step works for an analytical solution using a single moving body
TEST(LJStepper, Analytical1) {
    // Set the margin for the maximum floatingpoint error
    const double error_margin = 1E-9;

    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 1.0, 10.0, -25.0 }, { 0.5, -0.5, 0.2 }, 0),
    };
    // Initialise the list of type descriptors
    std::vector<TypeDesc> ptypes = { TypeDesc { 3.3, 1.0, 5.0, 0.0001, 0.0 } };

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
    physicsCalculator::LJCalculator calc(env, particles, ptypes);
    double total_time = 0.0;
    Stepper stepper({ INF_CONT, INF_CONT, INF_CONT, INF_CONT, INF_CONT, INF_CONT }, {});

    // Perform the steps for 50 time units
    for (size_t i = 0; i <= 500000; i++) {
        // Test that the position is correct
        const Vec<double> expected_pos = { 1.0 + total_time * 0.5, 10.0 + total_time * -0.5, -25.0 + total_time * 0.2 };
        ASSERT_LT((calc.get_container()[0].getX() - expected_pos).len(), error_margin)
            << "The calculation diverged at time step " << i << " (" << total_time << ") (Expected: " << expected_pos
            << ", Got: " << calc.get_container()[0].getX() << ")";

        ASSERT_NO_THROW(stepper.step(calc, 0));
        total_time += 0.0001;
    }
}

// Test if step works for an analytical solution using two bodies circling around their shared center of gravity on the same circle. Meanwhile the
// center of gravity is slowly moving.
TEST(LJStepper, Analytical2) {
    // Set the margin for the maximum floatingpoint error
    const double error_margin = 1E-6;

    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 6.3, 2.2, -1.5 }, { 0.02, 0.99, 0.03 }, 0),
        Particle({ -3.7, 2.2, -1.5 }, { 0.02, -1.01, 0.03 }, 0),
    };
    // Initialise the list of type descriptors
    std::vector<TypeDesc> ptypes = {
        TypeDesc { 365346816.0, 8.0, 244140625.0, 0.0001, 0.0 },
    };

    // Initialize the simulation environment
    const char* argv[] = {
        "./MolSim",
        "path/to/input.txt",
        "-delta_t=0.0001",
        "-sigma=8.0",
        "-epsilon=244140625.0",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);
    Environment env;

    ASSERT_NO_THROW(env = Environment(argc, argv));

    // Initialize the Calculator
    physicsCalculator::LJCalculator calc(env, particles, ptypes);
    double total_time = 0.0;
    Stepper stepper({ INF_CONT, INF_CONT, INF_CONT, INF_CONT, INF_CONT, INF_CONT }, {});

    // Perform the steps for 100 time units
    for (size_t i = 0; i <= 1000000; i++) {
        // Test that the position is correct
        const Vec<double> expected_pos_0 = {
            5.0 * std::cos(total_time / 5.0) + total_time * 0.02 + 1.3,
            5.0 * std::sin(total_time / 5.0) + total_time * -0.01 + 2.2,
            -1.5 + total_time * 0.03,
        };

        const Vec<double> expected_pos_1 = {
            -5.0 * std::cos(total_time / 5.0) + total_time * 0.02 + 1.3,
            -5.0 * std::sin(total_time / 5.0) + total_time * -0.01 + 2.2,
            -1.5 + total_time * 0.03,
        };

        ASSERT_LT((calc.get_container()[0].getX() - expected_pos_0).len(), error_margin)
            << "The calculation diverged at time step " << i << " (" << total_time << ") (Expected: " << expected_pos_0
            << ", Got: " << calc.get_container()[0].getX() << ")";
        ASSERT_LT((calc.get_container()[1].getX() - expected_pos_1).len(), error_margin)
            << "The calculation diverged at time step " << i << " (" << total_time << ") (Expected: " << expected_pos_1
            << ", Got: " << calc.get_container()[1].getX() << ")";

        ASSERT_NO_THROW(stepper.step(calc, 0));
        total_time += 0.0001;
    }
}

// Test if step works for an analytical solution using two bodies circling around their shared center of gravity. However this time the two circles
// have two different radiuses.
TEST(LJStepper, Analytical3) {
    // Set the margin for the maximum floatingpoint error
    const double error_margin = 1E-6;

    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 0.0, 0.0, 5.0 }, { 0.0, 1.0, 0.0 }, 0),
        Particle({ 0.0, 0.0, -10.0 }, { 0.0, -2.0, 0.0 }, 1),
    };
    // Initialise the list of type descriptors
    std::vector<TypeDesc> ptypes = {
        TypeDesc { 243564544.0, 12.0, 244140625.0, 0.0001, 0.0 },
        TypeDesc { 121782272.5, 12.0, 244140625.0, 0.0001, 0.0 },
    };

    // Initialize the simulation environment
    const char* argv[] = {
        "./MolSim",
        "path/to/input.txt",
        "-delta_t=0.0001",
        "-sigma=12.0",
        "-epsilon=244140625.0",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);
    Environment env;

    ASSERT_NO_THROW(env = Environment(argc, argv));

    // Initialize the Calculator
    physicsCalculator::LJCalculator calc(env, particles, ptypes);
    double total_time = 0.0;
    Stepper stepper({ INF_CONT, INF_CONT, INF_CONT, INF_CONT, INF_CONT, INF_CONT }, {});

    // Perform the steps for 50 time units
    for (size_t i = 0; i <= 500000; i++) {
        // Test that the position is correct
        const Vec<double> expected_pos_0 = {
            0.0,
            5.0 * std::sin(total_time / 5.0),
            5.0 * std::cos(total_time / 5.0),
        };

        const Vec<double> expected_pos_1 = {
            0.0,
            -10.0 * std::sin(total_time / 5.0),
            -10.0 * std::cos(total_time / 5.0),
        };

        ASSERT_LT((calc.get_container()[0].getX() - expected_pos_0).len(), error_margin)
            << "The calculation diverged at time step " << i << " (" << total_time << ") (Expected: " << expected_pos_0
            << ", Got: " << calc.get_container()[0].getX() << ")";
        ASSERT_LT((calc.get_container()[1].getX() - expected_pos_1).len(), error_margin)
            << "The calculation diverged at time step " << i << " (" << total_time << ") (Expected: " << expected_pos_1
            << ", Got: " << calc.get_container()[1].getX() << ")";

        ASSERT_NO_THROW(stepper.step(calc, 0));
        total_time += 0.0001;
    }
}

// Test if step works for an analytical solution using three bodies which are completely stationary relative to each other. The shared center of
// gravity is also moving.
TEST(LJStepper, Analytical4) {
    // Set the margin for the maximum floatingpoint error
    const double error_margin = 1E-5;

    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle(
            {
                1.7480669497636142554225485204397801807855281376745671279006682579,
                -1.351933050236385744577451479560219819214471862325432872099331742,
                1.1480669497636142554225485204397801807855281376745671279006682579,
            },
            { 0.001, 0.001, -0.002 }, 1.0),
        Particle({ 1.1, -2.0, 0.5 }, { 0.001, 0.001, -0.002 }, 0),
        Particle(
            {
                0.4519330502363857445774514795602198192144718623254328720993317420,
                -2.6480669497636142554225485204397801807855281376745671279006682579,
                -0.148066949763614255422548520439780180785528137674567127900668257,
            },
            { 0.001, 0.001, -0.002 }, 1),
    };
    // Initialise the list of type descriptors
    std::vector<TypeDesc> ptypes = {
        TypeDesc { 1.5, 1.0012978649056434718130942322111430509313330669648478791579796678, 4.0, 0.0001, 0.0 },
        TypeDesc { 2.0, 1.0012978649056434718130942322111430509313330669648478791579796678, 4.0, 0.0001, 0.0 },
    };

    // Initialize the simulation environment
    const char* argv[] = {
        "./MolSim",
        "path/to/input.txt",
        "-delta_t=0.0001",
        "-sigma=1.0012978649056434718130942322111430509313330669648478791579796678",
        "-epsilon=4.0",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);
    Environment env;

    ASSERT_NO_THROW(env = Environment(argc, argv));

    // Initialize the Calculator
    physicsCalculator::LJCalculator calc(env, particles, ptypes);
    double total_time = 0.0;
    Stepper stepper({ INF_CONT, INF_CONT, INF_CONT, INF_CONT, INF_CONT, INF_CONT }, {});

    // Perform the steps for 20 time units
    for (size_t i = 0; i <= 200000; i++) {
        // Test that the position is correct
        const Vec<double> expected_pos_0 = {
            1.7480669497636142554225485204397801807855281376745671279006682579 + total_time * 0.001,
            -1.351933050236385744577451479560219819214471862325432872099331742 + total_time * 0.001,
            1.1480669497636142554225485204397801807855281376745671279006682579 + total_time * -0.002,
        };

        const Vec<double> expected_pos_1 = {
            1.1 + total_time * 0.001,
            -2.0 + total_time * 0.001,
            0.5 + total_time * -0.002,
        };

        const Vec<double> expected_pos_2 = {
            0.4519330502363857445774514795602198192144718623254328720993317420 + total_time * 0.001,
            -2.6480669497636142554225485204397801807855281376745671279006682579 + total_time * 0.001,
            -0.148066949763614255422548520439780180785528137674567127900668257 + total_time * -0.002,
        };

        ASSERT_LT((calc.get_container()[0].getX() - expected_pos_0).len(), error_margin)
            << "The calculation diverged at time step " << i << " (" << total_time << ") (Expected: " << expected_pos_0
            << ", Got: " << calc.get_container()[0].getX() << ")";
        ASSERT_LT((calc.get_container()[1].getX() - expected_pos_1).len(), error_margin)
            << "The calculation diverged at time step " << i << " (" << total_time << ") (Expected: " << expected_pos_1
            << ", Got: " << calc.get_container()[1].getX() << ")";
        ASSERT_LT((calc.get_container()[2].getX() - expected_pos_2).len(), error_margin)
            << "The calculation diverged at time step " << i << " (" << total_time << ") (Expected: " << expected_pos_2
            << ", Got: " << calc.get_container()[2].getX() << ")";

        ASSERT_NO_THROW(stepper.step(calc, 0));
        total_time += 0.0001;
    }
}

// ==================================================================================================
//
//      Perform analytical tests for the boundary conditions
//
// ==================================================================================================

// Test that the stepper works correctly with multiple boundary conditions. It tests the outflow and the hard reflection condition, as well as the
// cutoff distance implementation. Therefor it uses one particle escaping out of the domain and two particle running in a rectangular shape.
TEST(Stepper, MultipleBoundaries) {
    // Set the margin for the maximum floatingpoint error
    const double error_margin = 1E-5;

    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 3.0, 0.0, 5.0 }, { -1.0, -1.0, 0.0 }, 0),
        Particle({ 7.0, 10.0, 5.0 }, { 1.0, 1.0, 0.0 }, 0),
        Particle({ 5.0, 5.0, 5.0 }, { 0.0, 0.0, 1.0 }, 0),
    };
    // Initialise the list of type descriptors
    std::vector<TypeDesc> ptypes = {
        TypeDesc { 1.0, 1.0, 5.0, 0.1, 0.0 },
    };

    particles[0].setF({ 0.0, 0.0, 0.0 });
    particles[0].setOldF({ 0.0, 0.0, 0.0 });
    particles[1].setF({ 0.0, 0.0, 0.0 });
    particles[1].setOldF({ 0.0, 0.0, 0.0 });

    // Initialize the simulation environment
    const char* argv[] = {
        "./MolSim",
        "path/to/input.txt",
        "-delta_t=0.1",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);
    Environment env;

    ASSERT_NO_THROW(env = Environment(argc, argv));
    env.set_r_cutoff(2.0);
    env.set_domain_size({ 10.0, 10.0, 10.0 });

    // Initialize the Calculator
    physicsCalculator::LJCalculator calc(env, particles, ptypes, false, false);
    Stepper stepper({ HARD, HARD, OUTFLOW, HARD, HARD, OUTFLOW }, { 10.0, 10.0, 10.0 });

    // Perform the steps for 20000 time units
    for (size_t i = 0; i <= 200000; i++) {
        double x_rel = ((i + 170) % 200) / 10.0;

        if (x_rel > 10.0) {
            x_rel = 20.0 - x_rel;
        }

        double y_rel = (i % 200) / 10.0;

        if (y_rel > 10.0) {
            y_rel = 20.0 - y_rel;
        }

        std::vector<Vec<double>> positions = {
            { x_rel, y_rel, 5.0 },
            { 10.0 - x_rel, 10.0 - y_rel, 5.0 },
        };

        if (i <= 50) {
            positions.push_back({ 5.0, 5.0, 5.0 + (i / 10.0) });
        }

        ASSERT_EQ(positions.size(), calc.get_container().size()) << "The particles of the outflow condition weren't removed correctly.";

        for (size_t j = 0; j < positions.size(); j++) {
            ASSERT_LT((calc.get_container()[j].getX() - positions[j]).len(), error_margin)
                << "The calculation diverged at time step " << i << " (Expected: " << positions[i] << ", Got: " << calc.get_container()[i].getX()
                << ") (Particle " << j << ")";
        }

        stepper.step(calc, 0);
    }
}

// Test that particles remain within the domain when using a halo reflecting boundary condition, and that there are no loops over particles with
// illegal distance.
TEST(Stepper, MultipleReflecting) {
    // Initialize the list of particles
    std::vector<Particle> particles = {};

    // Initialize the simulation environment
    const char* argv[] = {
        "./MolSim",
        "path/to/input.txt",
        "-delta_t=0.0001",
        "-sigma=1.0",
        "-epsilon=5.0",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);
    Environment env;

    ASSERT_NO_THROW(env = Environment(argc, argv));
    env.set_r_cutoff(3.0);
    env.set_domain_size({ 21.0, 21.0, 21.0 });

    ParticleGenerator gen;

    // Initialize the Calculator
    physicsCalculator::LJCalculator calc(env, particles, {}, false, false);
    calc.get_container().resize(1000);
    gen.generateCuboid(calc.get_container(), 0, { 3.0, 3.0, 3.0 }, { 0.0, 0.0, 0.0 }, 0, { 10, 10, 10 }, 1.5, 1.0, 3);
    calc.get_container().build_type_table({ TypeDesc { 1.0, 1.0, 5.0, 0.0001, 0.0 } });
    calc.get_container().update_positions();

    Stepper stepper({ HALO, HALO, HARD, HALO, HALO, HARD }, { 21.0, 21.0, 21.0 });

    // Perform the steps for 1 time units
    for (size_t i = 0; i <= 1000; i++) {
        ASSERT_EQ(calc.get_container().size(), 1000) << "The particle container must not leak. (" << i << ")";

        calc.get_container().iterate_pairs([](Particle& p1, Particle& p2) {
            ASSERT_LE((p1.getX() - p2.getX()).len(), 3.0) << "Iterated over a particle pair with illegal spacing.";
        });

        for (const Particle& p : calc.get_container()) {
            ASSERT_GE(p.getX()[0], 0.0) << "A particle must not leave the domain. (" << i << ")";
            ASSERT_GE(p.getX()[1], 0.0) << "A particle must not leave the domain. (" << i << ")";
            ASSERT_GE(p.getX()[2], 0.0) << "A particle must not leave the domain. (" << i << ")";
            ASSERT_LE(p.getX()[0], 21.0) << "A particle must not leave the domain. (" << i << ")";
            ASSERT_LE(p.getX()[1], 21.0) << "A particle must not leave the domain. (" << i << ")";
            ASSERT_LE(p.getX()[2], 21.0) << "A particle must not leave the domain. (" << i << ")";
        }

        stepper.step(calc, 0);
    }
}

// ==================================================================================================
//
//      Perform analytical tests for periodic systems
//
// ==================================================================================================

static inline void shrink_arr(Vec<double>& arr, const Vec<double>& dom) {
    for (size_t i = 0; i < 3; i++) {
        arr[i] = std::fmod(arr[i], dom[i]);

        if (arr[i] < 0.0) {
            arr[i] = dom[i] + arr[i];
        }
    }
}

// Test if a periodic system remains stable for a long lasting simulation.
TEST(Stepper, Periodic1) {
    // Set the margin for the maximum floatingpoint error
    const double error_margin = 1E-6;

    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 0.0, 0.0, 25.0 }, { 3.0, 1.0, 2.0 }, 0),
        Particle({ 0.0, 0.0, 10.0 }, { 3.0, -2.0, 2.0 }, 1),
    };

    // Initialise the list of type descriptors
    std::vector<TypeDesc> ptypes = {
        TypeDesc { 243564544.0, 12.0, 244140625.0, 0.0001, 0.0 },
        TypeDesc { 121782272.5, 12.0, 244140625.0, 0.0001, 0.0 },
    };

    // Initialize the simulation environment
    const char* argv[] = {
        "./MolSim",
        "path/to/input.txt",
        "-delta_t=0.0001",
        "-sigma=12.0",
        "-epsilon=244140625.0",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);
    Environment env;

    ASSERT_NO_THROW(env = Environment(argc, argv));
    env.set_r_cutoff(20.0);
    env.set_domain_size({ 100.0, 100.0, 100.0 });

    // Initialize the Calculator
    physicsCalculator::LJCalculator calc(env, particles, ptypes, true, false);
    double total_time = 0.0;
    Stepper stepper({ PERIODIC, PERIODIC, PERIODIC, PERIODIC, PERIODIC, PERIODIC }, { 100.0, 100.0, 100.0 });

    // Perform the steps for 75 time units
    for (size_t i = 0; i <= 750000; i++) {
        // Test that the position is correct
        Vec<double> expected_pos_0 = {
            3.0 * total_time,
            5.0 * std::sin(total_time / 5.0),
            20.0 + 2.0 * total_time + 5.0 * std::cos(total_time / 5.0),
        };

        shrink_arr(expected_pos_0, env.get_domain_size());

        Vec<double> expected_pos_1 = {
            3.0 * total_time,
            -10.0 * std::sin(total_time / 5.0),
            20.0 + 2.0 * total_time - 10.0 * std::cos(total_time / 5.0),
        };

        shrink_arr(expected_pos_1, env.get_domain_size());

        ASSERT_LT((calc.get_container()[0].getX() - expected_pos_0).len(), error_margin)
            << "The calculation diverged at time step " << i << " (" << total_time << ") (Expected: " << expected_pos_0
            << ", Got: " << calc.get_container()[0].getX() << ")";
        ASSERT_LT((calc.get_container()[1].getX() - expected_pos_1).len(), error_margin)
            << "The calculation diverged at time step " << i << " (" << total_time << ") (Expected: " << expected_pos_1
            << ", Got: " << calc.get_container()[1].getX() << ")";

        ASSERT_NO_THROW(stepper.step(calc, 0));
        total_time += 0.0001;
    }
}


// Test if a periodic system remains stable for a long lasting simulation, if paired with a hard boundary. This uses particles running to edges and
// plains.
TEST(Stepper, PeriodicHard) {
    // Set the margin for the maximum floatingpoint error
    const double error_margin = 1E-6;

    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 0.0, 0.0, 12.0 }, { 4.0, 4.0, 0.0 }, 0),
        Particle({ 60.0, 0.0, 60.0 }, { 0.0, 6.0, 0.0 }, 0),
        Particle({ 40.0, 0.0, 0.0 }, { 0.0, 3.0, 160.0 }, 0),
    };

    // Initialise the list of type descriptors
    std::vector<TypeDesc> ptypes = {
        TypeDesc(1.0, 1.0, 50.0, 0.0, 0.0001),
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
    env.set_r_cutoff(10.0);
    env.set_domain_size({ 79.99999, 79.99999, 80.0 });

    // Initialize the Calculator
    physicsCalculator::LJCalculator calc(env, particles, ptypes, true, false);
    double total_time = 0.0;
    Stepper stepper({ PERIODIC, PERIODIC, HARD, PERIODIC, PERIODIC, HARD }, { 79.99999, 79.99999, 80.0 });

    // Perform the steps for 100 time units
    for (size_t i = 0; i <= 1000000; i++) {
        // Test that the position is correct
        Vec<double> expected_pos_0 = {
            4.0 * total_time,
            4.0 * total_time,
            12.0,
        };

        shrink_arr(expected_pos_0, env.get_domain_size());

        Vec<double> expected_pos_1 = {
            60.0,
            6.0 * total_time,
            60.0,
        };

        shrink_arr(expected_pos_1, env.get_domain_size());

        Vec<double> expected_pos_2 = {
            40.0,
            3.0 * total_time,
            0.0160 * (i % 10000),
        };

        if (expected_pos_2[2] > 80.0) {
            expected_pos_2[2] = 160.0 - expected_pos_2[2];
        }

        shrink_arr(expected_pos_2, Vec<double>({ 79.99999, 79.99999, 81.0 }));

        ASSERT_LT((calc.get_container()[0].getX() - expected_pos_0).len(), error_margin)
            << "The calculation diverged at time step " << i << " (" << total_time << ") (Expected: " << expected_pos_0
            << ", Got: " << calc.get_container()[0].getX() << ")";

        ASSERT_LT((calc.get_container()[1].getX() - expected_pos_1).len(), error_margin)
            << "The calculation diverged at time step " << i << " (" << total_time << ") (Expected: " << expected_pos_1
            << ", Got: " << calc.get_container()[1].getX() << ")";

        ASSERT_LT((calc.get_container()[2].getX() - expected_pos_2).len(), error_margin)
            << "The calculation diverged at time step " << i << " (" << total_time << ") (Expected: " << expected_pos_2
            << ", Got: " << calc.get_container()[2].getX() << ")";

        ASSERT_NO_THROW(stepper.step(calc, 0));
        total_time += 0.0001;
    }
}
