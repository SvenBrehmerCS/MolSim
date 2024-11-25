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
         * @param xsdfile specifies the rules for the xml file @filename
         * @param environment will set the environment variables read in from @filename to this environment
         *
         */
        void readFile(ParticleContainer& container, const char* filename) override { }
        void readFile(ParticleContainer& container, const char* filename, const char* xsdfile, Environment& environment);
        bool isValidXML(const char* filename, const char* xsdfile);

        struct XMLInitializer {
            XMLInitializer() { XMLPlatformUtils::Initialize(); }
            ~XMLInitializer() { XMLPlatformUtils::Terminate(); }
        };

        class CustomErrorHandler : public HandlerBase {
        public:
            void fatalError(const SAXParseException& e) override {
                char* message = XMLString::transcode(e.getMessage());
                spdlog::error("fatal error: {}", message);
                XMLString::release(&message);
            }

            void error(const SAXParseException& e) override {
                char* message = XMLString::transcode(e.getMessage());
                spdlog::error("error: {}", message);
                XMLString::release(&message);
            }

            void warning(const SAXParseException& e) override {
                char* message = XMLString::transcode(e.getMessage());
                spdlog::warn("warning: {}", message);
                XMLString::release(&message);
            }
        };
    };
}
