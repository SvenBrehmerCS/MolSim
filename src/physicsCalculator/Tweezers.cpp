#include "Tweezers.h"

namespace physicsCalculator {
    Tweezers::Tweezers(const std::vector<size_t>& i, const Vec<double>& f, const double e) {
        indices = i;
        force = f;
        end = e;
    }

    void Tweezers::apply(ParticleContainer& particles) {
        for (const size_t i : indices) {
            particles[i].setF(particles[i].getF() + force);
        }
    }
} // namespace physicsCalculator
