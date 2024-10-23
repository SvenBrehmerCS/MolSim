
#include "Environment.h"
#include "FileReader.h"
#include "outputWriter/VTKWriter.h"
#include "outputWriter/XYZWriter.h"
#include "utils/ArrayUtils.h"

#include <cmath>
#include <iostream>
#include <list>
#include <string>

/**** forward declaration of the calculation functions ****/

/**
 * calculate the force for all particles
 */
void calculateF(const Environment& env);

/**
 * calculate the position for all particles
 */
void calculateX(const Environment& env);

/**
 * calculate the position for all particles
 */
void calculateV(const Environment& env);

/**
 * plot the particles to a xyz-file
 */
void plotParticles(const int iteration, const Environment& env);

// TODO: what data structure to pick? vector
std::list<Particle> particles;

std::unique_ptr<outputWriter::Writer> writer { new outputWriter::VTKWriter() };

// TODO: const argv
int main(const int argc, char* argv[]) {

    std::cout << "Started " << argv[0] << std::endl;

    Environment env(argc, argv);

    FileReader fileReader;
    fileReader.readFile(particles, env.get_input_file_name());

    double current_time = 0.0;
    int iteration = 0;

    // for this loop, we assume: current x, current f and current v are known
    // ? Possible Optimization: Replace some std::endl with "\n" ->
    // ?    Avoid flushing the stream unnecessarily often.
    // ?    Reduce the number of calls to ostream.operator<<.
    // ? Possible Improvement:
    // ?    Measure simulation time.
    // ?    Add an option for disabling and enabling print statements.
    // ?    Add assertions for simpler error detection.
    while (current_time < env.get_t_end()) {
        // calculate new x
        calculateX(env);
        // calculate new f
        calculateF(env);
        // calculate new v
        calculateV(env);

        iteration++;

        if (iteration % env.get_print_step() == 0) {
            plotParticles(iteration, env);
        }

        std::cout << "Iteration " << iteration << " finished." << std::endl;

        current_time += env.get_delta_t();
    }

    std::cout << "output written. Terminating..." << std::endl;
    return 0;
}

void calculateF(const Environment& env) {
    for (Particle& p : particles) {
        p.setOldF(p.getF());
        p.setF({ 0.0, 0.0, 0.0 });
    }

    for (size_t i = 0; i < particles.size(); i++) {
        for (size_t j = i + 1; j < particles.size(); j++) {
            // TODO: Update the forces (requires vector)
            Particle p1, p2;

            const double distance = ArrayUtils::L2Norm(p2.getX() - p1.getX());
            const double force = p1.getM() * p2.getM() / (distance * distance * distance);

            p1.setF(force * (p2.getX() - p1.getX()) + p1.getF());
            p2.setF(force * (p1.getX() - p2.getX()) + p2.getF());
        }
    }

    // for (auto& p1 : particles) {
    //     p1.setOldF(p1.getF());
    //     p1.setF({ 0, 0, 0 });

    //     for (auto& p2 : particles) {
    //         if (p1 == p2) {
    //             continue;
    //         }

    //         const double distance = ArrayUtils::L2Norm(p2.getX() - p1.getX());

    //         const double force = p1.getM() * p2.getM() / (distance * distance * distance);

    //         p1.setF(force * (p2.getX() - p1.getX()) + p1.getF());
    //     }
    // }
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

void plotParticles(const int iteration, const Environment& env) {
    std::string out_name(env.get_output_file_name());

    writer->plotParticles(particles, out_name, iteration);
}
