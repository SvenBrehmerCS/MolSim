#include <boundaries/BoxContainer.h>
#include <boundaries/HardStepper.h>
#include <boundaries/InfContainer.h>
#include <gtest/gtest.h>
#include <physicsCalculator/GravityCalculator.h>
#include <physicsCalculator/LJCalculator.h>

// Test if the reflective boundary works with an inf container and a single moving particle
TEST(HardStepper, InfContainer) {
    HardStepper stepper;

    std::vector<Particle> particles = {
        Particle({ 2.0, 1.0, 1.0 }, { 2.0, 1.0, 0.0 }, 1.0, 0.0),
    };

    std::shared_ptr<InfContainer> container { new InfContainer(particles, { 20.0, 20.0, 5.0 }) };

    const char* argv[] = {
        "./MolSim",
        "path/to/input.txt",
        "-delta_t=1.0",
    };

    constexpr int argc = sizeof(argv) / sizeof(argv[0]);

    Environment env;

    ASSERT_NO_THROW(env = Environment(argc, argv));

    physicsCalculator::GravityCalculator calc(env, container);


}

// Test if the reflective boundary works with a box container and a single moving particle

// Test if the reflective boundary works with a box container and a single moving particle, while the domain size is different form the simulation
// size
