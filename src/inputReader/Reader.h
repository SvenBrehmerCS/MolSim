/*
 * Reader.h
 *
 *  Created on: 25.10.2024
 *      Author: Sven Brehmer
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