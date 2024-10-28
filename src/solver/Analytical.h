/**
 * @file
 *
 * Describe analytical solutions for multiple problems.
 */
#include "../Particle.h"

/**
 * @namespace
 *
 * This namespace contains multiple analytical solutions to different single, two and three body problems.
 */
namespace solver {
    /**
     * Test if two particles correspond to the center dance problem. This problem is a problem with a simple
     * periodical analytical solution, therefore it is well suited for testing. The corresponding problem
     * state is given in input/balanced-dance.txt
     *
     * @param p1 The first particle.
     * @param p2 The second particle.
     * @param t The current simulation time.
     *
     * @return A boolean indicating if the points are close to the analytical solution.
     */
    bool is_balanced_dance_solution(const Particle& p1, const Particle& p2, const double t);

    /**
     * Test if three particles correspond to a balanced center dance problem. This problem is a problem with a
     * simple periodical analytical solution, therefore it is well suited for testing. The corresponding problem
     * state is given in input/balanced-center.txt
     *
     * @param p1 The first particle.
     * @param p2 The second particle.
     * @param center The center particle.
     * @param t The current simulation time.
     *
     * @return A boolean indicating if the points are close to the analytical solution.
     */
    bool is_center_dance_solution(const Particle& p1, const Particle& p2, const Particle& center, const double t);

    /**
     * Test if a particles correspond to a linear evasion problem. This problem is a problem with a simple
     * linear analytical solution, therefore it is well suited for testing.  The corresponding problem state
     * is given in input/evading.txt
     *
     * @param p The particle.
     * @param t The current simulation time.
     *
     * @return A boolean indicating if the point is close to the analytical solution.
     */
    bool is_evasion_solution(const Particle& p, const double t);

    /**
     * Test if two particles correspond to the center evasion problem. This problem is a problem with a
     * periodical and linear analytical solution, therefore it is well suited for testing. The corresponding
     * problem state is given in input/center-evasion.txt
     *
     * @param p1 The first particle.
     * @param p2 The second particle.
     * @param t The current simulation time.
     *
     * @return A boolean indicating if the points are close to the analytical solution.
     */
    bool is_center_evasion_solution(const Particle& p1, const Particle& p2, const double t);

} // namespace solver
