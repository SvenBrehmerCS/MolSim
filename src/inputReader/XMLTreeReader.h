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
 * @brief Collection of readers for different input types
 */
namespace inputReader {

    /**
     * @class XMLTreeReader
     *
     * @brief A Reader to read a xml file, and parse it into Objects.
     */
    class XMLTreeReader : public Reader {

    private:
        /**
         * Stores a tree structure representing the XML file parsed from the input.
         */
        std::unique_ptr<sim_t> sim;


    public:
        XMLTreeReader(const char* filename);

        virtual ~XMLTreeReader();

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

        /**
         * Calculates the number of particles generated when creating a disc of particles.
         *
         * @param h Distance between particles inside the disc.
         * @param r Radius of the disc.
         */
        int num_particles_added(double h, double r);
    };
} // namespace inputReader
