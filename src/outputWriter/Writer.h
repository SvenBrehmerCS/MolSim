/**
 * @file
 *
 * @brief Defines a default interface for the output writers
 */

#pragma once

#include <string>
#include <vector>

class Particle;

/**
 * @brief Collection of writers for different output types
 */
namespace outputWriter {

    /**
     * @class Writer
     *
     * @brief The default class for an instance of a writer
     */
    class Writer {

    public:

        /**
         * Provide a default destructor for every writer.
         */
        virtual ~Reader() = 0;

        /**
         * @brief Creates an output file based on the state of the given particles
         *
         * @param particles List of particles to be plotted.
         * @param filename The base name of the file to be written.
         * @param iteration The number of the current iteration,
         *        which is used to generate an unique filename
         */
        virtual void plotParticles(const std::vector<Particle>& particles, const std::string& filename, int iteration) = 0;
    };
} // namespace outputWriter