#include "Analytical.h"
#include "utils/ArrayUtils.h"

#include <cmath>

static constexpr double error_margin = 0.001;

bool solver::is_balanced_dance_solution(const Particle& p1, const Particle& p2, const double t) {
    // This method assumes that the particles are stored into the vector with the same order as they are provided
    const std::array<double, 3> x1 = { std::cos(t), std::sin(t), 0.0 };
    const std::array<double, 3> x2 = { -std::cos(t), -std::sin(t), 0.0 };

    if (ArrayUtils::L2Norm(p1.getX() - x1) < error_margin && ArrayUtils::L2Norm(p2.getX() - x2) < error_margin) {
        return true;
    }

    return false;
}

bool solver::is_center_dance_solution(const Particle& p1, const Particle& p2, const Particle& center, const double t) {
    // This method assumes that the particles are stored into the vector with the same order as they are provided
    const std::array<double, 3> x1 = { std::cos(t), std::sin(t), 0.0 };
    const std::array<double, 3> x2 = { -std::cos(t), -std::sin(t), 0.0 };
    const std::array<double, 3> x_center = { 0.0, 0.0, 0.0 };

    if (ArrayUtils::L2Norm(p1.getX() - x1) < error_margin && ArrayUtils::L2Norm(p2.getX() - x2) < error_margin
        && ArrayUtils::L2Norm(center.getX() - x_center) < error_margin) {
        return true;
    }

    return false;
}

bool solver::is_evasion_solution(const Particle& p, const double t) {
    const std::array<double, 3> x = { t, 2.0 * t, -1.0 * t };

    if (ArrayUtils::L2Norm(p.getX() - x) < error_margin) {
        return true;
    }

    return false;
}

bool solver::is_center_evasion_solution(const Particle& p1, const Particle& p2, const double t) {
    // This method assumes that the particles are stored into the vector with the same order as they are provided
    const std::array<double, 3> x1 = { std::cos(t) + 0.1 * t, std::sin(t), 0.0 };
    const std::array<double, 3> x2 = { -2.0 * std::cos(t) + 0.1 * t, -2.0 * std::sin(t), 0.0 };

    if (ArrayUtils::L2Norm(p1.getX() - x1) < error_margin && ArrayUtils::L2Norm(p2.getX() - x2) < error_margin) {
        return true;
    }

    return false;
}
