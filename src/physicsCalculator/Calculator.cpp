#include "Calculator.h"

#include "boundaries/InfContainer.h"

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

    Calculator::Calculator(const Environment& new_env, const std::shared_ptr<ParticleContainer>& new_cont)
        : env { new_env }
        , cont { new_cont } { }

    Calculator::Calculator(const Environment& new_env, const std::vector<Particle>& particles) {
        env = new_env;
        cont.reset(new InfContainer(particles));
    }

    ParticleContainer& Calculator::get_container() { return *cont; }

    const Environment& Calculator::get_env() const { return env; }

    void Calculator::calculateOldF() {
        for (Particle& p : *cont) {
            p.setOldF(p.getF());
            p.setF({ 0.0, 0.0, 0.0 });
        }

        spdlog::debug("Updated the old force.");
    }

    void Calculator::calculateX() {
        for (Particle& p : *cont) {
            p.setX(p.getX() + env.get_delta_t() * p.getV() + (env.get_delta_t() * env.get_delta_t() * 0.5 / p.getM()) * p.getF());
        }

        spdlog::debug("Updated the positions.");
    }

    void Calculator::calculateF() {
        cont->iterate_pairs([this](Particle& i, Particle& j) {
            const double force = this->calculateFAbs(i, j);

            // Update the forces for both particles
            i.setF(force * (j.getX() - i.getX()) + i.getF());
            j.setF(force * (i.getX() - j.getX()) + j.getF());
        });

        spdlog::debug("Calculated the new force.");
    }

    void Calculator::calculateV() {
        for (Particle& p : *cont) {
            p.setV(p.getV() + (env.get_delta_t() * 0.5 / p.getM()) * (p.getOldF() + p.getF()));
        }

        spdlog::debug("Updated the velocities.");
    }

}
