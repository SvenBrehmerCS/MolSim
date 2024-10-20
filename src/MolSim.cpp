
#include "FileReader.h"
#include "outputWriter/XYZWriter.h"
#include "utils/ArrayUtils.h"

#include <cmath>
#include <iostream>
#include <list>

/**** forward declaration of the calculation functions ****/

/**
 * calculate the force for all particles
 */
void calculateF();

/**
 * calculate the position for all particles
 */
void calculateX();

/**
 * calculate the position for all particles
 */
void calculateV();

/**
 * plot the particles to a xyz-file
 */
void plotParticles(int iteration);

constexpr double start_time = 0;
constexpr double end_time = 1000;
constexpr double delta_t = 0.014;

// TODO: what data structure to pick?
std::list<Particle> particles;

int main(int argc, char* argsv[]) {

    std::cout << "Hello from MolSim for PSE!" << std::endl;
    if (argc != 2) {
        std::cout << "Erroneous programme call! " << std::endl;
        std::cout << "./molsym filename" << std::endl;
    }

    FileReader fileReader;
    fileReader.readFile(particles, argsv[1]);

    double current_time = start_time;

    int iteration = 0;

    // for this loop, we assume: current x, current f and current v are known
    while (current_time < end_time) {
        // calculate new x
        calculateX();
        // calculate new f
        calculateF();
        // calculate new v
        calculateV();

        iteration++;
        if (iteration % 10 == 0) {
            plotParticles(iteration);
        }
        std::cout << "Iteration " << iteration << " finished." << std::endl;

        current_time += delta_t;
    }

    std::cout << "output written. Terminating..." << std::endl;
    return 0;
}

void calculateF() {
    std::list<Particle>::iterator iterator;
    iterator = particles.begin();

    for (auto& p1 : particles) {
        for (auto& p2 : particles) {
            if (p1 == p2) {
                continue;
            }

            const double distance = std::sqrt((p1.getX()[0] - p2.getX()[0]) * (p1.getX()[0] - p2.getX()[0])
                + (p1.getX()[1] - p2.getX()[1]) * (p1.getX()[1] - p2.getX()[1]) + (p1.getX()[2] - p2.getX()[2]) * (p1.getX()[2] - p2.getX()[2]));

            const double force = p1.getM() * p2.getM() / (distance * distance * distance);

            p1.setOldF(p1.getF());
            p1.setF({ (p2.getX()[0] - p1.getX()[0]) * force, (p2.getX()[1] - p1.getX()[1]) * force, (p2.getX()[2] - p1.getX()[2]) * force });
        }
    }
}

void calculateX() {
    for (auto& p : particles) {
        p.setX({
            p.getX()[0] + delta_t * p.getV()[0] + delta_t * delta_t * 0.5 * p.getF()[0] / p.getM(),
            p.getX()[1] + delta_t * p.getV()[1] + delta_t * delta_t * 0.5 * p.getF()[1] / p.getM(),
            p.getX()[2] + delta_t * p.getV()[2] + delta_t * delta_t * 0.5 * p.getF()[2] / p.getM(),
        });
    }
}

void calculateV() {
    for (auto& p : particles) {
        p.setV({
            p.getV()[0] + delta_t * 0.5 * (p.getOldF()[0] + p.getF()[0]) / p.getM(),
            p.getV()[1] + delta_t * 0.5 * (p.getOldF()[1] + p.getF()[1]) / p.getM(),
            p.getV()[2] + delta_t * 0.5 * (p.getOldF()[2] + p.getF()[2]) / p.getM(),
        });
    }
}

void plotParticles(int iteration) {

    std::string out_name("MD_vtk");

    outputWriter::XYZWriter writer;
    writer.plotParticles(particles, out_name, iteration);
}
