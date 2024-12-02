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
#include "spdlog/spdlog.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

/**
 * @brief Collection of readers for different input types
 */
namespace inputReader {

    /**
     * @class FileReader
     *
     * @brief A Reader able to read standard input files and parse the arguments into the Environment
     */
    class FileReader : public Reader {

    private:
        /**
         * Stream used to parse the input file.
         */
        std::ifstream input_file;

    public:
        FileReader(const char* filename);

        virtual ~FileReader();

        /**
         * Imports the simulation arguments from the input file.
         *
         * @param environment Data structure for holding the simulation parameters.
         */
        virtual void readArguments(Environment& environment);

        /**
         * Imports the particles from the input file.
         *
         * @param particles Data structure for holding the particles.
         */
        virtual void readParticle(ParticleContainer& container);
    };
} // namespace inputReader