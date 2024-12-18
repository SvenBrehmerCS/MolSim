/*
 * FileReader.cpp
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */


#include "FileReader.h"

namespace inputReader {

    FileReader::FileReader(const char* filename) {
        // Check for valid input file and open it.
        input_file = std::ifstream(filename);
        if (!input_file.is_open()) {
            spdlog::error("Could not open file {}", filename);
            std::exit(EXIT_FAILURE);
        }
    };

    FileReader::~FileReader() = default;

    void FileReader::readArguments(Environment& environment) { }

    void FileReader::readParticle(ParticleContainer& container) {

        std::array<double, 3> x;
        std::array<double, 3> v;
        std::array<int, 3> N;
        double m;
        double h;
        int num_particles = 0;
        int num_cubes = 0;
        int num_dimensions = 0;
        double brownian_motion = 0.1;

        std::string tmp_string;

        getline(input_file, tmp_string);
        spdlog::debug("Read line: {}", tmp_string);

        while (tmp_string.empty() or tmp_string[0] == '#') {
            getline(input_file, tmp_string);
            spdlog::debug("Read line: {}", tmp_string);
        }

        std::istringstream numstream(tmp_string);
        numstream >> num_particles;
        spdlog::debug("Reading {} particles.", num_particles);
        getline(input_file, tmp_string);
        spdlog::debug("Read line: {}", tmp_string);

        spdlog::debug("Resizing container");
        container.resize(num_particles);

        auto particle = container.begin();

        for (int i = 0; i < num_particles; i++) {
            std::istringstream datastream(tmp_string);

            for (auto& xj : x) {
                datastream >> xj;
            }
            for (auto& vj : v) {
                datastream >> vj;
            }
            if (datastream.eof()) {
                spdlog::critical("Error reading file: eof reached unexpectedly reading from line {}", i);
                std::exit(EXIT_FAILURE);
            }
            datastream >> m;

            particle->setX(x);
            particle->setV(v);
            particle->setM(m);

            getline(input_file, tmp_string);
            spdlog::debug("Read line: {}", tmp_string);

            particle++;
        }

        // if() clause to also support older text files
        if (!input_file.eof()) {
            spdlog::debug("Reading File with cuboid support");
            std::istringstream cubestream(tmp_string);
            cubestream >> num_cubes >> num_dimensions;
            spdlog::debug("Reading num_cubes {}, num_dimensions {}", num_cubes, num_dimensions);
            getline(input_file, tmp_string);
            spdlog::debug("Read line: {}", tmp_string);

            ParticleGenerator generator;
            for (int i = 0; i < num_cubes; i++) {
                std::istringstream datastream(tmp_string);

                for (auto& xj : x) {
                    datastream >> xj;
                }
                for (auto& vj : v) {
                    datastream >> vj;
                }
                datastream >> m;

                for (auto& nj : N) {
                    datastream >> nj;
                }
                if (datastream.eof()) {
                    spdlog::critical("Error reading file: eof reached unexpectedly reading from line {}", i);
                    std::exit(EXIT_FAILURE);
                }
                datastream >> h;


                if (!datastream.eof()) {
                    datastream >> brownian_motion;
                    spdlog::debug("Read brownian motion from input file: {}", brownian_motion);
                }

                container.resize(num_particles + (N[0] * N[1] * N[2]));

                generator.generateCuboid(container, num_particles, x, v, m, N, h, brownian_motion, num_dimensions);

                num_particles += (N[0] * N[1] * N[2]);

                getline(input_file, tmp_string);
                spdlog::debug("Read line: {}", tmp_string);
            }
        }
    }
} // namespace inputReader
