
#include "MolSim.h"
#include "inputReader/FileReader.h"
#include "outputWriter/NoWriter.h"
#include "outputWriter/VTKWriter.h"
#include "outputWriter/XYZWriter.h"
#include "physicsCalculator/GravityCalculator.h"
#include "physicsCalculator/LJCalculator.h"
#include "solver/Analytical.h"
#include "utils/ArrayUtils.h"

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
    Environment env(argc, argv);

    spdlog::info("Started {}", argv[0]);

    // Initialize the calculator
    std::unique_ptr<physicsCalculator::Calculator> calculator { nullptr };

    switch (env.get_calculator_type()) {
    case GRAVITY:
        calculator.reset(new physicsCalculator::GravityCalculator(env));
        break;

    case LJ_FULL:
        calculator.reset(new physicsCalculator::LJCalculator(env));
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


    // Initialize the simulation environment
    double current_time = 0.0;
    int iteration = 0;

    // Write step 0
    const std::string out_name(env.get_output_file_name());
    writer->plotParticles(calculator->get_container(), out_name, iteration);

    // For this loop, we assume: current x, current f and current v are known
    while (current_time < env.get_t_end()) {
        // Update x, v, f
        calculator->step();

        iteration++;
        current_time += env.get_delta_t();

        // Store the particles to an output file
        if (iteration % env.get_print_step() == 0) {
            writer->plotParticles(calculator->get_container(), out_name, iteration);
        }

        // End the iteration
        spdlog::info("Iteration {} finished.", iteration);
    }

    // Store the particles to an output file if it is the last iteration
    if (iteration % env.get_print_step() != 0) {
        writer->plotParticles(calculator->get_container(), out_name, iteration);
    }

    spdlog::info("output written. Terminating...");
    return 0;
}
