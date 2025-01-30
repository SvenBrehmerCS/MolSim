/**
 * @file
 *
 * @brief This file stores the utility required for generating radial distribution functions.
 */

#pragma once

#include "Environment.h"
#include "container/ParticleContainer.h"

#include <fstream>
#include <string>
#include <vector>

namespace outputWriter {

    class RDF {
    private:
        /**
         * The name of the file to write to.
         */
        std::string rdfFile;

        /**
         * The number of buckets.
         */
        size_t buckets;

        /**
         * The width of each bucket.
         */
        double delta_r;

    public:
        /**
         * Constructor for the RDF class.
         *
         * @param delta_r The width of each bucket.
         * @param buckets The number of buckets.
         * @param filename The name of the file to write to.
         */
        RDF(const double delta_r, const size_t buckets, const std::string& filename);

        /**
         * @brief Destructor for the RDF class.
         */
        ~RDF() = default;

        /**
         * Generates a radial distribution function.
         *
         * @param pc The particle container.
         * @param bt The type of the boundaries.
         * @param domain The domain size.
         *
         * @return The radial distribution function.
         */
        std::vector<size_t> generateRDF(const ParticleContainer& pc, const std::array<BoundaryType, 6>& bt, const Vec<double> domain);

        /**
         * Writes the RDF to a file.
         *
         * @param pc The particle container.
         * @param bt The type of the boundaries.
         * @param domain The domain size.
         * @param it The current iteration.
         */
        void writeRDF(const ParticleContainer& pc, const std::array<BoundaryType, 6>& bt, const Vec<double> domain, const size_t it);
    };
}
