#include <gtest/gtest.h>

// In general there are no tests for the getter, setter, destructors and default constructors, as well as other very simple functions
// Most of these functions are still used within the tests, therefore they are implicitly tested.

// There are no tests for physicsCalculator/Calculator.h as this file only contains an interface
#include "physicsCalculator/GravityCalculator.test.h"
// TODO: #include "physicsCalculator/LJCalculator.test.h"

#include "Environment.test.h"
// There are no tests for MolSim.h as this file only contains the main function
#include "Particle.test.h"
#include "ParticleContainer.test.h"
#include "ParticleGenerator.test.h"
