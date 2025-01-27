#include "MolSim.h"
#include "Thermostat.h"
#include "boundaries/Stepper.h"
#include "container/BoxContainer.h"
#include "container/DSContainer.h"
#include "inputReader/FileReader.h"
#include "inputReader/XMLTreeReader.h"
#include "outputWriter/CheckpointWriter.h"
#include "outputWriter/NoWriter.h"
#include "outputWriter/VTKWriter.h"
#include "outputWriter/XYZWriter.h"
#include "physicsCalculator/GravityCalculator.h"
#include "physicsCalculator/LJCalculator.h"

#include <iostream>
#include <spdlog/spdlog.h>
#include <string>

/**
 * The main entry point for the program.
 */
int main(const int argc, const char* argv[]) {
    // Initialize the simulation environment.
    Environment env { argc, argv };

    SPDLOG_INFO("Started {}", argv[0]);

    // Initialize the file reader.
    std::unique_ptr<inputReader::Reader> reader { nullptr };

    switch (env.get_input_file_format()) {
    case TXT:
        reader = std::make_unique<inputReader::FileReader>(env.get_input_file_name());
        break;
    case XML:
        reader = std::make_unique<inputReader::XMLTreeReader>(env.get_input_file_name());
        break;
    default:
        SPDLOG_CRITICAL("Error: Illegal input file format specifier.");
        std::exit(EXIT_FAILURE);
        break;
    }

    // Initialize the thermostat.
    Thermostat thermostat;

    reader->readArguments(env, thermostat);

    std::shared_ptr<ParticleContainer> cont { nullptr };

    if (env.requires_direct_sum()) {
        cont = std::make_shared<DSContainer>(env.get_domain_size());
    } else {
        cont = std::make_shared<BoxContainer>(env.get_r_cutoff(), env.get_domain_size(), env.get_update_strategy());
    }

    reader->readParticle(*cont, env.get_delta_t(), env.get_gravity());
    reader.reset();
    env.assert_boundary_conditions();

    // Initialize the calculator.
    std::unique_ptr<physicsCalculator::Calculator> calculator { nullptr };

    switch (env.get_calculator_type()) {
    case GRAVITY:
        calculator = std::make_unique<physicsCalculator::GravityCalculator>(env, cont);
        break;
    case LJ_FULL:
        calculator = std::make_unique<physicsCalculator::LJCalculator>(env, cont);
        break;
    default:
        SPDLOG_CRITICAL("Error: Illegal force model specifier.");
        std::exit(EXIT_FAILURE);
        break;
    }

    // Initialize the writer.
    std::unique_ptr<outputWriter::Writer> writer { nullptr };

    switch (env.get_output_file_format()) {
    case NO_OUT:
        writer = std::make_unique<outputWriter::NoWriter>();
        break;
    case VTK:
        writer = std::make_unique<outputWriter::VTKWriter>();
        break;
    case XYZ:
        writer = std::make_unique<outputWriter::XYZWriter>();
        break;
    case CHECKPOINT:
        writer = std::make_unique<outputWriter::VTKWriter>();
        break;
    default:
        SPDLOG_CRITICAL("Error: Illegal file format specifier.");
        std::exit(EXIT_FAILURE);
        break;
    }

    // Initialize the stepper.
    Stepper stepper { env.get_boundary_type(), env.get_domain_size() };

    // Fully initialise Thermostat
    thermostat.set_particles(cont);

    // Initialize the simulation environment.
    double current_time = 0.0;
    int iteration = 0;

    // Write step 0
    const std::string out_name(env.get_output_file_name());
    writer->plotParticles(*cont, out_name, iteration);

    // Get the start time of the simulation
    const auto start_time = std::chrono::steady_clock::now();

    // For this loop, we assume: current x, current f and current v are known
    while (current_time < env.get_t_end()) {
        // Update x, v, f
        stepper.step(*calculator);

        iteration++;
        current_time += env.get_delta_t();

        // Apply thermostat
        if (thermostat.get_active() && iteration % env.get_temp_frequency() == 0)
            thermostat.regulate_Temperature();

        // Store the particles to an output file
        if (iteration % env.get_print_step() == 0) {
            writer->plotParticles(*cont, out_name, iteration);
            SPDLOG_INFO("Iteration {} finished.", iteration);
        }
    }

    // Get the start time of the simulation (std::cout is used for performance measurements when the log level is off)
    const auto end_time = std::chrono::steady_clock::now();
    const auto ns_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
    std::cout << "The simulation took " << ns_duration / 1000000.0 << " ms. The update time for a single particle was "
              << (ns_duration / static_cast<double>(iteration * cont->size()) / 1000.0) << " µs." << std::endl;

    if (env.get_output_file_format() == CHECKPOINT) {
        SPDLOG_INFO("Checkpoint written.");
        outputWriter::CheckpointWriter checkpoint_writer;
        const char* filename = env.get_output_file_name();
        checkpoint_writer.plot(*cont, env, filename);
    }

    SPDLOG_INFO("Output written. Terminating...");

    return 0;
}
