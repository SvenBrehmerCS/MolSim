//
// Created by frederik on 12/6/24.
//
#include "outputWriter/CheckpointWriter.h"
#include "boundaries/InfContainer.h"
#include <gtest/gtest.h>
TEST(CheckpointWriterTest, writeCheckpoint) {
    Environment env;
    InfContainer container;
    outputWriter::CheckpointWriter writer;

    env.set_delta_t(0.01);
    env.set_t_end(15);

    writer.plot(container, env, "test", 0);
    env.set_delta_t(10);
}