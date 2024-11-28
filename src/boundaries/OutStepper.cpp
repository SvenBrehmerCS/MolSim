#include "OutStepper.h"
#include "BoxContainer.h"
#include "utils/ArrayUtils.h"

void OutStepper::step(physicsCalculator::Calculator& calc) {
    BoxContainer& container = dynamic_cast<BoxContainer&>(calc.get_container());

    // calculate new x
    calc.calculateX();
    // Update the cells to contain the correct arguments
    container.update_positions();
    container.clear_halo_cells();
    // update the old f
    calc.calculateOldF();
    // calculate new f
    calc.calculateF();
    // calculate new v
    calc.calculateV();
}
