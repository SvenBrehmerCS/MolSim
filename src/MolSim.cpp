
#include "MolSim.h"
#include "inputReader/FileReader.h"
#include "outputWriter/VTKWriter.h"
#include "outputWriter/XYZWriter.h"
#include "physicsCalculator/SimpleCalculator.h"
#include "solver/Analytical.h"
#include "utils/ArrayUtils.h"

#include <filesystem>
#include <iostream>
#include <string>

/**
 * The main entry point for the program.
 */
int main(const int argc, const char* argv[]) {
    namespace fs = std::filesystem;

    std::cout << "Started " << argv[0] << std::endl;

    // Initialize the simulation environment, readers and writers.
    Environment env(argc, argv);

    ParticleContainer container;

    std::unique_ptr<inputReader::Reader> fileReader { new inputReader::FileReader() };
    fileReader->readFile(container.get_particles(), env.get_input_file_name());

    // TODO: Comment out befor final merge
    std::cout << "Cleaning up old output files!" << std::endl;
    fs::path dir_path { "." };
    for (auto const& dir_entry : fs::directory_iterator { dir_path }) {
        if (dir_entry.path().extension() == ".vtu" || dir_entry.path().extension() == ".xyz") {
            fs::remove(dir_entry.path());
        }
    }
    // std::string command = "rm ./*.vtu";
    // system(command.c_str());


    std::unique_ptr<outputWriter::Writer> writer { nullptr };

    switch (env.get_output_file_format()) {
    case VTK:
        writer.reset(new outputWriter::VTKWriter());
        break;

    case XYZ:
        writer.reset(new outputWriter::XYZWriter());
        break;

    default:
        std::cout << "Error: Illegal file format specifier." << std::endl;
        std::exit(EXIT_FAILURE);
        break;
    }

    // Initialize the simulation environment
    double current_time = 0.0;
    int iteration = 0;

    // Initialize the calculator
    std::unique_ptr<physicsCalculator::Calculator> calculator { new physicsCalculator::SimpleCalculator() };
    // Initialize the force for the first iteration
    calculator->calculateF(container, env);

    // For this loop, we assume: current x, current f and current v are known
    while (current_time < env.get_t_end()) {
        // calculate new x
        calculator->calculateX(container, env);
        // calculate new f
        calculator->calculateF(container, env);
        // calculate new v
        calculator->calculateV(container, env);

        iteration++;

        // Store the particles to an output file
        if (iteration % env.get_print_step() == 0) {
            std::string out_name(env.get_output_file_name());

            writer->plotParticles(container.get_particles(), out_name, iteration);
        }

        current_time += env.get_delta_t();

        // Test if the solution corresponds to an analytical solution.
        constexpr bool test_analytical = true; // < TODO: Set to false before release
        if (!solver::is_center_evasion_solution(container.get_particles()[0], container.get_particles()[1], current_time) || !test_analytical) {
            std::cout << "Simulation diverged." << std::endl;

            for (const Particle& p : container.get_particles()) {
                std::cout << "    " << p << std::endl;
            }

            std::cout << "Time: " << current_time << std::endl;
        }

        // End the iteration and initialize the new one
        std::cout << "Iteration " << iteration << " finished." << std::endl;
    }

    std::cout << "output written. Terminating..." << std::endl;
    return 0;
}
