/**
 * @file
 *
 * @brief Defines a default interface for the output writers.
 */

#pragma once

#include "container/ParticleContainer.h"
#include <string>

/**
 * @brief Collection of writers for different output types.
 */
namespace outputWriter {

    /**
     * @class Writer
     *
     * @brief The default class for an instance of a writer.
     */
    class Writer {

    public:
        /**
         * Provide a default destructor for every writer.
         */
        virtual ~Writer() {};

        /**
         * Creates an output file based on the state of the given particles.
         *
         * @param container List of particles to be plotted.
         * @param filename The base name of the file to be written.
         * @param iteration The number of the current iteration, which is used to generate an unique filename.
         */
        virtual void plotParticles(const ParticleContainer& container, const std::string& filename, const int iteration) = 0;
    };
} // namespace outputWriter
