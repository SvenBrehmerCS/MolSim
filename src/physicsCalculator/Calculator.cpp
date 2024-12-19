#include "Calculator.h"
#include "container/DSContainer.h"
#include "inputReader/FileReader.h"
#include "inputReader/Reader.h"
#include "utils/Vec.h"

#include <memory>
#include <spdlog/spdlog.h>

namespace physicsCalculator {

    Calculator::Calculator() = default;

    Calculator::Calculator(const Environment& new_env, const std::shared_ptr<ParticleContainer>& new_cont)
        : env { new_env }
        , cont { new_cont } { }

    Calculator::Calculator(const Environment& new_env, const std::vector<Particle>& particles, const std::vector<TypeDesc>& new_desc) {
        env = new_env;
        cont = std::make_shared<DSContainer>(particles, new_desc);
    }

    ParticleContainer& Calculator::get_container() { return *cont; }

    const Environment& Calculator::get_env() const { return env; }

    void Calculator::calculateOldF() {
        for (Particle& p : *cont) {
            p.setOldF(p.getF());
            p.setF(cont->get_type_descriptor(p.getType()).get_G());
        }

        SPDLOG_DEBUG("Updated the old force.");
    }

    void Calculator::calculateX() {
        for (Particle& p : *cont) {
            p.setX(p.getX() + env.get_delta_t() * p.getV() + cont->get_type_descriptor(p.getType()).get_dt_dt_m() * p.getF());
        }

        SPDLOG_DEBUG("Updated the positions.");
    }

    void Calculator::calculateF() {
        cont->iterate_pairs([this](Particle& i, Particle& j) {
            const double dist = (i.getX() - j.getX()).len();
            const double force = this->calculateFAbs(i, j, dist);

            // Update the forces for both particles
            i.setF(force * (j.getX() - i.getX()) + i.getF());
            j.setF(force * (i.getX() - j.getX()) + j.getF());
        });

        SPDLOG_DEBUG("Calculated the new force.");
    }

    void Calculator::calculateV() {
        for (Particle& p : *cont) {
            p.setV(p.getV() + cont->get_type_descriptor(p.getType()).get_dt_m() * (p.getOldF() + p.getF()));
        }

        SPDLOG_DEBUG("Updated the velocities.");
    }
}
