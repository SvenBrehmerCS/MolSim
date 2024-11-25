#include "XMLTreeReader.h"

#include "Environment.h"
#include "ParticleContainer.h"
#include "ParticleGenerator.h"
#include "input.hxx"
#include "spdlog/spdlog.h"
#include <bits/fs_fwd.h>
#include <bits/fs_path.h>
#include <fstream>
#include <iostream>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>

using namespace xml_schema;

namespace inputReader {
    XMLTreeReader::XMLTreeReader() = default;
    XMLTreeReader::~XMLTreeReader() = default;


    void XMLTreeReader::readFile(ParticleContainer& container, const char* filename, const char* xsd_schema, Environment& environment) {
        std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
        if (xsd_schema == nullptr || strlen(xsd_schema) == 0) {
            spdlog::error("Invalid XSD schema provided.");
            std::exit(EXIT_FAILURE);
        }

        spdlog::info("The XML-File {} is valid, starting to read now", filename);

        std::ifstream XMLFile(filename);

        if (!XMLFile.is_open()) {
            spdlog::error("Could not open file {}", filename);
            std::exit(EXIT_FAILURE);
        }


        //function checks if XML is valid, is defined below
        if (!isValidXML(filename, xsd_schema)) {
            spdlog::error("Invalid XML file {}", filename);
            std::exit(EXIT_FAILURE);
        }

        CustomErrorHandler errHandler;

        // read in the simulation data
        std::unique_ptr<sim_t> sim = simulation(filename);

        if (!sim) {
            spdlog::critical("Could not open file {}", filename);
            std::exit(EXIT_FAILURE);
        }

        if (sim->output().name().empty() || sim->output().frequency() <= 0) {
            spdlog::critical("Invalid output data in simulation file {}", filename);
            std::exit(EXIT_FAILURE);
        }

        if (sim->param().t_end() <= 0 || sim->param().delta_t() <= 0) {
            spdlog::critical("Invalid simulation parameters in file {}", filename);
            std::exit(EXIT_FAILURE);
        }

        const char* output_file_name = sim->output().name().c_str();
        environment.set_output_file_name(output_file_name);

        int write_frequency = sim->output().frequency();
        environment.set_print_step(write_frequency);

        const double t_end = sim->param().t_end();
        environment.set_t_end(t_end);

        const double delta_t = sim->param().delta_t();
        environment.set_delta_t(delta_t);


        size_t t = sim->particle().size();
        if (t > INT_MAX) {
            spdlog::error("Particle size too large");
            std::exit(EXIT_FAILURE);
        }
        int num_particles = static_cast<int>(sim->particle().size());

        std::array<double, 3> x = { 0.0, 0.0, 0.0 };
        std::array<double, 3> v = { 0.0, 0.0, 0.0 };
        std::array<int, 3> N = { 0, 0, 0 };
        double m;
        double h;

        // TODO !! num_dimensions in dem xml file als element unter cuboid hinzufügen, momentan hard gecoded als 3
        int num_dimensions = 3;
        double brownian_motion = 0.1;

        container.resize(num_particles);

        const auto& particles = sim->particle();

        // initialize all the particles from the XML file into the container
        int i = 0;
        for (const auto& particle : particles) {

            if (particle.m() <= 0) {
                spdlog::critical("Invalid particle mass: {}", particle.m());
                std::exit(EXIT_FAILURE);
            }
            if (std::isnan(particle.pos_x()) || std::isnan(particle.pos_y()) || std::isnan(particle.pos_z())) {
                spdlog::critical("Invalid particle position in simulation file {}", filename);
                std::exit(EXIT_FAILURE);
            }

            container[i].setX({ particle.pos_x(), particle.pos_y(), particle.pos_z() });
            container[i].setV({ particle.vel_x(), particle.vel_y(), particle.vel_z() });
            container[i].setM(particle.m());
            i++;
        }

        const auto& cuboids = sim->cuboid();
        ParticleGenerator generator;

        // initialize all the cuboids into the container
        for (const auto& cuboid : cuboids) {

            if (cuboid.n_x() <= 0 || cuboid.n_y() <= 0 || cuboid.n_z() <= 0) {
                spdlog::critical("Invalid cuboid dimensions in simulation file {}", filename);
                std::exit(EXIT_FAILURE);
            }
            if (cuboid.m() <= 0) {
                spdlog::critical("Invalid cuboid mass in simulation file {}", filename);
                std::exit(EXIT_FAILURE);
            }

            x = { cuboid.pos_x(), cuboid.pos_y(), cuboid.pos_z() };
            v = { cuboid.vel_x(), cuboid.vel_y(), cuboid.vel_z() };
            N = { cuboid.n_x(), cuboid.n_y(), cuboid.n_z() };
            m = cuboid.m();
            h = cuboid.h();
            brownian_motion = cuboid.b_motion();

            container.resize(num_particles + cuboid.n_x() * cuboid.n_y() * cuboid.n_z());

            generator.generateCuboid(container, num_particles, x, v, m, N, h, brownian_motion, num_dimensions);

            num_particles += cuboid.n_x() * cuboid.n_y() * cuboid.n_z();
        }
    }

    bool XMLTreeReader::isValidXML(const char* filename, const char* xsd_schema) {
        try {
            // initializes xerces-c, RAII like
            XMLInitializer xmlInitializer;

            // create a Parser
            auto parser = std::make_shared<XercesDOMParser>();

            parser->setValidationScheme(XercesDOMParser::Val_Always);
            parser->setDoNamespaces(true);
            parser->setDoSchema(true);
            parser->setValidationConstraintFatal(true);
            parser->setValidationSchemaFullChecking(true);

            auto error_handler = std::make_shared<CustomErrorHandler>();
            parser->setErrorHandler(error_handler.get());
            //parser->setExitOnFirstFatalError(true);

            /*
            Grammar* grammar = parser->loadGrammar(xsd_schema, Grammar::SchemaGrammarType, true);
            if (grammar == nullptr) {
                std::cerr << "Fehler: loadGrammar hat kein Schema geladen (nullptr)." << std::endl;
            } else {
                std::cout << "Grammatik erfolgreich geladen: " << grammar << std::endl;
            }
            */
            //LocalFileInputSource source(XMLString::transcode(xsd_schema));
            /*
            Grammar* grammar = parser->loadGrammar(xsd_schema, Grammar::SchemaGrammarType, true);

            if (grammar == nullptr) {
                std::cerr << "Fehler: loadGrammar hat kein Schema geladen (nullptr)." << std::endl;
                return false;
            } else {
                std::cout << "Grammatik erfolgreich geladen: " << grammar << std::endl;
            }
            */

            //parsing, should throw if not valid
            parser->parse(filename);

            if (parser->getErrorCount() > 0) {
                spdlog::error("Error parsing file {}", filename);
                return false;
            }

        } catch (const XMLException& e) {
            char* message = XMLString::transcode(e.getMessage());
            spdlog::error("XMLException: {}", message);
            XMLString::release(&message);
            return false;

        } catch (const SAXParseException& e) {
            char* message = XMLString::transcode(e.getMessage());
            spdlog::error("SAXParseException: {}", message);
            XMLString::release(&message);
            return false;
        } catch (...) {
            spdlog::error("Unknown exception occured");
            return false;
        }
        return true;
    }


}


