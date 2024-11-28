#include "HaloStepper.h"
#include "BoxContainer.h"
#include "utils/ArrayUtils.h"

#include <cmath>

void HaloStepper::step(physicsCalculator::Calculator& calc) {
    BoxContainer& container = dynamic_cast<BoxContainer&>(calc.get_container());

    // calculate new x
    calc.calculateX();
    // Update the cells to contain the correct arguments
    container.update_positions();
    // update the old f
    calc.calculateOldF();
    // calculate new f
    calc.calculateF();
    // Create halo particles if required
    container.iterate_boundary_cells([&calc, &container](Particle& p) {
        const double scale = std::pow(2.0, 1.0 / 6.0);

        std::array<double, 3> f = { 0.0, 0.0, 0.0 };

        std::array<double, 3> c1 = {
            container.getRC(),
            container.getRC(),
            container.getRC(),
        };

        std::array<double, 3> c2 = container.get_corner_vector();

        const double r = calc.get_env().get_sigma() * scale;

        if (p.getX()[0] - c1[0] < r) {
            f[0] = calc.calculateFDist(p.getX()[0] - c1[0]);
        } else if (c2[0] - p.getX()[0] < r) {
            f[0] = calc.calculateFDist(p.getX()[0] - c2[0]);
        }

        if (p.getX()[1] - c1[1] < r) {
            f[1] = calc.calculateFDist(p.getX()[1] - c1[1]);
        } else if (c2[1] - p.getX()[1] < r) {
            f[1] = calc.calculateFDist(p.getX()[1] - c2[1]);
        }

        if (p.getX()[2] - c1[2] < r) {
            f[2] = calc.calculateFDist(p.getX()[2] - c1[2]);
        } else if (c2[2] - p.getX()[2] < r) {
            f[2] = calc.calculateFDist(p.getX()[2] - c2[2]);
        }

        p.setF(p.getF() + f);
    });

    // calculate new v
    calc.calculateV();
}
