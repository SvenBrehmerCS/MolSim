#include "SimpleCalculator.h"

namespace physicsCalculator {
    SimpleCalculator::SimpleCalculator() = default;

    SimpleCalculator::~SimpleCalculator() = default;

    void SimpleCalculator::calculateF(ParticleContainer& container, const Environment& env) {
        // Set the old f to f and reset the current forces
        for (Particle& p : container.get_particles()) {
            p.setOldF(p.getF());
            p.setF({ 0.0, 0.0, 0.0 });
        }

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
    }

    void SimpleCalculator::calculateX(ParticleContainer& container, const Environment& env) {
        for (Particle& p : container.get_particles()) {
            p.setX(p.getX() + env.get_delta_t() * p.getV() + (env.get_delta_t() * env.get_delta_t() * 0.5 / p.getM()) * p.getF());
        }
    }

    void SimpleCalculator::calculateV(ParticleContainer& container, const Environment& env) {
        for (Particle& p : container.get_particles()) {
            p.setV(p.getV() + (env.get_delta_t() * 0.5 / p.getM()) * (p.getOldF() + p.getF()));
        }
    }
} // namespace physicsCalculator