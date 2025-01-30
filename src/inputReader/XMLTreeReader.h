/**
 * @file
 *
 * @brief Handles the reading of a XML input file.
 */

#pragma once

#include "Reader.h"

#include "Environment.h"
#include "ParticleGenerator.h"
#include "Thermostat.h"
#include "container/ParticleContainer.h"
#include "input.h"
#include "physicsCalculator/Tweezers.h"
#include "spdlog/spdlog.h"

#include <fstream>
#include <iostream>
#include <string>

/**
 * @brief Collection of readers for different input types.
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
        /**
         * Define the xml file reader constructor.
         *
         * @param filename The name of the output file.
         */
        XMLTreeReader(const char* filename);

        virtual ~XMLTreeReader();

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
         * @param tweezers Data structure representing tweezers.
         * @param delta_t Time between steps for type initialization.
         * @param gravity Constant force on particles for type initialization.
         */
        virtual void readParticle(ParticleContainer& container, physicsCalculator::Tweezers& tweezers, const double delta_t, const double gravity);

        /**
         * Calculates the number of particles generated when creating a disc of particles.
         *
         * @param h Distance between particles inside the disc.
         * @param r Radius of the disc.
         *
         * @return The number of added particles.
         */
        int num_particles_added(double h, double r);
    };
} // namespace inputReader
