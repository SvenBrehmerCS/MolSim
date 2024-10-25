
#include "MolSim.h"
#include "FileReader.h"
#include "outputWriter/VTKWriter.h"
#include "outputWriter/XYZWriter.h"
#include "utils/ArrayUtils.h"

#include <cmath>
#include <iostream>
#include <string>
#include <vector>


std::vector<Particle> particles;

/**
 * The main entry point for the programm.
 */
int main(const int argc, const char* argv[]) {

    std::cout << "Started " << argv[0] << std::endl;

    // Initialize the simulation environment, readers and writers.
    Environment env(argc, argv);

    FileReader fileReader;
    fileReader.readFile(particles, env.get_input_file_name());

    std::unique_ptr<outputWriter::Writer> writer { nullptr };

    switch (env.get_output_file_format()) {
    case VTK:
        writer.reset(new outputWriter::VTKWriter());
        break;

    case XYZ:
        writer.reset(new outputWriter::XYZWriter());
        break;

    default:
        break;
    }

    // Initialize the
    double current_time = 0.0;
    int iteration = 0;

    // for this loop, we assume: current x, current f and current v are known
    while (current_time < env.get_t_end()) {
        // calculate new x
        calculateX(env);
        // calculate new f
        calculateF(env);
        // calculate new v
        calculateV(env);

        iteration++;

        // Store the particles to an output file
        if (iteration % env.get_print_step() == 0) {
            std::string out_name(env.get_output_file_name());

            writer->plotParticles(particles, out_name, iteration);
        }

        // End the iteration and initialize the new one
        std::cout << "Iteration " << iteration << " finished." << std::endl;

        current_time += env.get_delta_t();
    }

    std::cout << "output written. Terminating..." << std::endl;
    return 0;
}

void calculateF(const Environment& env) {
    // Set the old f to f and reset the current forces
    for (Particle& p : particles) {
        p.setOldF(p.getF());
        p.setF({ 0.0, 0.0, 0.0 });
    }

    for (size_t i = 0; i < particles.size(); i++) {
        for (size_t j = i + 1; j < particles.size(); j++) {
            // Calculate the distance and force experienced by two particles
            const double distance = ArrayUtils::L2Norm(particles[j].getX() - particles[i].getX());
            const double force = particles[i].getM() * particles[j].getM() / (distance * distance * distance);

            // Update the forces fo rboth particles
            particles[i].setF(force * (particles[j].getX() - particles[i].getX()) + particles[i].getF());
            particles[j].setF(force * (particles[i].getX() - particles[j].getX()) + particles[j].getF());
        }
    }
}

void calculateX(const Environment& env) {
    for (Particle& p : particles) {
        p.setX(p.getX() + env.get_delta_t() * p.getV() + (env.get_delta_t() * env.get_delta_t() * 0.5 / p.getM()) * p.getF());
    }
}

void calculateV(const Environment& env) {
    for (Particle& p : particles) {
        p.setV(p.getV() + (env.get_delta_t() * 0.5 / p.getM()) * (p.getOldF() + p.getF()));
    }
}
