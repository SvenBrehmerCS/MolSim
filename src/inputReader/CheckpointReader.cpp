//
// Created by frederik on 12/6/24.
//

#include "CheckpointReader.h"

#include "spdlog/spdlog.h"

#include <fstream>

namespace inputReader {
    // TODO Change for new particles and container (We might lose the mass)
    void inputReader::CheckpointReader::readSimulation(ParticleContainer& container, const char* filename) {
        std::ifstream inputFile(filename, std::ios::binary);

        if (!inputFile.is_open()) {
            SPDLOG_ERROR("Could not open file {}", filename);
            std::exit(EXIT_FAILURE);
        }
        size_t num_particles = 0;

        inputFile.read(reinterpret_cast<char*>(&num_particles), sizeof(size_t));
        size_t new_particles = container.size();
        container.resize(new_particles + num_particles);
        SPDLOG_DEBUG("Reading num_particles from CheckPoint {} and read {} particles from the xml ", num_particles, new_particles);

        double x, y, z, vx, vy, vz, fx, fy, fz;
        int type;

        for (size_t i = new_particles; i < num_particles + new_particles; i++) {
            // inputFile >> x >> y >> z >> vx >> vy >> vz >> type >> fx >> fy >> fz >> m;

            inputFile.read(reinterpret_cast<char*>(&x), sizeof(double));
            inputFile.read(reinterpret_cast<char*>(&y), sizeof(double));
            inputFile.read(reinterpret_cast<char*>(&z), sizeof(double));

            inputFile.read(reinterpret_cast<char*>(&vx), sizeof(double));
            inputFile.read(reinterpret_cast<char*>(&vy), sizeof(double));
            inputFile.read(reinterpret_cast<char*>(&vz), sizeof(double));

            inputFile.read(reinterpret_cast<char*>(&type), sizeof(int));

            inputFile.read(reinterpret_cast<char*>(&fx), sizeof(double));
            inputFile.read(reinterpret_cast<char*>(&fy), sizeof(double));
            inputFile.read(reinterpret_cast<char*>(&fz), sizeof(double));

            container[i].setX({ x, y, z });
            container[i].setV({ vx, vy, vz });
            container[i].setType(type);
            container[i].setF({ fx, fy, fz });
        }
    }
}