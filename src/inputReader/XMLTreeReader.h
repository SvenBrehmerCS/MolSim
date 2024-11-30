#pragma once

#include "Reader.h"
#include "spdlog/spdlog.h"

#include <Environment.h>
#include <iostream>
#include <string>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/PlatformUtils.hpp>


/**
 * @file
 *
 * @brief Handles the reading of a xml file
 */
using namespace xercesc;
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
         * @param environment will set the environment variables read in from @filename to this environment
         *
         */
        void readFile(ParticleContainer& container, const char* filename, Environment& environment);

        /**
         * This method has no use in this class, but needs to be implemented
         * @param container
         * @param filename
         */
        void readFile(ParticleContainer& container, const char* filename) override {
            spdlog::info("This function is of no use in this class XMLTreeReader but needs to be overridden, use the other function and specify the "
                         "parameters accordingly");
        }
        int num_particles_added(double h, double r);
    };
}
