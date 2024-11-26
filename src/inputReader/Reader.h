/**
 * @file
 *
 * @brief Defines a default interface for the input readers
 */

#pragma once

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
         * Imports a particle system from the given input file
         *
         * @param particles data structure for holding the particles.
         * @param filename file path to input file.
         */
        virtual void readFile(ParticleContainer& container, const char* filename) = 0;
    };
} // namespace inputReader