//
// Created by frederik on 12/6/24.
//

#ifndef CHECKPOINTWRITER_H
#define CHECKPOINTWRITER_H
#include "Environment.h"
#include "Writer.h"


namespace outputWriter {


    class CheckpointWriter {
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
        void plot(ParticleContainer& container, Environment& env, const char* filename);
    };


}
#endif // CHECKPOINTWRITER_H
