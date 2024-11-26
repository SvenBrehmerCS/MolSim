/**
 * @file
 *
 * @brief Handles the reading of a standard input file
 *
 * @author eckhardw
 */

#pragma once

#include "Reader.h"

namespace inputReader {

    /**
     * @class FileReader
     *
     * @brief A Reader able to read standard input files and parse the arguments into the Environment
     */
    class FileReader : public Reader {

    public:
        FileReader();

        virtual ~FileReader();

        /**
         * Imports a particle system from the given input file
         *
         * @param particles data structure for holding the particles.
         * @param filename file path to input file.
         */
        virtual void readFile(ParticleContainer& container, const char* filename);
    };

} // namespace inputReader