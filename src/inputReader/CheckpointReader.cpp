//
// Created by frederik on 12/6/24.
//

#include "CheckpointReader.h"

#include "spdlog/spdlog.h"

#include <fstream>

namespace inputReader {
    void inputReader::CheckpointReader::readSimulation(ParticleContainer& container, const char* filename) {
        std::ifstream inputFile(filename, std::ios::binary);

        if (!inputFile.is_open()) {
            SPDLOG_ERROR("Could not open file {}", filename);
            std::exit(EXIT_FAILURE);
        }

        size_t num_types;
        std::vector<TypeDesc> ptypes;
        inputFile.read(reinterpret_cast<char*>(&num_types), sizeof(size_t));
        ptypes.resize(num_types);

        double m, s, e, dt, g;

        for (size_t i = 0; i < num_types; i++) {
            inputFile.read(reinterpret_cast<char*>(&m), sizeof(double));
            inputFile.read(reinterpret_cast<char*>(&s), sizeof(double));
            inputFile.read(reinterpret_cast<char*>(&e), sizeof(double));
            inputFile.read(reinterpret_cast<char*>(&dt), sizeof(double));
            inputFile.read(reinterpret_cast<char*>(&g), sizeof(double));

            ptypes[i] = TypeDesc(m, s, e, dt, g);
        }

        container.set_particle_type(ptypes);

        size_t num_particles = 0;

        inputFile.read(reinterpret_cast<char*>(&num_particles), sizeof(size_t));
        // size_t new_particles = container.size();
        container.resize(num_particles);
        SPDLOG_DEBUG("Reading num_particles from CheckPoint {}", num_particles);

        double x, y, z, vx, vy, vz, fx, fy, fz;
        int type;

        for (size_t i = 0; i < num_particles; i++) {
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