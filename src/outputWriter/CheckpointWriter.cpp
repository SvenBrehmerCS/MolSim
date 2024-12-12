//
// Created by frederik on 12/6/24.
//

#include "CheckpointWriter.h"

#include "spdlog/fmt/bundled/xchar.h"
#include "spdlog/spdlog.h"

#include <fstream>
#include <iostream>
namespace outputWriter {
    ;
    void CheckpointWriter::plot(ParticleContainer& container, Environment& env, const char* filename) {
        std::ofstream outputFile(filename, std::ios::binary);

        if (!outputFile.is_open()) {
            spdlog::error("Error opening output file {}", filename);
            std::exit(EXIT_FAILURE);
        }
        size_t numParticles = container.size();
        outputFile.write((char*)&numParticles, sizeof(size_t));
        // outputFile << container.size();
        spdlog::info("container size: {}", container.size());

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
            double m = particle->getM();
            outputFile.write((char*)&m, sizeof(double));

            ++particle;
        }
    }
    void CheckpointWriter::plotParticles(ParticleContainer& container, const std::string& filename, int iteration) { return; }
}