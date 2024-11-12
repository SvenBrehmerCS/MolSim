/**
 * @file
 *
 * @brief Handles no output at all
 */

#pragma once

#include "Writer.h"

namespace outputWriter {

    /**
     * @class VTKWriter
     *
     * @brief This class implements the functionality to generate vtk output from
     * particles.
     */
    class NoWriter : public Writer {

    public:
        NoWriter();

        virtual ~NoWriter();

        /**
         * Handles the creation and writing of the vtk file
         *
         * @param particles List of particles to be plotted.
         * @param filename The base name of the file to be written.
         * @param iteration The number of the current iteration, which is used to generate an unique filename
         */
        virtual void plotParticles(const ParticleContainer& container, const std::string& filename, int iteration);
    };

} // namespace outputWriter