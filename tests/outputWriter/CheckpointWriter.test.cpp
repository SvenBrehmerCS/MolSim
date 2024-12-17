//
// Created by frederik on 12/6/24.
//
#include "outputWriter/CheckpointWriter.h"
#include "container/DSContainer.h"
#include <gtest/gtest.h>
TEST(CheckpointWriterTestCombined, CheckpointCombined) {
    Environment env;
    DSContainer container;
    outputWriter::CheckpointWriter writer;

    container.resize(4);
    Particle particle0;
    Particle particle1;
    Particle particle2;
    Particle particle3;

    particle0.setX({0.5,0.5,0.5});
    particle0.setV({1.1,1.1,1.1});
    writer.plot(container,env, "test");
    env.set_delta_t(10);
}