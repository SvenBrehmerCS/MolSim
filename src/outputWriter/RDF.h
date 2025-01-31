/**
 * @file
 *
 * @brief This file stores the utility required for generating radial distribution functions.
 */

#pragma once

#include "Environment.h"
#include "Writer.h"
#include "container/ParticleContainer.h"

#include <fstream>
#include <string>
#include <vector>

namespace outputWriter {
    /**
     * @class RDF
     *
     * @brief Define the RDF writer, which is used for computing and writing the radial distribution.
     */
    class RDF : public Writer {
    private:
        /**
         * The number of buckets.
         */
        size_t buckets;

        /**
         * The width of each bucket.
         */
        double delta_r;

        /**
         * The boundary types of the simulation.
         */
        std::array<BoundaryType, 6> bt;

        /**
         *  The size of the simulation domain.
         */
        Vec<double> domain;

    public:
        /**
         * Constructor for the RDF class.
         *
         * @param delta_r The width of each bucket.
         * @param buckets The number of buckets.
         * @param bt The type of the boundaries.
         * @param domain The domain size.
         */
        RDF(const double delta_r, const size_t buckets, const std::array<BoundaryType, 6>& bt, const Vec<double>& domain);

        /**
         * @brief Destructor for the RDF class.
         */
        ~RDF() = default;

        /**
         * Generates a radial distribution function.
         *
         * @param pc The particle container.
         *
         * @return The radial distribution function.
         */
        std::vector<size_t> generateRDF(const ParticleContainer& pc);

        /**
         * Handles the creation and writing of the vtk file.
         *
         * @param container List of particles to be plotted.
         * @param filename The base name of the file to be written.
         * @param iteration The number of the current iteration, which is used to generate an unique filename.
         */
        virtual void plotParticles(const ParticleContainer& pc, const std::string& filename, const int iteration);
    };
}
