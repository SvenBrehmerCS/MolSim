//
// Created by frederik on 12/6/24.
//

#include "CheckpointReader.h"

#include "spdlog/spdlog.h"

#include <fstream>


void inputReader::CheckpointReader::readSimulation(ParticleContainer& container, Environment& env, const char* filename) {
    std::ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        spdlog::error("Could not open file {}", filename);
        std::exit(EXIT_FAILURE);
    }
    double delta_t, t_end, sigma, r_cutoff, epsilon, domain_size0, domain_size1, domain_size2;
    int print_step;
    size_t num_particles;
    std::string output_file_name, output_file_format, input_file_name, input_file_format, calculator_type;
    std::string boundary_type_yz_near, boundary_type_xz_near, boundary_type_xy_near, boundary_type_yz_far, boundary_type_xz_far, boundary_type_xy_far;

    inputFile >> delta_t;
    env.set_delta_t(delta_t);

    inputFile >> t_end;
    env.set_t_end(t_end);

    inputFile >> sigma;
    env.set_sigma(sigma);

    inputFile >> r_cutoff;
    env.set_r_cutoff(r_cutoff);

    inputFile >> print_step;
    env.set_print_step(print_step);

    inputFile >> output_file_name;
    env.set_output_file_name(output_file_name);

    inputFile >> output_file_format;
    if (output_file_format == "XYZ") {
        env.set_output_file_format(XYZ);
    } else if (output_file_format == "VTK") {
        env.set_output_file_format(VTK);
    } else {
        env.set_output_file_format(NO_OUT);
    }
    // TODO braucht man input_file_name und format ?
    inputFile >> input_file_name;
    inputFile >> input_file_format;

    inputFile >> epsilon;
    env.set_epsilon(epsilon);

    inputFile >> domain_size0 >> domain_size1 >> domain_size2;
    env.set_domain_size({ domain_size0, domain_size1, domain_size2 });

    inputFile >> calculator_type;
    if (calculator_type == "GRAVITY") {
        env.set_calculator_type(GRAVITY);
    } else {
        env.set_calculator_type(LJ_FULL);
    }

    inputFile >> boundary_type_yz_near >> boundary_type_xz_near >> boundary_type_xy_near >> boundary_type_yz_far >> boundary_type_xz_far
        >> boundary_type_xy_far;

    env.set_boundary_type({
        getBoundaryType(boundary_type_yz_near),
        getBoundaryType(boundary_type_xz_near),
        getBoundaryType(boundary_type_xy_near),
        getBoundaryType(boundary_type_yz_far),
        getBoundaryType(boundary_type_xz_far),
        getBoundaryType(boundary_type_xy_far),
    });

    inputFile >> num_particles;
    container.resize(num_particles);

    double x, y, z, vx, vy, vz, fx, fy, fz, m;
    int type;

    for (size_t i = 0; i < num_particles; i++) {
        inputFile >> x >> y >> z >> vx >> vy >> vz >> type >> fx >> fy >> fz >> m;

        container[i].setX({ x, y, z });
        container[i].setV({ vx, vy, vz });
        container[i].setType(type);
        container[i].setF({ fx, fy, fz });
        container[i].setM(m);
    }
}
