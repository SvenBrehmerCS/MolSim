#include "../src/Thermostat.h"
#include "../src/container/BoxContainer.h"
#include "../src/container/DSContainer.h"

#include <gtest/gtest.h>

TEST(Thermostat, Initialisation) {
    Thermostat thermo;
    std::shared_ptr<ParticleContainer> cont {};
    bool val;

    EXPECT_NO_THROW(val = thermo.get_active()) << "get_active() should not throw an exception.";
    EXPECT_FALSE(val) << "Activity status after creation should be false.";

    EXPECT_NO_THROW(thermo.set_active(true)) << "set_active() should not throw exceptions.";
    EXPECT_NO_THROW(thermo.set_dimensions(2)) << "set_dimensions() should not throw exceptions.";
    EXPECT_NO_THROW(thermo.set_max_change(80.212)) << "set_max_change() should not throw exceptions.";
    EXPECT_NO_THROW(thermo.set_particles(cont)) << "set_particles() should not throw exceptions.";
    EXPECT_NO_THROW(thermo.set_T_target(2.0006)) << "set_T_target() should not throw exceptions.";

    EXPECT_TRUE(thermo.get_active()) << "Activity status should have changed after set_active().";
}

TEST(Thermostat, EmptyContainerRegulation) {
    Thermostat thermo;
    // DSContainer
    std::shared_ptr<ParticleContainer> cont = std::make_shared<DSContainer>();

    thermo.set_dimensions(3);
    thermo.set_max_change(5);
    thermo.set_particles(cont);
    thermo.set_T_target(80);

    EXPECT_NO_THROW(thermo.regulate_Temperature()) << "regulate_Temperature() should not throw an exception.";

    EXPECT_EQ(cont->size(), 0) << "regulate_Temperature() should not change number of particles in the container.";
    EXPECT_FALSE(thermo.get_active()) << "regulate_Temperature() should not change the activity status.";

    // Boxcontainer
    cont = std::make_shared<BoxContainer>(3.0, Vec<double> { 15.0, 15.0, 15.0 });

    EXPECT_NO_THROW(thermo.regulate_Temperature()) << "regulate_Temperature() should not throw an exception.";

    EXPECT_EQ(cont->size(), 0) << "regulate_Temperature() should not change number of particles in the container.";
    EXPECT_FALSE(thermo.get_active()) << "regulate_Temperature() should not change the activity status.";
}

TEST(Thermostat, OneParticleRegulation) {
    Particle particle = { { 15, 15, 15 }, { 1, 1, 1 } };
    TypeDesc type = { 1, 1, 5, 0.1, -12.44 };
    std::shared_ptr<ParticleContainer> cont = std::make_shared<DSContainer>(std::vector<Particle> { particle }, std::vector<TypeDesc> { type });

    Thermostat thermo;
    thermo.set_dimensions(3);
    thermo.set_max_change(15);
    thermo.set_T_target(9);
    thermo.set_particles(cont);

    // Temperature has to be adjusted.
    EXPECT_NO_THROW(thermo.regulate_Temperature()) << "regulate_Temperature() should not throw an exception.";

    EXPECT_FALSE(thermo.get_active()) << "regulate_Temperature() should not change the activity status.";

    Particle expected = { { 15, 15, 15 }, { 3, 3, 3 } };

    EXPECT_EQ(cont->size(), 1) << "Amount of particles should not change when changing temperature.";
    EXPECT_EQ(*cont->begin(), expected) << "Particle should be updated correctly.";

    // Temperature does not have to be adjusted.
    EXPECT_NO_THROW(thermo.regulate_Temperature()) << "regulate_Temperature() should not throw an exception.";

    EXPECT_EQ(cont->size(), 1) << "Amount of particles should not change when not changing temperature.";
    EXPECT_EQ(*cont->begin(), expected) << "Particle should not be changed if temperature is correct.";
}

TEST(Thermostat, MultipleParticleRegulation) {
    const double error_margin = 1E-9;

    std::vector<Particle> particles = {
        { { 15, 15, 15 }, { 1, 1, 1 }, 0 },
        { { 5, 5, 5 }, { 0, 2, -1 }, 0 },
        { { 2, 36, 12 }, { -3, 0, 1 }, 1 },
    };
    std::vector<TypeDesc> types = {
        { 1, 1, 5, 0.1, -9.81 },
        { 2, 1, 5, 0.1, -9.81 },
    };
    std::shared_ptr<ParticleContainer> cont = std::make_shared<DSContainer>(particles, Vec<double> { 20, 20, 20 }, types);

    Thermostat thermo;
    thermo.set_active(true);
    thermo.set_dimensions(3);
    thermo.set_T_target(15);
    thermo.set_particles(cont);

    // Temperature has to be adjusted.
    EXPECT_NO_THROW(thermo.regulate_Temperature()) << "regulate_Temperature() should not throw an exception.";

    std::vector<Particle> expected = {
        { { 15, 15, 15 }, { 2.195775164, 2.195775164, 2.195775164 }, 0 },
        { { 5, 5, 5 }, { 0, 4.391550328, -2.195775164 }, 0 },
        { { 2, 36, 12 }, { -6.587325492, 0, 2.195775164 }, 1 },
    };

    EXPECT_TRUE(thermo.get_active()) << "regulate_Temperature() should not change the activity status.";
    EXPECT_EQ(cont->size(), 3) << "Amount of particles should not change when changing temperature.";
    auto actual = cont->begin();
    for (size_t i = 0; i < 3; i++) {
        EXPECT_EQ((*actual).getX(), expected[i].getX()) << "Position of particle should not be changed by temperature regulation.";
        EXPECT_LT(((*actual).getV() - expected[i].getV()).len(), error_margin)
            << "Velocity should be correctly scaled according to temperature.";
        EXPECT_EQ((*actual).getF(), expected[i].getF()) << "Force of particle should not be changed by temperature regulation.";
        EXPECT_EQ((*actual).getOldF(), expected[i].getOldF()) << "Old of particle force should not be changed by temperature regulation.";
        EXPECT_EQ((*actual).getType(), expected[i].getType()) << "Type of particle should not be changed by temperature regulation.";
        actual++;
    }

    // Temperature does not have to be adjusted.
    EXPECT_NO_THROW(thermo.regulate_Temperature()) << "regulate_Temperature() should not throw an exception.";

    EXPECT_TRUE(thermo.get_active()) << "regulate_Temperature() should not change the activity status.";
    EXPECT_EQ(cont->size(), 3) << "Amount of particles should not change when changing temperature.";
    actual = cont->begin();
    for (size_t i = 0; i < 3; i++) {
        EXPECT_EQ((*actual).getX(), expected[i].getX()) << "Position of particle should not be changed by temperature regulation.";
        EXPECT_LT(((*actual).getV() - expected[i].getV()).len(), error_margin)
            << "Velocity should be correctly scaled according to temperature.";
        EXPECT_EQ((*actual).getF(), expected[i].getF()) << "Force of particle should not be changed by temperature regulation.";
        EXPECT_EQ((*actual).getOldF(), expected[i].getOldF()) << "Old of particle force should not be changed by temperature regulation.";
        EXPECT_EQ((*actual).getType(), expected[i].getType()) << "Type of particle should not be changed by temperature regulation.";
        actual++;
    }
}