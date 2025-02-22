/**
 * @file
 *
 * @brief Handles the reading of a standard input file.
 *
 * @author eckhardw
 */

#pragma once

#include "Reader.h"

#include "Environment.h"
#include "ParticleGenerator.h"
#include "Thermostat.h"
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
     * @brief A Reader able to read standard input files and parse the arguments into the Environment.
     */
    class FileReader : public Reader {

    private:
        /**
         * Stream used to parse the input file.
         */
        std::ifstream input_file;

    public:
        /**
         * Define the file reader constructor.
         *
         * @param filename The name of the output file.
         */
        FileReader(const char* filename);

        virtual ~FileReader();

        /**
         * Imports the simulation arguments from the input file.
         *
         * @param environment Data structure for holding the simulation parameters.
         * @param thermostat Data structure representing the thermostat.
         */
        virtual void readArguments(Environment& environment, Thermostat& thermostat);

        /**
         * Imports the particles from the input file.
         *
         * @param container Data structure for holding the particles.
         * @param delta_t Time between steps for type initialization.
         * @param gravity Constant force on particles for type initialization.
         */
        virtual void readParticle(ParticleContainer& container, const double delta_t, const double gravity);
    };
} // namespace inputReader