
#include "MolSim.h"
#include "boundaries/BoxContainer.h"
#include "boundaries/HaloStepper.h"
#include "boundaries/InfContainer.h"
#include "boundaries/InfStepper.h"
#include "boundaries/OutStepper.h"
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

    // TODO: Use the correct containers.

    // Initialize the simulation environment, readers and writers.
    Environment env(argc, argv);

    spdlog::info("Started {}", argv[0]);

    // Initialize the file reader.
    std::unique_ptr<inputReader::Reader> reader { nullptr };

    switch (env.get_input_file_format()) {
    case TXT:
        reader.reset(new inputReader::FileReader());
        break;
    case XML:
        reader.reset(new inputReader::XMLTreeReader());
        break;
    default:
        spdlog::critical("Error: Illegal input file format specifier.");
        std::exit(EXIT_FAILURE);
        break;
    }

    std::shared_ptr<ParticleContainer> cont { nullptr };

    switch (INF_CONT) {
    case INF_CONT:
        cont.reset(new InfContainer());
        break;
    case HALO:
        spdlog::warn("Not yet implemented.");
        std::exit(EXIT_FAILURE);
        break;
    case HARD:
        spdlog::warn("Not yet implemented.");
        std::exit(EXIT_FAILURE);
        break;
    case PERIODIC:
        spdlog::warn("Not yet implemented.");
        std::exit(EXIT_FAILURE);
        break;
    case OUTFLOW:
        spdlog::warn("Not yet implemented.");
        std::exit(EXIT_FAILURE);
        break;
    default:
        spdlog::critical("Error: Illegal Boundary condition specifier.");
        std::exit(EXIT_FAILURE);
        break;
    }

    reader->readFile(env.get_input_file_name(), env, *cont);

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
    std::unique_ptr<Stepper> stepper { nullptr };

    switch (INF_CONT) {
    case INF_CONT:
        stepper.reset(new InfStepper());
        break;
    case HALO:
        stepper.reset(new HaloStepper());
        break;
    case HARD:
        spdlog::warn("Not yet implemented.");
        std::exit(EXIT_FAILURE);
        break;
    case OUTFLOW:
        stepper.reset(new OutStepper());
        break;
    case PERIODIC:
        spdlog::warn("Not yet implemented.");
        std::exit(EXIT_FAILURE);
        break;
    default:
        spdlog::critical("Error: Illegal step specifier.");
        std::exit(EXIT_FAILURE);
        break;
    }

    // Initialize the simulation environment
    double current_time = 0.0;
    int iteration = 0;

    // Write step 0
    const std::string out_name(env.get_output_file_name());
    writer->plotParticles(*cont, out_name, iteration);

    // For this loop, we assume: current x, current f and current v are known
    while (current_time < env.get_t_end()) {
        // Update x, v, f
        stepper->step(*calculator);

        iteration++;
        current_time += env.get_delta_t();

        // Store the particles to an output file
        if (iteration % env.get_print_step() == 0) {
            writer->plotParticles(*cont, out_name, iteration);
        }

        // End the iteration
        spdlog::info("Iteration {} finished.", iteration);
    }

    // Store the particles to an output file if it is the last iteration
    if (iteration % env.get_print_step() != 0) {
        writer->plotParticles(*cont, out_name, iteration);
    }

    spdlog::info("output written. Terminating...");
    return 0;
}
