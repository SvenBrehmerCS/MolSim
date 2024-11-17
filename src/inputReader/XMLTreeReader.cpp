#include "XMLTreeReader.h"

#include "ParticleContainer.h"
#include "ParticleGenerator.h"
#include "spdlog/spdlog.h"

#include <fstream>
#include <iostream>

namespace inputReader {
    XMLTreeReader::XMLTreeReader() = default;
    XMLTreeReader::~XMLTreeReader() = default;


    void XMLTreeReader::readFile(ParticleContainer& container, const char* filename) {
        std::ifstream XMLFile(filename);
        if (!XMLFile.is_open()) {
            spdlog::error("Could not open file {}", filename);
            return;
        }


        spdlog::info("Reading XML file {}", filename);

        // read in the simulation data
        std::unique_ptr<sim_t> sim = simulation(XMLFile);

        //TODO !!! the following 4 variables are already red in but not used!

        /*
        const char* output_file_name = sim->output().name().c_str();
        int write_frequency = sim->output().frequency();

        double t_end = sim->param().t_end();
        double delta_t = sim->param().delta_t;
        */

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
