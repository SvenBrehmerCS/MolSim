//
// Created by frederik on 12/6/24.
//
#include "outputWriter/CheckpointWriter.h"
#include "container/DSContainer.h"
#include "inputReader/CheckpointReader.h"

#include <gtest/gtest.h>
TEST(CheckpointWriterTest, CheckpointCombined) {
    Environment env;
    DSContainer container;
    outputWriter::CheckpointWriter writer;
    inputReader::CheckpointReader reader;

    const double mass = 1.0;
    const double sigma = 1.0;
    const double epsilon = 1.0;
    const double delta_t = 15;
    const double gravity = -12.44;

    env.set_delta_t(delta_t);
    env.set_gravity(gravity);

    auto desc = TypeDesc(mass, sigma, epsilon, delta_t, gravity);
    container.set_particle_type({ desc });

    container.resize(4);
    for (int i = 0; i < 4; i++) {
        double j = i;
        container[i].setX({ j, j, j });
        container[i].setType(0);
        container[i].setV({ j, j, j });
        container[i].setF({ j, j, j });
    }

    writer.plot(container, env, "CheckpointCombined");

    reader.readSimulation(container, "CheckpointCombined");

    ASSERT_EQ(container.size(), 4);
    ASSERT_EQ(container.get_types().size(), 1);
    EXPECT_EQ(container.get_types()[0].get_mass(), mass);
    EXPECT_EQ(container.get_types()[0].get_sigma(), sigma);
    EXPECT_EQ(container.get_types()[0].get_epsilon(), epsilon);
    EXPECT_EQ(container.get_types()[0].get_dt_m(), (delta_t * 0.5) / mass);
    EXPECT_EQ(container.get_types()[0].get_G()[1], gravity * mass);

    EXPECT_EQ(container[0].getX()[0], 0);
    EXPECT_EQ(container[0].getX()[1], 0);
    EXPECT_EQ(container[0].getX()[2], 0);
    EXPECT_EQ(container[0].getType(), 0);
    EXPECT_EQ(container[0].getV()[0], 0);
    EXPECT_EQ(container[0].getV()[1], 0);
    EXPECT_EQ(container[0].getV()[2], 0);
    EXPECT_EQ(container[0].getF()[0], 0);
    EXPECT_EQ(container[0].getF()[1], 0);
    EXPECT_EQ(container[0].getF()[2], 0);

    EXPECT_EQ(container[1].getX()[0], 1);
    EXPECT_EQ(container[1].getX()[1], 1);
    EXPECT_EQ(container[1].getX()[2], 1);
    EXPECT_EQ(container[1].getType(), 0);
    EXPECT_EQ(container[1].getV()[0], 1);
    EXPECT_EQ(container[1].getV()[1], 1);
    EXPECT_EQ(container[1].getV()[2], 1);
    EXPECT_EQ(container[1].getF()[0], 1);
    EXPECT_EQ(container[1].getF()[1], 1);
    EXPECT_EQ(container[1].getF()[2], 1);

    EXPECT_EQ(container[2].getX()[0], 2);
    EXPECT_EQ(container[2].getX()[1], 2);
    EXPECT_EQ(container[2].getX()[2], 2);
    EXPECT_EQ(container[2].getType(), 0);
    EXPECT_EQ(container[2].getV()[0], 2);
    EXPECT_EQ(container[2].getV()[1], 2);
    EXPECT_EQ(container[2].getV()[2], 2);
    EXPECT_EQ(container[2].getF()[0], 2);
    EXPECT_EQ(container[2].getF()[1], 2);
    EXPECT_EQ(container[2].getF()[2], 2);

    EXPECT_EQ(container[3].getX()[0], 3);
    EXPECT_EQ(container[3].getX()[1], 3);
    EXPECT_EQ(container[3].getX()[2], 3);
    EXPECT_EQ(container[3].getType(), 0);
    EXPECT_EQ(container[3].getV()[0], 3);
    EXPECT_EQ(container[3].getV()[1], 3);
    EXPECT_EQ(container[3].getV()[2], 3);
    EXPECT_EQ(container[3].getF()[0], 3);
    EXPECT_EQ(container[3].getF()[1], 3);
    EXPECT_EQ(container[3].getF()[2], 3);
}