#include <ParticleGenerator.h>
#include <container/BoxContainer.h>
#include <gtest/gtest.h>
#include <physicsCalculator/MolCalculator.h>

// Test if a molecule grid (3x3) is handled correctly, if it is balanced.
TEST(MolCalculator, calculateMolecules) {
    //  Set the margin for the maximum floatingpoint error
    const double error_margin = 1E-9;

    ParticleGenerator gen;
    std::vector<TypeDesc> desc = {
        TypeDesc(1.0, 0.1, 1.0, 1.0, 0.0, 1.0, 1.0),
    };

    std::vector<Particle> particles = {
        Particle({ 50.5, 50.5, 50.5 }, {}),
        Particle({ 51.5, 50.5, 50.5 }, {}),
        Particle({ 52.5, 50.5, 50.5 }, {}),
        Particle({ 50.5, 51.5, 50.5 }, {}),
        Particle({ 51.5, 51.5, 50.5 }, {}),
        Particle({ 52.5, 51.5, 50.5 }, {}),
        Particle({ 50.5, 52.5, 50.5 }, {}),
        Particle({ 51.5, 52.5, 50.5 }, {}),
        Particle({ 52.5, 52.5, 50.5 }, {}),
    };

    particles[0].setNeighbors({ 1, 3, SIZE_MAX, SIZE_MAX, 4, SIZE_MAX, SIZE_MAX, SIZE_MAX });
    particles[1].setNeighbors({ 0, 2, 4, SIZE_MAX, 3, 5, SIZE_MAX, SIZE_MAX });
    particles[2].setNeighbors({ 1, 5, SIZE_MAX, SIZE_MAX, 4, SIZE_MAX, SIZE_MAX, SIZE_MAX });
    particles[3].setNeighbors({ 0, 4, 6, SIZE_MAX, 1, 7, SIZE_MAX, SIZE_MAX });
    particles[4].setNeighbors({ 1, 3, 5, 7, 0, 2, 6, 8 });
    particles[5].setNeighbors({ 2, 4, 8, SIZE_MAX, 1, 7, SIZE_MAX, SIZE_MAX });
    particles[6].setNeighbors({ 3, 7, SIZE_MAX, SIZE_MAX, 4, SIZE_MAX, SIZE_MAX, SIZE_MAX });
    particles[7].setNeighbors({ 4, 6, 8, SIZE_MAX, 3, 5, SIZE_MAX, SIZE_MAX });
    particles[8].setNeighbors({ 5, 7, SIZE_MAX, SIZE_MAX, 4, SIZE_MAX, SIZE_MAX, SIZE_MAX });

    for (size_t i = 0; i < particles.size(); i++) {
        particles[i].setInMolecule(true);
        particles[i].setIndex(i);
    }

    // Initialize the simulation environment
    Environment env;

    // Initialize the Calculator
    physicsCalculator::MolCalculator calc(env, particles, desc, false);

    // Initialize the force to the expected values
    const std::vector<Vec<double>> expected_f = {
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 0.0 },
    };

    // Perform a single calculateF
    ASSERT_NO_THROW(calc.calculateF());

    // Make sure that there are no unwanted changes
    for (size_t i = 0; i < particles.size(); i++) {
        // Test if the new force is correct
        EXPECT_LT((calc.get_container()[i].getF() - expected_f[i]).len(), error_margin)
            << "The force was not correct. (expected: " << expected_f[i] << ", got: " << calc.get_container()[i].getF() << ")" << " (" << i << ")"
            << std::endl;
    }
}

// Test if the forces work if the membrane is spaced out
TEST(MolCalculator, calculateMoleculesSpaced) {
        //  Set the margin for the maximum floatingpoint error
    const double error_margin = 1E-9;

    ParticleGenerator gen;
    std::vector<TypeDesc> desc = {
        TypeDesc(1.0, 0.1, 1.0, 1.0, 0.0, 1.0, 1.0),
    };

    std::vector<Particle> particles = {
        Particle({ 50.0, 50.0, 50.0 }, {}),
        Particle({ 50.0, 51.1, 50.0 }, {}),
        Particle({ 51.1, 50.0, 50.0 }, {}),
        Particle({ 51.1, 51.1, 50.0 }, {}),
    };

    particles[0].setNeighbors({ 1, 2, SIZE_MAX, SIZE_MAX, 3, SIZE_MAX, SIZE_MAX, SIZE_MAX });
    particles[1].setNeighbors({ 0, 3, SIZE_MAX, SIZE_MAX, 2, SIZE_MAX, SIZE_MAX, SIZE_MAX });
    particles[2].setNeighbors({ 0, 3, SIZE_MAX, SIZE_MAX, 1, SIZE_MAX, SIZE_MAX, SIZE_MAX });
    particles[3].setNeighbors({ 1, 2, SIZE_MAX, SIZE_MAX, 0, SIZE_MAX, SIZE_MAX, SIZE_MAX });

    for (size_t i = 0; i < particles.size(); i++) {
        particles[i].setInMolecule(true);
        particles[i].setIndex(i);
    }

    // Initialize the simulation environment
    Environment env;

    // Initialize the Calculator
    physicsCalculator::MolCalculator calc(env, particles, desc, false);

    // Initialize the force to the expected values
    const std::vector<Vec<double>> expected_f = {
        { 0.2, 0.2, 0.0 },
        { 0.2, -0.2, 0.0 },
        { -0.2, 0.2, 0.0 },
        { -0.2, -0.2, 0.0 },
    };

    // Perform a single calculateF
    ASSERT_NO_THROW(calc.calculateF());

    // Make sure that there are no unwanted changes
    for (size_t i = 0; i < particles.size(); i++) {
        // Test if the new force is correct
        EXPECT_LT((calc.get_container()[i].getF() - expected_f[i]).len(), error_margin)
            << "The force was not correct. (expected: " << expected_f[i] << ", got: " << calc.get_container()[i].getF() << ")" << " (" << i << ")"
            << std::endl;
    }
}