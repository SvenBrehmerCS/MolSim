/**
 * @file
 *
 * @brief Handles the output to a .xyz file
 *
 * @author eckhardw
 */

#pragma once

#include "Writer.h"

#include <fstream>

namespace outputWriter {

    /**
     * @class XYZWriter
     *
     * @brief Class implements the generation of an XYZ output from particles.
     */
    class XYZWriter : public Writer {

    public:
        XYZWriter();

        virtual ~XYZWriter();

        /**
         * Creates the XYZ file and plots the particles
         *
         * @param particles List of particles to be plotted.
         * @param filename The base name of the file to be written.
         * @param iteration The number of the current iteration,
         *        which is used to generate an unique filename
         */
        virtual void plotParticles(const ParticleContainer& container, const std::string& filename, int iteration);
    };

} // namespace outputWriter
