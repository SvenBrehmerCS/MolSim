
#include "MolSim.h"
#include "boundaries/BoxContainer.h"
#include "boundaries/GhostBoundary.h"
#include "boundaries/HardBoundary.h"
#include "boundaries/InfContainer.h"
#include "boundaries/NoBoundary.h"
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

    // Initialize the simulation environment.
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

    // Initialize the particle container.
    bool isinf = true;
    std::array<BoundaryType, 6> bound_t = env.get_boundary_type();
    std::array<Boundary*, 6> boundaries {};
    for (size_t i = 0; i < 6; i++) {
        const double pos = i < 3 ? 0.0 : env.get_domain_size()[i % 3];
        switch (bound_t[i]) {
        case INF_CONT:
            boundaries[i] = new NoBoundary(pos, i % 3);
            break;
        case HALO:
            boundaries[i] = new GhostBoundary(pos, i % 3);
            isinf = false;
            break;
        case HARD:
            boundaries[i] = new HardBoundary(pos, i % 3);
            isinf = false;
            break;
        case PERIODIC:
            spdlog::warn("Not yet iimplemented.");
            std::exit(EXIT_FAILURE);
            isinf = false;
            break;
        case OUTFLOW:
            boundaries[i] = new NoBoundary(pos, i % 3);
            isinf = false;
            break;
        default:
            spdlog::critical("Unsupported boundary type.");
            std::exit(EXIT_FAILURE);
            break;
        }
    }

    std::shared_ptr<ParticleContainer> cont { nullptr };

    if (isinf) {
        cont.reset(new InfContainer(env.get_domain_size()));
    } else {
        cont.reset(new BoxContainer(env.get_r_cutoff(), env.get_domain_size()));
    }

    reader->readParticle(*cont);

    reader.reset(nullptr);

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
    default:
        spdlog::critical("Error: Illegal file format specifier.");
        std::exit(EXIT_FAILURE);
        break;
    }

    // Initialize the stepper.
    Stepper stepper(boundaries, isinf);

    // Initialize the simulation environment.
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
            spdlog::info("Iteration {} finished.", iteration);
        }
    }

    spdlog::info("Output written. Terminating...");

    // Freeing allocated memory
    for (auto b : boundaries) {
        delete b;
    }
    return 0;
}
