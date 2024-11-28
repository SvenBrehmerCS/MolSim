#pragma once

#include "Stepper.h"

class HaloStepper : public Stepper {
private:
public:
    HaloStepper() = default;

    ~HaloStepper() = default;

    virtual void step(physicsCalculator::Calculator& calc);
};
