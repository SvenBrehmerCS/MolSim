/*
 * XYZWriter.h
 *
 *  Created on: 01.03.2010
 *      Author: eckhardw
 */

#pragma once

#include "Writer.h"

#include <fstream>
#include <list>

namespace outputWriter {

    /**
     * This class implements the generation of an XYZ output from particles.
     */
    class XYZWriter : public Writer {

    public:
        XYZWriter();

        virtual ~XYZWriter();

        /**
         * creates the XYZ file and plots the particles
         *
         * @param particles list of particles to be plotted.
         * @param filename the base name of the file to be written.
         * @param iteration the number of the current iteration,
         *        which is used to generate an unique filename
         */
        virtual void plotParticles(const std::list<Particle>& particles, const std::string& filename, int iteration);
    };

} // namespace outputWriter
