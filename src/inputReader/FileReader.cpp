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
            SPDLOG_ERROR("Could not open file {}", filename);
            std::exit(EXIT_FAILURE);
        }
    };

    FileReader::~FileReader() = default;

    void FileReader::readArguments(Environment& environment, Thermostat& thermostat) { }

    void FileReader::readParticle(ParticleContainer& container, const double delta_t, const double gravity) {

        Vec<double> x;
        Vec<double> v;
        std::array<int, 3> N;
        double m;
        double h;
        int num_particles = 0;
        int num_cubes = 0;
        int num_dimensions = 0;
        double brownian_motion = 0.1;

        std::string tmp_string;

        getline(input_file, tmp_string);
        SPDLOG_DEBUG("Read line: {}", tmp_string);

        while (tmp_string.empty() or tmp_string[0] == '#') {
            getline(input_file, tmp_string);
            SPDLOG_DEBUG("Read line: {}", tmp_string);
        }

        std::istringstream numstream(tmp_string);
        numstream >> num_particles;
        SPDLOG_DEBUG("Reading {} particles.", num_particles);
        getline(input_file, tmp_string);
        SPDLOG_DEBUG("Read line: {}", tmp_string);

        SPDLOG_DEBUG("Resizing container");
        container.resize(num_particles);

        auto particle = container.begin();
        std::vector<TypeDesc> ptypes;
        int ptype = 0;

        for (int i = 0; i < num_particles; i++) {
            std::istringstream datastream(tmp_string);

            datastream >> x[0] >> x[1] >> x[2];
            datastream >> v[0] >> v[1] >> v[2];

            if (datastream.eof()) {
                SPDLOG_CRITICAL("Error reading file: eof reached unexpectedly reading from line {}", i);
                std::exit(EXIT_FAILURE);
            }
            datastream >> m;

            particle->setX(x);
            particle->setV(v);
            ptypes.push_back(TypeDesc { m, 1.0, 5.0, delta_t, gravity });
            particle->setType(ptype++);

            particle++;

            getline(input_file, tmp_string);
            SPDLOG_DEBUG("Read line: {}", tmp_string);
        }

        // if() clause to also support older text files
        if (!input_file.eof()) {
            SPDLOG_DEBUG("Reading File with cuboid support");
            std::istringstream cubestream(tmp_string);
            cubestream >> num_cubes >> num_dimensions;
            SPDLOG_DEBUG("Reading num_cubes {}, num_dimensions {}", num_cubes, num_dimensions);
            getline(input_file, tmp_string);
            SPDLOG_DEBUG("Read line: {}", tmp_string);

            ParticleGenerator generator;

            for (int i = 0; i < num_cubes; i++) {
                std::istringstream datastream(tmp_string);

                datastream >> x[0] >> x[1] >> x[2];
                datastream >> v[0] >> v[1] >> v[2];

                datastream >> m;

                for (auto& nj : N) {
                    datastream >> nj;
                }
                if (datastream.eof()) {
                    SPDLOG_CRITICAL("Error reading file: eof reached unexpectedly reading from line {}", i);
                    std::exit(EXIT_FAILURE);
                }
                datastream >> h;


                if (!datastream.eof()) {
                    datastream >> brownian_motion;
                    SPDLOG_DEBUG("Read brownian motion from input file: {}", brownian_motion);
                }

                container.resize(num_particles + (N[0] * N[1] * N[2]));

                generator.generateCuboid(container, num_particles, x, v, ptype, N, h, brownian_motion, num_dimensions);

                ptypes.push_back(TypeDesc { m, 1.0, 5.0, delta_t, gravity });
                particle->setType(ptype++);

                num_particles += (N[0] * N[1] * N[2]);

                getline(input_file, tmp_string);
                SPDLOG_DEBUG("Read line: {}", tmp_string);
            }
        }

        container.build_type_table(ptypes);
    }
} // namespace inputReader
