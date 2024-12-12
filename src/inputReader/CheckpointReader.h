//
// Created by frederik on 12/6/24.
//

#ifndef CHECKPOINTREADER_H
#define CHECKPOINTREADER_H
#include "Environment.h"
#include "boundaries/ParticleContainer.h"

namespace inputReader {
    /**
     * This class reads in the Simulation
     */
    class CheckpointReader {
    public:
        CheckpointReader() = default;

        ~CheckpointReader()= default;

        BoundaryType getBoundaryType(const std::string& boundary) {
            if (boundary == "INF_CONT") {
                return BoundaryType::INF_CONT;
            }
            if (boundary == "HALO") {
                return BoundaryType::HALO;
            }
            if (boundary == "HARD") {
                return BoundaryType::HARD;
            }
            if (boundary == "PERIODIC") {
                return BoundaryType::PERIODIC;
            }
            else {
                return BoundaryType::OUTFLOW;
            }
        }

        /**
         *
         * @param container
         * @param environment
         * @param filename
         */
        void readSimulation(ParticleContainer& container, Environment& environment, const char* filename, int new_particles);
    };
}


#endif // CHECKPOINTREADER_H
