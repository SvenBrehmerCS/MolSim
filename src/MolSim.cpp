
#include "MolSim.h"
#include "inputReader/FileReader.h"
#include "outputWriter/VTKWriter.h"
#include "outputWriter/XYZWriter.h"
#include "physicsCalculator/SimpleCalculator.h"
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

    spdlog::info("Started {}", argv[0]);

    // Initialize the simulation environment, readers and writers.
    Environment env(argc, argv);

    // Initialize the calculator
    std::unique_ptr<physicsCalculator::Calculator> calculator { new physicsCalculator::SimpleCalculator(env) };

    // Initialize the writer
    std::unique_ptr<outputWriter::Writer> writer { nullptr };

    switch (env.get_output_file_format()) {
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

    // For this loop, we assume: current x, current f and current v are known
    while (current_time < env.get_t_end()) {
        // Update x, v, f
        calculator->step();

        iteration++;

        // Store the particles to an output file
        if (iteration % env.get_print_step() == 0) {
            std::string out_name(env.get_output_file_name());

            writer->plotParticles(calculator->get_particles(), out_name, iteration);
        }

        current_time += env.get_delta_t();

        // End the iteration and initialize the new one
        spdlog::info("Iteration {} finished.", iteration);
    }

    spdlog::info("output written. Terminating...");
    return 0;
}
