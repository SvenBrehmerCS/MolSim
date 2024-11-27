#include "Calculator.h"

#include "inputReader/FileReader.h"
#include "inputReader/Reader.h"

#include "utils/ArrayUtils.h"

#include <memory>
#include <spdlog/spdlog.h>

/**
 * @brief Collection of calculators for different levels of complexity
 */
namespace physicsCalculator {

    Calculator::Calculator() = default;

    Calculator::Calculator(const Environment& new_env) {
        env = new_env;

        // Read the input file
        std::unique_ptr<inputReader::Reader> fileReader { new inputReader::FileReader() };
        fileReader->readFile(container, env.get_input_file_name());
    }

    Calculator::Calculator(const Environment& new_env, const std::vector<Particle>& particles) {
        env = new_env;
        container = ParticleContainer(particles);
    }

    void Calculator::step() {
        spdlog::debug("Started a simulation step.");

        // calculate new x
        calculateX();
        // update the old f
        calculateOldF();
        // calculate new f
        calculateF();
        // calculate new v
        calculateV();
    }

    ParticleContainer& Calculator::get_container() { return container; }

    void Calculator::calculateOldF() {
        for (Particle& p : container) {
            p.setOldF(p.getF());
            p.setF({ 0.0, 0.0, 0.0 });
        }

        spdlog::debug("Updated the old force.");
    }

    void Calculator::calculateX() {
        for (Particle& p : container) {
            p.setX(p.getX() + env.get_delta_t() * p.getV() + (env.get_delta_t() * env.get_delta_t() * 0.5 / p.getM()) * p.getF());
        }

        spdlog::debug("Updated the positions.");
    }

    void Calculator::calculateF() {
        container.iterate_pairs([this](Particle& i, Particle& j) {
            const double force = this->calculateFAbs(i, j);

            // Update the forces for both particles
            i.setF(force * (j.getX() - i.getX()) + i.getF());
            j.setF(force * (i.getX() - j.getX()) + j.getF());
        });

        spdlog::debug("Calculated the new force.");
    }

    void Calculator::calculateV() {
        for (Particle& p : container) {
            p.setV(p.getV() + (env.get_delta_t() * 0.5 / p.getM()) * (p.getOldF() + p.getF()));
        }

        spdlog::debug("Updated the velocities.");
    }

}
