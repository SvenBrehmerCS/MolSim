/**
 * @file
 *
 * @brief Handles the reading of a XML input file
 */

#pragma once

#include "Reader.h"

#include "Environment.h"
#include "ParticleGenerator.h"
#include "boundaries/ParticleContainer.h"
#include "input.h"
#include "spdlog/spdlog.h"

#include <fstream>
#include <iostream>
#include <string>


/**
 * @file
 *
 * @brief Handles the reading of a xml file
 */
namespace inputReader {


    /**
     * @class XMLTreeReader
     *
     * @brief A Reader to read a xml file, and parse it into Objects.
     */
    class XMLTreeReader : public Reader {

    public:
        XMLTreeReader();

        virtual ~XMLTreeReader();

        /**
         * Imports the simulation arguments from the given input file.
         *
         * @param environment data structure for holding the simulation parameters.
         * @param filename file path to input file.
         */
        virtual void readFile(const char* filename, Environment& environment, ParticleContainer& container);

        int num_particles_added(double h, double r);
    };
}
