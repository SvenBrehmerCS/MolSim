/**
 * @file
 *
 * @brief Defines a default interface for the input readers.
 */

#pragma once

class Environment;
class ParticleContainer;
class Thermostat;

/**
 * @brief Collection of readers for different input types.
 */
namespace inputReader {

    /**
     * @class Reader
     *
     * @brief The default class for an instance of a reader.
     */
    class Reader {

    public:
        /**
         * Provide a default destructor for every reader.
         */
        virtual ~Reader() {};

        /**
         * Imports the simulation arguments from the input file.
         *
         * @param environment Data structure for holding the simulation parameters.
         * @param thermostat Data structure representing the thermostat.
         */
        virtual void readArguments(Environment& environment, Thermostat& thermostat) = 0;

        /**
         * Imports the particles from the input file.
         *
         * @param container Data structure for holding the particles.
         * @param delta_t Time between steps for type initialization.
         * @param gravity Constant force on particles for type initialization.
         */
        virtual void readParticle(ParticleContainer& container, const double delta_t, const double gravity) = 0;
    };
} // namespace inputReader