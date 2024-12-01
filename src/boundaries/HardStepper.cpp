#include "HardStepper.h"
#include "BoxContainer.h"
#include "utils/ArrayUtils.h"

#include <cmath>

void HardStepper::step(physicsCalculator::Calculator& calc) {
    // calculate new x
    calc.calculateX();
    // Update the cells to contain the correct arguments
    calc.get_container().update_positions();
    // update the old f
    calc.calculateOldF();
    // calculate new f
    calc.calculateF();

    const double scale = std::pow(2.0, 1.0 / 6.0);
    const std::array<double, 3> domain = calc.get_container().get_corner_vector();
    const double r = calc.get_env().get_sigma() * scale;

    // Reflect particles if required
    for (Particle& p : calc.get_container()) {

        if (p.getX()[0] < 0) {
            p.setX({
                -p.getX()[0],
                p.getX()[1],
                p.getX()[2],
            });

            p.setV({
                -p.getV()[0],
                p.getV()[1],
                p.getV()[2],
            });
        } else if (domain[0] - p.getX()[0] < r) {
            p.setX({
                domain[0] * 2 - p.getX()[0],
                p.getX()[1],
                p.getX()[2],
            });

            p.setV({
                -p.getV()[0],
                p.getV()[1],
                p.getV()[2],
            });
        }

        if (p.getX()[1] < 0) {
            p.setX({
                p.getX()[0],
                -p.getX()[1],
                p.getX()[2],
            });

            p.setV({
                p.getV()[0],
                -p.getV()[1],
                p.getV()[2],
            });
        } else if (domain[1] - p.getX()[1] < r) {
            p.setX({
                p.getX()[0],
                domain[1] * 2 - p.getX()[1],
                p.getX()[2],
            });

            p.setV({
                p.getV()[0],
                -p.getV()[1],
                p.getV()[2],
            });
        }

        if (p.getX()[2] < 0) {
            p.setX({
                p.getX()[0],
                p.getX()[1],
                -p.getX()[2],
            });

            p.setV({
                p.getV()[0],
                p.getV()[1],
                -p.getV()[2],
            });
        } else if (domain[2] - p.getX()[2] < r) {
            p.setX({
                p.getX()[0],
                p.getX()[1],
                domain[2] * 2 - p.getX()[2],
            });

            p.setV({
                p.getV()[0],
                p.getV()[1],
                -p.getV()[2],
            });
        }
    }

    // calculate new v
    calc.calculateV();
}
