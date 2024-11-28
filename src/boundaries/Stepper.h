#pragma once

#include "physicsCalculator/Calculator.h"

class Stepper {
public:
    Stepper() = default;

    Stepper(const Stepper& stepper) = delete;

    ~Stepper() = default;

    virtual void step(physicsCalculator::Calculator& calc) = 0;
};
