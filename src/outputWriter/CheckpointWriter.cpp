//
// Created by frederik on 12/6/24.
//

#include "CheckpointWriter.h"

#include "spdlog/fmt/bundled/xchar.h"
#include "spdlog/spdlog.h"

#include <fstream>
#include <iostream>

template <typename T> std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
    for (const auto& val : vec) {
        os << val << " ";
    }
    return os;
}


void outputWriter::CheckpointWriter::plot(ParticleContainer& container, Environment& env, const char* filename, size_t iteration_number) {
    std::ofstream outputFile(filename);

    if (!outputFile.is_open()) {
        spdlog::error("Error opening output file {}", filename);
        std::exit(EXIT_FAILURE);
    }
    outputFile << env.get_delta_t() << std::endl;
    outputFile << env.get_t_end() << std::endl;
    outputFile << env.get_sigma() << std::endl;
    outputFile << env.get_r_cutoff() << std::endl;
    outputFile << env.get_print_step() << std::endl;
    outputFile << env.get_output_file_name() << std::endl;
    outputFile << env.get_output_file_format() << std::endl;
    outputFile << env.get_input_file_name() << std::endl;
    outputFile << env.get_input_file_format() << std::endl;
    outputFile << env.get_epsilon() << std::endl;
    outputFile << env.get_domain_size()[0] << " " << env.get_domain_size()[1] << " " << env.get_domain_size()[2] << std::endl;
    outputFile << env.get_calculator_type() << std::endl;
    outputFile << env.get_boundary_type()[0] << " " << env.get_boundary_type()[1] << " " << env.get_boundary_type()[2] << " "
               << env.get_boundary_type()[3] << " " << env.get_boundary_type()[4] << " " << env.get_boundary_type()[5] << std::endl;
    outputFile << iteration_number << std::endl;
    outputFile << container.size() << " " << std::endl;
    spdlog::info("container size: {}", container.size());

    //TODO evtl. noch dimension einlesen

    auto particle = container.begin();

    for (size_t i = 0; i < container.size(); i++) {
        outputFile << particle->getX()[0] << " " << particle->getX()[1] << " " << particle->getX()[2] << " " << particle->getV()[0] << " "
                   << particle->getV()[1] << " " << particle->getV()[2] << " " << particle->getType() << " " << particle->getF()[0] << " "
                   << particle->getF()[1] << " " << particle->getF()[2] << " " << particle->getM() << std::endl;
        ++particle;
    }
}
