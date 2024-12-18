//
// Created by frederik on 12/6/24.
//

#include "CheckpointWriter.h"

#include "spdlog/fmt/bundled/xchar.h"
#include "spdlog/spdlog.h"

#include <fstream>
#include <iostream>
namespace outputWriter {
    void CheckpointWriter::plot(const ParticleContainer& container, const Environment& env, const char* filename) {
        std::ofstream outputFile(filename, std::ios::binary);

        if (!outputFile.is_open()) {
            SPDLOG_ERROR("Error opening output file {}", filename);
            std::exit(EXIT_FAILURE);
        }
        size_t num_types = container.get_types().size();
        outputFile.write((char*)&num_types, sizeof(size_t));

        auto types = container.get_types();
        double delta_t = env.get_delta_t();
        double g = env.get_gravity();
        double mass, sigma, epsilon;
        for (auto type : types) {
            mass = type.get_mass();
            sigma = type.get_sigma();
            epsilon = type.get_epsilon();

            outputFile.write((char*)&mass, sizeof(size_t));
            outputFile.write((char*)&sigma, sizeof(double));
            outputFile.write((char*)&epsilon, sizeof(double));
            outputFile.write((char*)&delta_t, sizeof(double));
            outputFile.write((char*)&g, sizeof(double));
        }

        size_t numParticles = container.size();
        outputFile.write((char*)&numParticles, sizeof(size_t));
        // outputFile << container.size();
        SPDLOG_INFO("container size: {}", container.size());

        // TODO evtl. noch dimension einlesen

        auto particle = container.begin();

        for (size_t i = 0; i < container.size(); i++) {
            outputFile.write((char*)&particle->getX()[0], sizeof(double));
            outputFile.write((char*)&particle->getX()[1], sizeof(double));
            outputFile.write((char*)&particle->getX()[2], sizeof(double));
            outputFile.write((char*)&particle->getV()[0], sizeof(double));
            outputFile.write((char*)&particle->getV()[1], sizeof(double));
            outputFile.write((char*)&particle->getV()[2], sizeof(double));
            int particle_type = particle->getType();
            outputFile.write((char*)&particle_type, sizeof(int));
            outputFile.write((char*)&particle->getF()[0], sizeof(double));
            outputFile.write((char*)&particle->getF()[1], sizeof(double));
            outputFile.write((char*)&particle->getF()[2], sizeof(double));

            ++particle;
        }
    }
    void CheckpointWriter::plotParticles(const ParticleContainer& container, const std::string& filename, const int iteration) { return; }
}