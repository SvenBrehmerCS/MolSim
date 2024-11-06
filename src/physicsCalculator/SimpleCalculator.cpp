#include "SimpleCalculator.h"

#include <spdlog/spdlog.h>

namespace physicsCalculator {
    SimpleCalculator::SimpleCalculator(const Environment& new_env)
        : Calculator(new_env) {

        // Initialize the forces
        calculateF();
    }

    SimpleCalculator::SimpleCalculator(const Environment& new_env, const std::vector<Particle>& particles) {
        spdlog::warn("Called a SimpleCalculator constructor wich should only be used for testing.");
        env = new_env;
        container = ParticleContainer(particles);

        // Initialize the forces
        calculateF();
    }

    SimpleCalculator::~SimpleCalculator() = default;

    void SimpleCalculator::calculateF() {
        // Set the old f to f and reset the current forces
        for (Particle& p : container.get_particles()) {
            p.setOldF(p.getF());
            p.setF({ 0.0, 0.0, 0.0 });
        }

        spdlog::debug("Updated the old force.");

        for (size_t i = 0; i < container.get_particles().size(); i++) {
            for (size_t j = i + 1; j < container.get_particles().size(); j++) {
                // Calculate the distance and force experienced by two particles
                const double distance = ArrayUtils::L2Norm(container.get_particles()[j].getX() - container.get_particles()[i].getX());
                const double force = container.get_particles()[i].getM() * container.get_particles()[j].getM() / (distance * distance * distance);

                // Update the forces for both particles
                container.get_particles()[i].setF(
                    force * (container.get_particles()[j].getX() - container.get_particles()[i].getX()) + container.get_particles()[i].getF());
                container.get_particles()[j].setF(
                    force * (container.get_particles()[i].getX() - container.get_particles()[j].getX()) + container.get_particles()[j].getF());
            }
        }

        spdlog::debug("Calculated the new force.");
    }

    void SimpleCalculator::calculateX() {
        for (Particle& p : container.get_particles()) {
            p.setX(p.getX() + env.get_delta_t() * p.getV() + (env.get_delta_t() * env.get_delta_t() * 0.5 / p.getM()) * p.getF());
        }

        spdlog::debug("Updated the positions.");
    }

    void SimpleCalculator::calculateV() {
        for (Particle& p : container.get_particles()) {
            p.setV(p.getV() + (env.get_delta_t() * 0.5 / p.getM()) * (p.getOldF() + p.getF()));
        }

        spdlog::debug("Updated the velocities.");
    }
} // namespace physicsCalculator