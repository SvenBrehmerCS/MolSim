#include "HaloStepper.h"
#include "BoxContainer.h"
#include "utils/ArrayUtils.h"

void HaloStepper::step(physicsCalculator::Calculator& calc) {
    // calculate new x
    calc.calculateX();

    // TODO: Update the containers

    // update the old f
    calc.calculateOldF();
    // calculate new f
    calc.calculateF();
    // Create halo particles if required
    BoxContainer& container = dynamic_cast<BoxContainer&>(calc.get_container());

    container.iterate_boundary_cells([&calc](Particle& p) {
        std::array<double, 3> f = { 0.0, 0.0, 0.0 };

        if (p.getX()[0] <= calc.get_env().get_sigma()) {
            f[0] += calc.calculateFDist(p.getX()[0]);
        }

        if (p.getX()[1] <= calc.get_env().get_sigma()) {
            f[1] += calc.calculateFDist(p.getX()[1]);
        }

        if (p.getX()[2] <= calc.get_env().get_sigma()) {
            f[2] += calc.calculateFDist(p.getX()[2]);
        }

        // TODO: Halo other site

        p.setF(p.getF() + f);
    });

    // calculate new v
    calc.calculateV();
}
