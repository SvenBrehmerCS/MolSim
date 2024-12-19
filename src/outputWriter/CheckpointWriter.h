/**
 * @file
 *
 * @brief Handles writing checkpoints.
 *
 * @author frederik
 * @date 12/6/24
 */
#ifndef CHECKPOINTWRITER_H
#define CHECKPOINTWRITER_H
#include "Environment.h"
#include "Writer.h"

#pragma once

namespace outputWriter {

    /**
     * @class CheckpointWriter
     *
     * @brief This class implements the functionality to generate a checkpoint file.
     */
    class CheckpointWriter : public Writer {
    public:
        CheckpointWriter() = default;

        virtual ~CheckpointWriter() = default;

        /**
         * Plots the state of all particles of the current simulation.
         *
         * @param container Container of particle to save.
         * @param env Container holding necessary variables for saveing the types.
         * @param filename Name of the File the simulation will be written to.
         */
        void plot(const ParticleContainer& container, const Environment& env, const char* filename);

        /**
         * Handles the creation and writing of the checkpoint file.
         *
         * @param container List of particles to be plotted.
         * @param filename The base name of the file to be written.
         * @param iteration The number of the current iteration, which is used to generate an unique filename
         */
        virtual void plotParticles(const ParticleContainer& container, const std::string& filename, const int iteration);
    };
}
#endif // CHECKPOINTWRITER_H
