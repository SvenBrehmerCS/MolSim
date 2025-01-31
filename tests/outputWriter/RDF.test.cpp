#include <gtest/gtest.h>
#include <outputWriter/RDF.h>
#include <physicsCalculator/LJCalculator.h>
#include <utils/ArrayUtils.h>

// Test if the RDF gets correctly generated if the domain does not contain any particles.
TEST(RDF, NoParticles) {
    // Initialize the list of particles
    std::vector<Particle> particles = {};

    // Initialise the list of type descriptors
    std::vector<TypeDesc> ptypes = {
        TypeDesc { 3.0, 1.0, 5.0, 0.1, 0.0 },
    };

    Environment env;

    // Initialize the Calculator
    physicsCalculator::LJCalculator calc(env, particles, ptypes, false);

    // Initialize the RDF
    outputWriter::RDF rdf(2.0, 100, std::array<BoundaryType, 6> { PERIODIC, HARD, HARD, PERIODIC, HARD, HARD }, Vec<double> { 200.0, 200.0, 200.0 });

    // Generate the RDF
    std::vector<size_t> rdf_data = rdf.generateRDF(calc.get_container());

    EXPECT_EQ(rdf_data, std::vector<size_t>(100)) << "RDF data should be empty.";
}

// Test if the RDF gets correctly generated if the domain contains some particles and only reflective boundaries.
TEST(RDF, NoPeriodic) {
    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 10.0, 100.0, 100.0 }, {}),
        Particle({ 20.0, 100.0, 100.0 }, {}),
        Particle({ 30.0, 100.0, 100.0 }, {}),
        Particle({ 40.0, 100.0, 100.0 }, {}),
        Particle({ 50.0, 100.0, 100.0 }, {}),
        Particle({ 60.0, 100.0, 100.0 }, {}),
        Particle({ 70.0, 100.0, 100.0 }, {}),
        Particle({ 80.0, 100.0, 100.0 }, {}),
        Particle({ 90.0, 100.0, 100.0 }, {}),
        Particle({ 100.0, 100.0, 100.0 }, {}),
        Particle({ 110.0, 100.0, 100.0 }, {}),
        Particle({ 120.0, 100.0, 100.0 }, {}),
        Particle({ 130.0, 100.0, 100.0 }, {}),
        Particle({ 140.0, 100.0, 100.0 }, {}),
        Particle({ 150.0, 100.0, 100.0 }, {}),
        Particle({ 160.0, 100.0, 100.0 }, {}),
        Particle({ 170.0, 100.0, 100.0 }, {}),
        Particle({ 180.0, 100.0, 100.0 }, {}),
        Particle({ 190.0, 100.0, 100.0 }, {}),
    };

    // Initialise the list of type descriptors
    std::vector<TypeDesc> ptypes = {
        TypeDesc { 3.0, 1.0, 5.0, 0.1, 0.0 },
    };

    Environment env;

    // Initialize the Calculator
    physicsCalculator::LJCalculator calc(env, particles, ptypes, false);

    // Initialize the RDF
    outputWriter::RDF rdf(20.0, 10, std::array<BoundaryType, 6> { HARD, HARD, HARD, HARD, HARD, HARD }, Vec<double> { 200.0, 200.0, 200.0 });

    // Generate the RDF
    std::vector<size_t> rdf_data = rdf.generateRDF(calc.get_container());

    EXPECT_EQ(rdf_data, std::vector<size_t>({ 36, 66, 58, 50, 42, 34, 26, 18, 10, 2 }))
        << "RDF data was incorrect: " << ArrayUtils::to_string(rdf_data);
}

// Test if the RDF gets correctly generated if the domain contains some particles and only periodic boundaries.
TEST(RDF, Periodic) {
    // Initialize the list of particles
    std::vector<Particle> particles = {
        Particle({ 10.0, 100.0, 100.0 }, {}),
        Particle({ 20.0, 100.0, 100.0 }, {}),
        Particle({ 30.0, 100.0, 100.0 }, {}),
        Particle({ 40.0, 100.0, 100.0 }, {}),
        Particle({ 50.0, 100.0, 100.0 }, {}),
        Particle({ 60.0, 100.0, 100.0 }, {}),
        Particle({ 70.0, 100.0, 100.0 }, {}),
        Particle({ 80.0, 100.0, 100.0 }, {}),
        Particle({ 90.0, 100.0, 100.0 }, {}),
        Particle({ 100.0, 100.0, 100.0 }, {}),
        Particle({ 110.0, 100.0, 100.0 }, {}),
        Particle({ 120.0, 100.0, 100.0 }, {}),
        Particle({ 130.0, 100.0, 100.0 }, {}),
        Particle({ 140.0, 100.0, 100.0 }, {}),
        Particle({ 150.0, 100.0, 100.0 }, {}),
        Particle({ 160.0, 100.0, 100.0 }, {}),
        Particle({ 170.0, 100.0, 100.0 }, {}),
        Particle({ 180.0, 100.0, 100.0 }, {}),
        Particle({ 190.0, 100.0, 100.0 }, {}),
    };

    // Initialise the list of type descriptors
    std::vector<TypeDesc> ptypes = {
        TypeDesc { 3.0, 1.0, 5.0, 0.1, 0.0 },
    };

    Environment env;

    // Initialize the Calculator
    physicsCalculator::LJCalculator calc(env, particles, ptypes, false);

    // Initialize the RDF
    outputWriter::RDF rdf(
        20.0, 10, std::array<BoundaryType, 6> { PERIODIC, PERIODIC, PERIODIC, PERIODIC, PERIODIC, PERIODIC }, Vec<double> { 200.0, 200.0, 200.0 });

    // Generate the RDF
    std::vector<size_t> rdf_data = rdf.generateRDF(calc.get_container());

    EXPECT_EQ(rdf_data, std::vector<size_t>({ 36, 72, 72, 72, 72, 72, 72, 72, 72, 72 }))
        << "RDF data was incorrect: " << ArrayUtils::to_string(rdf_data);
}
