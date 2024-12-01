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
         * Imports the simulation arguments from the given input file.
         *
         * @param environment data structure for holding the simulation parameters.
         * @param particles data structure for holding the particles.
         * @param filename file path to input file.
         */
        virtual void readFile(const char* file, Environment& environment, ParticleContainer& container) = 0;
    };
} // namespace inputReader