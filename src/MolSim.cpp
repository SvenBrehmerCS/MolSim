
#include "MolSim.h"
#include "boundaries/BoxContainer.h"
#include "boundaries/InfContainer.h"
#include "boundaries/Stepper.h"
#include "inputReader/FileReader.h"
#include "inputReader/XMLTreeReader.h"
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

    // Initialize the simulation environment, readers and writers.
    Environment env { argc, argv };

    spdlog::info("Started {}", argv[0]);

    // Initialize the file reader.
    std::unique_ptr<inputReader::Reader> reader { nullptr };

    switch (env.get_input_file_format()) {
    case TXT:
        reader.reset(new inputReader::FileReader(env.get_input_file_name()));
        break;
    case XML:
        reader.reset(new inputReader::XMLTreeReader(env.get_input_file_name()));
        break;
    default:
        spdlog::critical("Error: Illegal input file format specifier.");
        std::exit(EXIT_FAILURE);
        break;
    }

    reader->readArguments(env);

    std::shared_ptr<ParticleContainer> cont { nullptr };

    switch (env.get_boundary_type()) {
    case INF_CONT:
        cont.reset(new InfContainer(env.get_domain_size()));
        break;
    case HALO:
        cont.reset(new BoxContainer(env.get_r_cutoff(), env.get_domain_size()));
        break;
    case HARD:
        cont.reset(new BoxContainer(env.get_r_cutoff(), env.get_domain_size()));
        break;
    case PERIODIC:
        cont.reset(new BoxContainer(env.get_r_cutoff(), env.get_domain_size()));
        break;
    case OUTFLOW:
        cont.reset(new BoxContainer(env.get_r_cutoff(), env.get_domain_size()));
        break;
    default:
        spdlog::critical("Error: Illegal Boundary condition specifier.");
        std::exit(EXIT_FAILURE);
        break;
    }

    reader->readParticle(*cont);

    // Initialize the calculator
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

    // Initialize the writer
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
    default:
        spdlog::critical("Error: Illegal file format specifier.");
        std::exit(EXIT_FAILURE);
        break;
    }

    // Initialize the stepper
    // TODO: Boundary conditions
    Stepper stepper({}, false);

    // Initialize the simulation environment
    double current_time = 0.0;
    int iteration = 0;

    // Write step 0
    const std::string out_name(env.get_output_file_name());
    writer->plotParticles(*cont, out_name, iteration);

    // For this loop, we assume: current x, current f and current v are known
    while (current_time < env.get_t_end()) {
        // Update x, v, f
        stepper.step(*calculator);

        iteration++;
        current_time += env.get_delta_t();

        // Store the particles to an output file
        if (iteration % env.get_print_step() == 0) {
            writer->plotParticles(*cont, out_name, iteration);
        }

        // End the iteration
        spdlog::info("Iteration {} finished.", iteration);
    }

    spdlog::info("output written. Terminating...");
    return 0;
}
