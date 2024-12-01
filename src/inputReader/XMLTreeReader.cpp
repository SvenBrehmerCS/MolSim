#include "XMLTreeReader.h"

using namespace xml_schema;

namespace inputReader {

    XMLTreeReader::XMLTreeReader(const char* filename) {
        spdlog::debug("Reading XML input {}", filename);

        // Test if filename is correct
        std::ifstream XMLFile(filename);
        if (!XMLFile.is_open()) {
            spdlog::error("Could not open file {}", filename);
            std::exit(EXIT_FAILURE);
        }

        // read in the simulation data
        try {
            sim = simulation(filename);
        } catch (const xml_schema::exception& e) {
            spdlog::error("XML validation error: {}", e.what());
            std::exit(EXIT_FAILURE);
        } catch (const std::exception& e) {
            spdlog::error("Error: {}", e.what());
            std::exit(EXIT_FAILURE);
        }
    }
    XMLTreeReader::~XMLTreeReader() = default;

    void XMLTreeReader::readEnvironment(Environment& environment) {
        spdlog::debug("Setting up simulation environment");

        const char* output_file_name = sim->output().name().c_str();
        environment.set_output_file_name(output_file_name);

        const output_t::format_type form = sim->output().format();
        environment.set_output_file_format(static_cast<OutputFormat>(static_cast<int>(form)));

        const int write_frequency = sim->output().frequency();
        environment.set_print_step(write_frequency);

        const param_t::calc_type::value calc = sim->param().calc();
        environment.set_calculator_type(static_cast<CalculatorType>(static_cast<int>(calc)));

        const double epsilon = sim->param().epsilon();
        environment.set_epsilon(epsilon);

        const double sigma = sim->param().sigma();
        environment.set_sigma(sigma);

        const double delta_t = sim->param().delta_t();
        environment.set_delta_t(delta_t);

        const double t_end = sim->param().t_end();
        environment.set_t_end(t_end);

        const double r_cutoff = sim->param().r_cutoff();
        environment.set_r_cutoff(r_cutoff);
    }

    void XMLTreeReader::readParticles(ParticleContainer& container) {
        // TODO rest and maybe move restrictions into the xsd
        // if (sim->param().t_end() <= 0 || sim->param().delta_t() <= 0) {
        //     spdlog::critical("Invalid simulation parameters in file {}", filename);
        //     std::exit(EXIT_FAILURE);
        // }

        spdlog::debug("Generating particles");

        const int num_dimensions = sim->param().dimensions();

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
        double brownian_motion;

        std::array<double, 3> disc_center = { 0.0, 0.0, 0.0 };
        std::array<double, 3> disc_velocity = { 0.0, 0.0, 0.0 };

        container.resize(num_particles);

        const auto& particles = sim->particle();

        // initialize all the particles from the XML file into the container
        int i = 0;
        for (const auto& particle : particles) {
            if (particle.m() <= 0) { // TODO
                spdlog::critical("Invalid particle mass: {}", particle.m());
                std::exit(EXIT_FAILURE);
            }
            if (std::isnan(particle.pos_x()) || std::isnan(particle.pos_y()) || std::isnan(particle.pos_z())) {
                spdlog::critical("Invalid particle position. Position cant be NaN.");
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
            if (cuboid.n_x() <= 0 || cuboid.n_y() <= 0 || cuboid.n_z() <= 0) { // TODO
                spdlog::critical("Invalid cuboid dimensions in simulation file {}");
                std::exit(EXIT_FAILURE);
            }
            if (cuboid.m() <= 0) {
                spdlog::critical("Invalid cuboid mass in simulation file {}");
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

        const auto& discs = sim->disc();
        for (const auto& disc : discs) {
            disc_center = { disc.c_x(), disc.c_y(), disc.c_z() };
            disc_velocity = { disc.vel_x(), disc.vel_y(), disc.vel_z() };

            int particles_future_added = num_particles_added(disc.h(), disc.r());

            container.resize(num_particles + particles_future_added);


            int particles_added = generator.generateDisc(
                container, num_particles, disc_center, disc_velocity, disc.m(), disc.r(), disc.h(), disc.b_motion(), num_dimensions);

            num_particles += particles_added;
        }
    }
    int XMLTreeReader::num_particles_added(double h, double r) {

        int particles_future_added = 0;

        int radius_distance = h * r;
        for (double x = -radius_distance; x <= radius_distance; x += h) {
            for (double y = -radius_distance; y <= radius_distance; y += h) {
                if (x * x + y * y <= radius_distance * radius_distance) {
                    particles_future_added++;
                }
            }
        }
        return particles_future_added;
    }

}
