/**
 * @file
 *
 * @brief Handles the reading of a checkpoint file.
 */

#ifndef CHECKPOINTREADER_H
#define CHECKPOINTREADER_H
#include "Environment.h"
#include "Reader.h"
#include "container/ParticleContainer.h"

/**
 * @brief Collection of readers for different input types.
 */
namespace inputReader {

    /**
     * @class CheckpointReader
     *
     * @brief A reader able to load a checkpoint into the simulation.
     */
    class CheckpointReader {
    public:
        CheckpointReader() = default;

        ~CheckpointReader() = default;

        /**
         * Loads the particles from a checkpoint into the simulation.
         *
         * @param container
         * @param filename
         */
        void readSimulation(ParticleContainer& container, const char* filename);
    };
} // namespace inputReader
#endif // CHECKPOINTREADER_H
