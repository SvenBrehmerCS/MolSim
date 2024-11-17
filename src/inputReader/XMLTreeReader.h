#pragma once

#include "Reader.h"
#include "input.hxx"


/**
 * @file
 *
 * @brief Handles the reading of a xml file
 */
namespace inputReader {


    /**
     * @class XMLTreeReader
     *
     * @brief A Reader to read a xml file, and parse it into Objects.
     */
    class XMLTreeReader : public Reader {
    public:

        XMLTreeReader();
        ~XMLTreeReader();

        /**
         * Imports a particle system from the given xml input file
         *
         * @param particles data structure for holding the particles.
         * @param filename file path to xml input file.
         */

        virtual void readFile(ParticleContainer& container, const char* filename);
    };
}
