
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

#include <filesystem>
#include <iostream>
#include <spdlog/spdlog.h>
#include <string>

/**
 * The main entry point for the program.
 */
int main(const int argc, const char* argv[]) {
    namespace fs = std::filesystem;

    // Initialize the simulation environment.
    Environment env { argc, argv };

    spdlog::info("Started {}", argv[0]);

    // Initialize the file reader.
    std::unique_ptr<inputReader::Reader> reader { nullptr };

    switch (env.get_input_file_format()) {
    case TXT:
        reader = std::make_unique<inputReader::FileReader>(env.get_input_file_name());
        // REMOVE reader.reset(new inputReader::FileReader(env.get_input_file_name()));
        break;
    case XML:
        reader = std::make_unique<inputReader::XMLTreeReader>(env.get_input_file_name());
        // REMOVE reader.reset(new inputReader::XMLTreeReader(env.get_input_file_name()));
        break;
    default:
        spdlog::critical("Error: Illegal input file format specifier.");
        std::exit(EXIT_FAILURE);
        break;
    }

    reader->readArguments(env);

    std::shared_ptr<ParticleContainer> cont { nullptr };

    if (env.requires_direct_sum()) {
        cont.reset(new DSContainer(env.get_domain_size()));
    } else {
        cont.reset(new BoxContainer(env.get_r_cutoff(), env.get_domain_size()));
    }

    reader->readParticle(*cont, env.get_delta_t(), env.get_gravity());
    reader.release();
    env.assert_boundary_conditions();

    // Initialize the calculator.
    std::unique_ptr<physicsCalculator::Calculator> calculator { nullptr };

    switch (env.get_calculator_type()) {
    case GRAVITY:
        calculator.reset(new physicsCalculator::GravityCalculator(env, cont));
        break;
    case LJ_FULL:
        calculator.reset(new physicsCalculator::LJCalculator(env, cont));
        break;
    default:
        spdlog::critical("Error: Illegal force model specifier.");
        std::exit(EXIT_FAILURE);
        break;
    }

    // Initialize the writer.
    std::unique_ptr<outputWriter::Writer> writer { nullptr };

    switch (env.get_output_file_format()) {
    case NO_OUT:
        writer.reset(new outputWriter::NoWriter());
        break;
    case VTK:
        writer.reset(new outputWriter::VTKWriter());
        break;
    case XYZ:
        writer.reset(new outputWriter::XYZWriter());
        break;
    case CHECKPOINT:
        writer.reset(new outputWriter::VTKWriter());
        break;
    default:
        spdlog::critical("Error: Illegal file format specifier.");
        std::exit(EXIT_FAILURE);
        break;
    }

    // Initialize the stepper.
    Stepper stepper { env.get_boundary_type(), calculator->get_env().get_domain_size() };
    // Initialize the thermostat.
    Thermostat thermostat { env.get_dimensions(), env.get_temp_target(), env.get_max_delta_temp(), cont };

    // Initialize the simulation environment.
    double current_time = 0.0;
    int iteration = 0;

    // Write step 0
    const std::string out_name(env.get_output_file_name());
    writer->plotParticles(*cont, out_name, iteration);

    // Get the start time of the simulation
    const auto start_time = std::chrono::steady_clock::now();
    spdlog::logger time_logger("Time Logger");

    // For this loop, we assume: current x, current f and current v are known
    while (current_time < env.get_t_end()) {
        // Update x, v, f
        stepper.step(*calculator);

        iteration++;
        current_time += env.get_delta_t();

        // Apply thermostat
        if (iteration % env.get_temp_frequency() == 0)
            thermostat.regulate_Temperature();

        // Store the particles to an output file
        if (iteration % env.get_print_step() == 0) {
            writer->plotParticles(*cont, out_name, iteration);
            spdlog::info("Iteration {} finished.", iteration);
        }
    }

    // Get the start time of the simulation
    const auto end_time = std::chrono::steady_clock::now();
    const auto ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    time_logger.info("The simulation took {} ms. The update time for a single particle was {} ns.", ms_duration.count(),
        ms_duration.count() / (iteration * cont->size()));

    // TODO hier Simulation checkpoint setzen wenn outputformat = checkpoint gesetzt
    if (env.get_output_file_format() == CHECKPOINT) {
        spdlog::info("Checkpoint written.");
        outputWriter::CheckpointWriter checkpoint_writer;
        const char* filename = env.get_output_file_name();
        checkpoint_writer.plot(*cont, filename);
    }

    spdlog::info("Output written. Terminating...");

    return 0;
}
