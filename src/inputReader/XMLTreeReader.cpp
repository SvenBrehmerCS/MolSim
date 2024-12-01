#include "XMLTreeReader.h"

using namespace xml_schema;

namespace inputReader {

    XMLTreeReader::XMLTreeReader() = default;

    XMLTreeReader::~XMLTreeReader() = default;

    void XMLTreeReader::readFile(const char* filename, Environment& environment, ParticleContainer& container) {
        spdlog::debug("Reading XML input {}", filename);

        // Test if filename is correct
        std::ifstream XMLFile(filename);
        if (!XMLFile.is_open()) {
            spdlog::error("Could not open file {}", filename);
            std::exit(EXIT_FAILURE);
        }

        // read in the simulation data
        std::unique_ptr<sim_t> sim;
        try {
            sim = simulation(filename);
        } catch (const xml_schema::exception& e) {
            spdlog::error("XML validation error: {}", e.what());
            std::exit(EXIT_FAILURE);
        } catch (const std::exception& e) {
            spdlog::error("Error: {}", e.what());
            std::exit(EXIT_FAILURE);
        }

        spdlog::debug("Setting up simulation environment");

        std::string output_file_name = sim->output().name();
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
            container[i].setX({ particle.position().vx(), particle.position().vy(), particle.position().vz() });
            container[i].setV({ particle.velocity().vx(), particle.velocity().vy(), particle.velocity().vz() });
            container[i].setM(particle.m());
            i++;
        }

        const auto& cuboids = sim->cuboid();
        ParticleGenerator generator;

        // initialize all the cuboids into the container
        for (const auto& cuboid : cuboids) {
            x = { cuboid.position().vx(), cuboid.position().vy(), cuboid.position().vz() };
            v = { cuboid.velocity().vx(), cuboid.velocity().vy(), cuboid.velocity().vz() };
            N = { cuboid.count().vx(), cuboid.count().vy(), cuboid.count().vz() };
            m = cuboid.m();
            h = cuboid.h();
            brownian_motion = cuboid.b_motion();

            container.resize(num_particles + cuboid.count().vx() * cuboid.count().vy() * cuboid.count().vz());

            generator.generateCuboid(container, num_particles, x, v, m, N, h, brownian_motion, num_dimensions);

            num_particles += cuboid.count().vx() * cuboid.count().vy() * cuboid.count().vz();
        }

        const auto& discs = sim->disc();
        for (const auto& disc : discs) {
            disc_center = { disc.center().vx(), disc.center().vy(), disc.center().vz() };
            disc_velocity = { disc.velocity().vx(), disc.velocity().vy(), disc.velocity().vz() };

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
