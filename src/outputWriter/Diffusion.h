/**
 * @file
 *
 * @brief Handle computing and writing the diffusion.
 */

#pragma once

#include "Environment.h"
#include "container/ParticleContainer.h"

#include <fstream>

namespace outputWriter {

    /**
     * @class Diffusion
     *
     * @brief This class implements the functionality to generate a diffusion file.
     */
    class Diffusion {
    private:
        /**
         * Store the output file.
         */
        std::ofstream file;

    public:
        /**
         * Create a default diffusion class.
         */
        Diffusion() = default;

        /**
         * Destroy a diffusion object.
         */
        ~Diffusion();

        /**
         * Set the output file.
         *
         * @param out_name The name of the output file.
         */
        void set_out_file(const std::string& out_name);

        /**
         * Perform a diffusion step.
         *
         * @param pc The particle container.
         * @param env The environment descriptor.
         */
        void add_diffusion(const ParticleContainer& pc, const Environment& env);
    };
}
