/**
 * @file
 * @brief Defines a default interface for the output writers
 */

#pragma once

#include <string>
#include <vector>

class Particle;

namespace outputWriter {

    class Writer {

    public:
        /**
         * creates an output file based on the state of the given particles
         *
         * @param particles list of particles to be plotted.
         * @param filename the base name of the file to be written.
         * @param iteration the number of the current iteration,
         *        which is used to generate an unique filename
         */
        virtual void plotParticles(const std::vector<Particle>& particles, const std::string& filename, int iteration) = 0;
    };
} // namespace outputWriter