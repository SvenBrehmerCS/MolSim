/*
 * FileReader.cpp
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */


#include "FileReader.h"
#include "../Particle.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ParticleGenerator.h>
#include <sstream>
#include <spdlog/spdlog.h>

namespace inputReader {

    FileReader::FileReader() = default;

    FileReader::~FileReader() = default;

    void FileReader::readFile(std::vector<Particle>& particles, const char* filename) {
        std::array<double, 3> x;
        std::array<double, 3> v;
        std::array<int, 3> N;
        double m;
        double h;
        int num_particles = 0;
        int num_cubes = 0;
        int num_dimensions = 0;
        int brownian_motion = 0.1;

        std::ifstream input_file(filename);
        std::string tmp_string;

        if (input_file.is_open()) {

            getline(input_file, tmp_string);
            spdlog::debug("Read line: {}", tmp_string);

            while (tmp_string.empty() or tmp_string[0] == '#') {
                getline(input_file, tmp_string);
                spdlog::debug("Read line: {}", tmp_string);
            }

            std::istringstream numstream(tmp_string);
            numstream >> num_particles;
            spdlog::debug("Reading {}.", num_particles);
            getline(input_file, tmp_string);
            spdlog::info("Read line: {}", tmp_string);

            particles.resize(num_particles);

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
                    exit(-1);
                }
                datastream >> m;

                particles[i].setX(x);
                particles[i].setV(v);
                particles[i].setM(m);

                getline(input_file, tmp_string);
                spdlog::debug("Read line: {}", tmp_string);
            }

            std::istringstream cubestream(tmp_string);
            numstream >> num_cubes >> num_dimensions;
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
                    exit(-1);
                }
                datastream >> h;

                particles.resize(num_particles + (N[0] * N[1] * N[2]));

                generator.generateCuboid(particles, num_particles, x, v, m, N, h, brownian_motion, num_dimensions);

                num_particles += (N[0] * N[1] * N[2]);

                getline(input_file, tmp_string);
                spdlog::debug("Read line: {}", tmp_string);
            }

            particles.shrink_to_fit();
        } else {
            spdlog::critical("Error: could not open file {}", filename);
            exit(-1);
        }
    }
} // namespace inputReader
