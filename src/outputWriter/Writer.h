/*
 * VTKWriter.h
 *
 *  Created on: 23.10.2024
 *      Author: Sven Brehmer
 */

#pragma once
#include <list>
#include <string>

class Particle;

namespace outputWriter {

    class Writer {

    public:
        /**
         * creates an output file based on the state of the given particles
         *
         * @param particles list of particles to be plotted.
         * @param filename the base name of the file to be written.
         * @param iteration the number of the current iteration,
         *        which is used to generate an unique filename
         */
        virtual void plotParticles(const std::list<Particle>& particles, const std::string& filename, int iteration) = 0;
    };
} // namespace outputWriter