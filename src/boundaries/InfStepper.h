#pragma once

#include "Stepper.h"

class InfStepper : public Stepper {
private:
public:
    InfStepper() = default;

    ~InfStepper() = default;

    virtual void step(physicsCalculator::Calculator& calc);
};
