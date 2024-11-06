#include "Calculator.h"

#include "../inputReader/FileReader.h"
#include "../inputReader/Reader.h"

#include <memory>

/**
 * @brief Collection of calculators for different levels of complexity
 */
namespace physicsCalculator {

    Calculator::Calculator() = default;

    Calculator::Calculator(const Environment& new_env) {
        env = new_env;

        // Read the input file
        std::unique_ptr<inputReader::Reader> fileReader { new inputReader::FileReader() };
        fileReader->readFile(container.get_particles(), env.get_input_file_name());
    }

    void Calculator::step() {
        // calculate new x
        calculateX();
        // calculate new f
        calculateF();
        // calculate new v
        calculateV();
    }

    ParticleContainer& Calculator::get_container() { return container; }

    std::vector<Particle>& Calculator::get_particles() { return container.get_particles(); }

}