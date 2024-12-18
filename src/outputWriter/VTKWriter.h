/**
 * @file
 *
 * @brief Handles the output to a .vtu file
 *
 * @author eckhardw
 */

#pragma once

#include "Writer.h"
#include "vtk-unstructured.h"

namespace outputWriter {

    /**
     * @class VTKWriter
     *
     * @brief This class implements the functionality to generate vtk output from
     * particles.
     */
    class VTKWriter : public Writer {

    private:
        /**
         * Stores the open VTK file.
         */
        std::unique_ptr<VTKFile_t> vtkFile;

    public:
        VTKWriter();

        virtual ~VTKWriter();

        /**
         * Set up internal data structures and prepare to plot a particle.
         *
         * @param numParticles Number of particles to be plotted.
         */
        void initializeOutput(const int numParticles);

        /**
         * Plot type, mass, position, velocity and force of a particle.
         *
         * @param p Particle to plot.
         * @param mass Mass of particle.
         *
         * @note: initializeOutput() must have been called before.
         */
        void plotParticle(const Particle& p, const double mass);

        /**
         * Writes the final output file.
         *
         * @param filename The base name of the file to be written.
         * @param iteration The number of the current iteration, which is used to generate an unique filename.
         */
        void writeFile(const std::string& filename, const int iteration);

        /**
         * Handles the creation and writing of the vtk file
         *
         * @param container List of particles to be plotted.
         * @param filename The base name of the file to be written.
         * @param iteration The number of the current iteration, which is used to generate an unique filename.
         */
        virtual void plotParticles(const ParticleContainer& container, const std::string& filename, const int iteration);
    };
} // namespace outputWriter
