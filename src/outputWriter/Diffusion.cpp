#include "Diffusion.h"


namespace outputWriter {
    Diffusion::~Diffusion() {
        file.flush();
        file.close();
    }

    void Diffusion::set_out_file(const std::string& out_name) { file.open(out_name); }

    void Diffusion::add_diffusion(const ParticleContainer& pc, const Environment& env) {
        double var = 0.0;

        for (size_t i = 0; i < pc.size(); i++) {
            var += (env.get_delta_t() * pc[i].getV() + pc.get_type_descriptor(pc[i].getType()).get_dt_dt_m() * pc[i].getF()).len_squ();
        }

        var /= pc.size();

        file << var << "\t";
    }
}
