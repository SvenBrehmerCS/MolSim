/*
 * VTKWriter.h
 *
 *  Created on: 01.03.2010
 *      Author: eckhardw
 */

#pragma once

#include "Writer.h"
#include "outputWriter/vtk-unstructured.h"

#include <list>

namespace outputWriter {

    /**
     * This class implements the functionality to generate vtk output from
     * particles.
     */
    class VTKWriter : public Writer {

    public:
        VTKWriter();

        virtual ~VTKWriter();

        /**
         * set up internal data structures and prepare to plot a particle.
         */
        void initializeOutput(int numParticles);

        /**
         * plot type, mass, position, velocity and force of a particle.
         *
         * @note: initializeOutput() must have been called before.
         */
        void plotParticle(const Particle& p);

        /**
         * writes the final output file.
         *
         * @param filename the base name of the file to be written.
         * @param iteration the number of the current iteration,
         *        which is used to generate an unique filename
         */
        void writeFile(const std::string& filename, int iteration);

        /**
         * handles the creation and writing of the vtk file
         *
         * @param particles list of particles to be plotted.
         * @param filename the base name of the file to be written.
         * @param iteration the number of the current iteration,
         *        which is used to generate an unique filename
         */
        virtual void plotParticles(const std::list<Particle>& particles, const std::string& filename, int iteration);

    private:
        VTKFile_t* vtkFile;
    };

} // namespace outputWriter
