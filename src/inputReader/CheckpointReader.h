//
// Created by frederik on 12/6/24.
// TODO Documentation

#ifndef CHECKPOINTREADER_H
#define CHECKPOINTREADER_H
#include "Environment.h"
#include "Reader.h"
#include "container/ParticleContainer.h"

namespace inputReader {
    /**
     * This class reads in the Simulation
     */
    class CheckpointReader {
    public:
        CheckpointReader() = default;

        ~CheckpointReader() = default;

        /**
         *
         * @param container
         * @param environment
         * @param filename
         */
        void readSimulation(ParticleContainer& container, const char* filename);
    };
}


#endif // CHECKPOINTREADER_H
