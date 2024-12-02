/**
 * @file
 *
 * @brief Defines a default interface for the input readers
 */

#pragma once

class Environment;
class ParticleContainer;

/**
 * @brief Collection of readers for different input types
 */
namespace inputReader {

    /**
     * @class Reader
     *
     * @brief The default class for an instance of a reader
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
         * @param environment data structure for holding the simulation parameters.
         */
        virtual void readArguments(Environment& environment) = 0;

        /**
         * Imports the particles from the input file.
         *
         * @param particles data structure for holding the particles.
         */
        virtual void readParticle(ParticleContainer& container) = 0;
    };
} // namespace inputReader