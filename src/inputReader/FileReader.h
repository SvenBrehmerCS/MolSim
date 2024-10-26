/*
 * FileReader.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include "Reader.h"

namespace inputReader {

    class FileReader : public Reader {

    public:
        FileReader();

        virtual ~FileReader();

        /**
         * Imports a particle system from the given input file
         *
         * @param particles data structure for holding the particles.
         * @param filename file path to input file.
         */
        virtual void readFile(std::vector<Particle>& particles, const char* filename);
    };

} // namespace inputReader