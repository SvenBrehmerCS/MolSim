#include "InfStepper.h"

void InfStepper::step(physicsCalculator::Calculator& calc) {
    // calculate new x
    calc.calculateX();
    // update the old f
    calc.calculateOldF();
    // calculate new f
    calc.calculateF();
    // calculate new v
    calc.calculateV();
}
