/**
 * @file
 *
 * @brief Handles the gravity based physics calculations
 */

#pragma once

#include "Calculator.h"
#include "Environment.h"
#include "container/ParticleContainer.h"
#include "utils/ArrayUtils.h"

namespace physicsCalculator {

    /**
     * @class GravityCalculator
     *
     * @brief Class corresponding to a leap frog based gravity force calculator.
     */
    class GravityCalculator : public Calculator {

    public:
        /**
         * Initialize a simple calculator using a simulation environment.
         *
         * @param new_env The simulation environment that should be used for initialization.
         * @param new_cont The container storing the particles that should be used throughout the simulation.
         */
        GravityCalculator(const Environment& new_env, const std::shared_ptr<ParticleContainer>& new_cont);

        /**
         * Provide a constructor that allows the construction of a calculator using a particle container
         * and simulation environment. This class should be used for testing.
         *
         * @param new_env The new simulation environment.
         * @param particles The vector storing the particles that should be used throughout the simulation.
         * @param init_forces Define wether the forces should be initialized.
         */
        GravityCalculator(
            const Environment& new_env, const std::vector<Particle>& particles, const bool init_forces = true, const BoundaryType type = INF_CONT);

        /**
         * Define a constructor for a gravity calculator.
         */
        virtual ~GravityCalculator();

        /**
         * Get the force absolute and sign direction between two particles.
         * 
         * @param p1 The first particle.
         * @param p2 The second particle.
         * @param dist THe distance between the particles.
         */
        virtual double calculateFAbs(const Particle& p1, const Particle& p2, const double dist_squ);

        /**
         * Get the force absolute and sign direction between two particles.
         *
         * @param dist The distance between two particles.
         * @param t1 The type of the first particle.
         * @param t2 The type of the second particle.
         */
        virtual double calculateFDist(const double dist, const int t1, const int t2) const;
    };
} // namespace physicsCalculator