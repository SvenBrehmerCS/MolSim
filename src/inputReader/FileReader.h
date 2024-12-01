/**
 * @file
 *
 * @brief Handles the reading of a standard input file
 *
 * @author eckhardw
 */

#pragma once

#include "Reader.h"

#include "Environment.h"
#include "ParticleGenerator.h"
#include "boundaries/InfContainer.h"
#include "spdlog/spdlog.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

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
         * Imports the simulation arguments from the given input file
         *
         * @param environment data structure for holding the simulation parameters.
         * @param particles data structure for holding the particles.
         * @param filename file path to input file.
         */
        virtual void readFile(const char* filename, Environment& environment, ParticleContainer& container);
    };
} // namespace inputReader