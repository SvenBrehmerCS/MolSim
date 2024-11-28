#pragma once

#include "Stepper.h"

class OutStepper : public Stepper {
private:
public:
    OutStepper() = default;

    ~OutStepper() = default;

    virtual void step(physicsCalculator::Calculator& calc);
};
