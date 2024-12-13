//
// Created by frederik on 12/6/24.
//

#ifndef CHECKPOINTWRITER_H
#define CHECKPOINTWRITER_H
#include "Environment.h"
#include "Writer.h"

#pragma once

namespace outputWriter {


    class CheckpointWriter : public Writer {
    public:
        CheckpointWriter() = default;
        virtual ~CheckpointWriter() = default;
        /**
         *
         * Function plots the entire state of the current simulation, all particles and environment variables
         *
         * @param container
         * @param env
         * @param filename the name of the File the simulation will be written to.
         */
        void plot(ParticleContainer& container, const char* filename);
        virtual void plotParticles(ParticleContainer& container, const std::string& filename, int iteration);
    };


}
#endif // CHECKPOINTWRITER_H
