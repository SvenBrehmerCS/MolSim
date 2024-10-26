/**
 * @file
 * @brief Defines a default interface for the input readers
 */

#pragma once

#include <vector>

class Particle;

namespace inputReader {

    class Reader {

    public:
        /**
         * Imports a particle system from the given input file
         *
         * @param particles data structure for holding the particles.
         * @param filename file path to input file.
         */
        virtual void readFile(std::vector<Particle>& particles, const char* filename) = 0;
    };
} // namespace inputReader