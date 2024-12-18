#include "XMLTreeReader.h"

namespace inputReader {

    XMLTreeReader::XMLTreeReader(const char* filename) {
        spdlog::debug("Parsing XML input {}", filename);
        // Testing if filename is correct.
        spdlog::trace("Verifying file name...");
        std::ifstream XMLFile(filename);
        if (!XMLFile.is_open()) {
            spdlog::critical("Could not open file {}", filename);
            std::exit(EXIT_FAILURE);
        }

        // Validating and parsing the input into tree object.
        spdlog::trace("Parsing XML file...");
        try {
            sim = simulation(filename);
        } catch (const xml_schema::exception& e) {
            spdlog::critical("XML validation error: {}", e.what());
            std::exit(EXIT_FAILURE);
        } catch (const std::exception& e) {
            spdlog::critical("Error: {}", e.what());
            std::exit(EXIT_FAILURE);
        }

        spdlog::trace("...Finished parsing XML input {}", filename);
    }

    XMLTreeReader::~XMLTreeReader() = default;

    void XMLTreeReader::readArguments(Environment& environment) {
        spdlog::debug("Setting up simulation environment");

        spdlog::trace("Loading output parameters...");
        std::string output_file_name = sim->output().name();
        environment.set_output_file_name(output_file_name);

        const output_t::format_type form = sim->output().format();
        environment.set_output_file_format(static_cast<OutputFormat>(static_cast<int>(form)));

        const int write_frequency = sim->output().frequency();
        environment.set_print_step(write_frequency);

        spdlog::trace("Loading environment arguments...");
        const param_t::calc_type::value calc = sim->param().calc();
        environment.set_calculator_type(static_cast<CalculatorType>(static_cast<int>(calc)));

        const bound::value yz_near = sim->param().boundaries().boundary_yz_near();
        const bound::value xz_near = sim->param().boundaries().boundary_xz_near();
        const bound::value xy_near = sim->param().boundaries().boundary_xy_near();
        const bound::value yz_far = sim->param().boundaries().boundary_yz_far();
        const bound::value xy_far = sim->param().boundaries().boundary_xy_far();
        const bound::value xz_far = sim->param().boundaries().boundary_xz_far();
        environment.set_boundary_type(std::array<BoundaryType, 6> {
            static_cast<BoundaryType>(static_cast<int>(yz_near)),
            static_cast<BoundaryType>(static_cast<int>(xz_near)),
            static_cast<BoundaryType>(static_cast<int>(xy_near)),
            static_cast<BoundaryType>(static_cast<int>(yz_far)),
            static_cast<BoundaryType>(static_cast<int>(xz_far)),
            static_cast<BoundaryType>(static_cast<int>(xy_far)),
        });

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

        const std::array<double, 3> domain_size = {
            sim->param().domain().vx(),
            sim->param().domain().vy(),
            sim->param().domain().vz(),
        };
        environment.set_domain_size(domain_size);

        spdlog::trace("...Finished setting up simulation environment");
    }

    void XMLTreeReader::readParticle(ParticleContainer& container) {
        spdlog::debug("Generating particles");

        const int num_dimensions = sim->param().dimensions();

        size_t t = sim->particle().size();
        if (t > INT_MAX) {
            spdlog::critical("Particle size too large");
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

        // Initialize all the single particles into the container.
        spdlog::trace("Single particles...");
        container.resize(num_particles);

        const auto& particles = sim->particle();

        int i = 0;
        for (const auto& particle : particles) {
            container[i].setX({ particle.position().vx(), particle.position().vy(), particle.position().vz() });
            container[i].setV({ particle.velocity().vx(), particle.velocity().vy(), particle.velocity().vz() });
            container[i].setM(particle.m());
            i++;
        }

        const auto& cuboids = sim->cuboid();
        ParticleGenerator generator;

        // Initialize all the cuboids into the container.
        spdlog::trace("Cuboids...");
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

        // Initialize all the discs into the container.
        spdlog::trace("Discs...");
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

        spdlog::trace("...Finished generating particles");
    }

    int XMLTreeReader::num_particles_added(double h, double r) {
        int particles_future_added = 0;

        double radius_distance = h * r;

        // Spliting the disc into four equal parts to get easy symmetry.
        for (double x_offset = 0; x_offset <= radius_distance; x_offset += h) {
            double max_y_offset = sqrt(radius_distance * radius_distance - x_offset * x_offset);
            for (double y_offset = 0; y_offset <= max_y_offset; y_offset += h) {
                particles_future_added++;

                if (x_offset != 0)
                    particles_future_added++;
                if (y_offset != 0)
                    particles_future_added++;
                if (x_offset != 0 && y_offset != 0)
                    particles_future_added++;
            }
        }

        return particles_future_added;
    }
} // namespace inputReader
