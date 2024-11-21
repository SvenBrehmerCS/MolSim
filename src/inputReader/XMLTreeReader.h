#pragma once

#include "Reader.h"

#include <string>
#include <Environment.h>


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
        using Reader::readFile;

        XMLTreeReader();
        ~XMLTreeReader();

        /**
         * Imports a particle system from the given xml input file
         *
         * @param container data structure for holding the particles.
         * @param filename file path to xml input file.
         * @param xsdfile specifies the rules for the xml file @filename
         * @param environment will set the environment variables read in from @filename to this environment
         *
         */
        void readFile(ParticleContainer& container, const char* filename, const char* xsdfile, Environment& environment);
    };
}
