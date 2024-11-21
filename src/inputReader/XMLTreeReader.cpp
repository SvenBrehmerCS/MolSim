#include "XMLTreeReader.h"

#include "Environment.h"
#include "ParticleContainer.h"
#include "ParticleGenerator.h"
#include "input.hxx"
#include "spdlog/spdlog.h"
#include <fstream>
#include <iostream>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/PlatformUtils.hpp>

using namespace xml_schema;
using namespace xercesc;

namespace inputReader {
    XMLTreeReader::XMLTreeReader() = default;
    XMLTreeReader::~XMLTreeReader() = default;


    void XMLTreeReader::readFile(ParticleContainer& container, const char* filename, const char* xsd_schema, Environment& environment) {
        std::ifstream XMLFile(filename);
        if (!XMLFile.is_open()) {
            spdlog::error("Could not open file {}", filename);
            std::exit(EXIT_FAILURE);
        }
        try {
            // initializes xerces-c
            XMLPlatformUtils::Initialize();

        } catch (const XMLException& e) {
            char* message = xercesc_3_2::XMLString::transcode(e.getMessage());
            spdlog::error("Mistake Initiliazing xerces-c: {}", message);
            XMLString::release(&message);
            std::exit(EXIT_FAILURE);
        }

        // create a Parser
        XercesDOMParser* parser = new XercesDOMParser();
        parser->setValidationScheme(XercesDOMParser::Val_Always);
        parser->setDoNamespaces(true);
        parser->setValidationSchemaFullChecking(true);

        parser->setExternalNoNamespaceSchemaLocation(xsd_schema);

        HandlerBase* error_handler;
        parser->setErrorHandler(error_handler);

        try {
            parser->parse(filename);
            spdlog::info("The XML-File {} is valid", filename);

        } catch (const XMLException& e) {
            char* message = XMLString::transcode(e.getMessage());
            spdlog::error("XMLException: {}" , message);
            XMLString::release(&message);
            delete parser;
            delete error_handler;
            std::exit(EXIT_FAILURE);

        } catch (const SAXParseException& e) {
            char* message = XMLString::transcode(e.getMessage());
            spdlog::error("SAXParseException: {}", message);
            XMLString::release(&message);
            delete parser;
            delete error_handler;
            std::exit(EXIT_FAILURE);
        } catch (...) {
            spdlog::error("Unknown exception occured");
            delete parser;
            delete error_handler;
            std::exit(EXIT_FAILURE);
        }
        delete parser;
        delete error_handler;
        XMLPlatformUtils::Terminate();


        spdlog::info("Reading XML file {}", filename);

        // read in the simulation data
        std::unique_ptr<sim_t> sim = simulation(XMLFile);

        if (!sim) {
            spdlog::critical("Could not open file {}", filename);
            return;
        }


        const char* output_file_name = sim->output().name().c_str();
        environment.set_output_file_name(output_file_name);

        int write_frequency = sim->output().frequency();
        environment.set_print_step(write_frequency);

        const double t_end = sim->param().t_end();
        environment.set_t_end(t_end);

        const double delta_t = sim->param().delta_t();
        environment.set_delta_t(delta_t);


        int num_particles = sim->particle().size();

        std::array<double, 3> x = { 0.0, 0.0, 0.0 };
        std::array<double, 3> v = { 0.0, 0.0, 0.0 };
        std::array<int, 3> N;
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

            container[i].setX({ particle.pos_x(), particle.pos_y(), particle.pos_z() });
            container[i].setV({ particle.vel_x(), particle.vel_y(), particle.vel_z() });
            container[i].setM(particle.m());
            i++;
        }

        const auto& cuboids = sim->cuboid();
        ParticleGenerator generator;

        // initialize all the cuboids into the container
        for (const auto& cuboid : cuboids) {

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

}
