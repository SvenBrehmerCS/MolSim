#include "RDF.h"

namespace outputWriter {

    RDF::RDF(const double delta_r, const size_t buckets, const std::string& filename) {
        this->buckets = buckets;
        this->delta_r = delta_r;
        rdfFile = filename;
    }

    std::vector<size_t> RDF::generateRDF(const ParticleContainer& pc, const std::array<BoundaryType, 6>& bt, const Vec<double> domain) {
        std::vector<size_t> rdf(buckets);

        const double max_dist = buckets * delta_r;

        for (size_t i = 0; i < pc.size(); i++) {
            for (size_t j = 0; j < pc.size(); j++) {
                if (i == j) {
                    continue;
                }

                Particle p1 = pc[i];
                Particle p2 = pc[j];

                // Handel their distance within the domain
                const double dist = (p1.getX() - p2.getX()).len();

                if (dist < max_dist) {
                    const size_t bucket = static_cast<size_t>(dist / delta_r);
                    rdf[bucket]++;
                }

                // Handle periodic boundaries
                if (bt[0] == PERIODIC) {
                    const double dist_p = (p1.getX() - (p2.getX() + Vec<double>(domain[0], 0.0, 0.0))).len();
                    const double dist_np = (p1.getX() - (p2.getX() - Vec<double>(domain[0], 0.0, 0.0))).len();

                    if (dist_p < max_dist) {
                        const size_t bucket = static_cast<size_t>(dist_p / delta_r);
                        rdf[bucket]++;
                    }

                    if (dist_np < max_dist) {
                        const size_t bucket = static_cast<size_t>(dist_np / delta_r);
                        rdf[bucket]++;
                    }
                }

                if (bt[1] == PERIODIC) {
                    const double dist_p = (p1.getX() - (p2.getX() + Vec<double>(0.0, domain[1], 0.0))).len();
                    const double dist_np = (p1.getX() - (p2.getX() - Vec<double>(0.0, domain[1], 0.0))).len();

                    if (dist_p < max_dist) {
                        const size_t bucket = static_cast<size_t>(dist_p / delta_r);
                        rdf[bucket]++;
                    }

                    if (dist_np < max_dist) {
                        const size_t bucket = static_cast<size_t>(dist_np / delta_r);
                        rdf[bucket]++;
                    }
                }

                if (bt[2] == PERIODIC) {
                    const double dist_p = (p1.getX() - (p2.getX() + Vec<double>(0.0, 0.0, domain[2]))).len();
                    const double dist_np = (p1.getX() - (p2.getX() - Vec<double>(0.0, 0.0, domain[2]))).len();

                    if (dist_p < max_dist) {
                        const size_t bucket = static_cast<size_t>(dist_p / delta_r);
                        rdf[bucket]++;
                    }

                    if (dist_np < max_dist) {
                        const size_t bucket = static_cast<size_t>(dist_np / delta_r);
                        rdf[bucket]++;
                    }
                }

                if (bt[0] == PERIODIC && bt[1] == PERIODIC) {
                    const double dist_pp = (p1.getX() - (p2.getX() + Vec<double>(domain[0], domain[1], 0.0))).len();
                    const double dist_pn = (p1.getX() - (p2.getX() + Vec<double>(domain[0], -domain[1], 0.0))).len();
                    const double dist_np = (p1.getX() - (p2.getX() - Vec<double>(domain[0], domain[1], 0.0))).len();
                    const double dist_nn = (p1.getX() - (p2.getX() - Vec<double>(domain[0], domain[1], 0.0))).len();

                    if (dist_pp < max_dist) {
                        const size_t bucket = static_cast<size_t>(dist_pp / delta_r);
                        rdf[bucket]++;
                    }

                    if (dist_pn < max_dist) {
                        const size_t bucket = static_cast<size_t>(dist_pn / delta_r);
                        rdf[bucket]++;
                    }

                    if (dist_np < max_dist) {
                        const size_t bucket = static_cast<size_t>(dist_np / delta_r);
                        rdf[bucket]++;
                    }

                    if (dist_nn < max_dist) {
                        const size_t bucket = static_cast<size_t>(dist_nn / delta_r);
                        rdf[bucket]++;
                    }
                }

                if (bt[0] == PERIODIC && bt[2] == PERIODIC) {
                    const double dist_pp = (p1.getX() - (p2.getX() + Vec<double>(domain[0], 0.0, domain[2]))).len();
                    const double dist_pn = (p1.getX() - (p2.getX() + Vec<double>(domain[0], 0.0, -domain[2]))).len();
                    const double dist_np = (p1.getX() - (p2.getX() - Vec<double>(domain[0], 0.0, domain[2]))).len();
                    const double dist_nn = (p1.getX() - (p2.getX() - Vec<double>(domain[0], 0.0, -domain[2]))).len();

                    if (dist_pp < max_dist) {
                        const size_t bucket = static_cast<size_t>(dist_pp / delta_r);
                        rdf[bucket]++;
                    }

                    if (dist_pn < max_dist) {
                        const size_t bucket = static_cast<size_t>(dist_pn / delta_r);
                        rdf[bucket]++;
                    }

                    if (dist_np < max_dist) {
                        const size_t bucket = static_cast<size_t>(dist_np / delta_r);
                        rdf[bucket]++;
                    }

                    if (dist_nn < max_dist) {
                        const size_t bucket = static_cast<size_t>(dist_nn / delta_r);
                        rdf[bucket]++;
                    }
                }

                if (bt[1] == PERIODIC && bt[2] == PERIODIC) {
                    const double dist_pp = (p1.getX() - (p2.getX() + Vec<double>(0.0, domain[1], domain[2]))).len();
                    const double dist_pn = (p1.getX() - (p2.getX() + Vec<double>(0.0, domain[1], -domain[2]))).len();
                    const double dist_np = (p1.getX() - (p2.getX() - Vec<double>(0.0, domain[1], domain[2]))).len();
                    const double dist_nn = (p1.getX() - (p2.getX() - Vec<double>(0.0, domain[1], -domain[2]))).len();

                    if (dist_pp < max_dist) {
                        const size_t bucket = static_cast<size_t>(dist_pp / delta_r);
                        rdf[bucket]++;
                    }

                    if (dist_pn < max_dist) {
                        const size_t bucket = static_cast<size_t>(dist_pn / delta_r);
                        rdf[bucket]++;
                    }

                    if (dist_np < max_dist) {
                        const size_t bucket = static_cast<size_t>(dist_np / delta_r);
                        rdf[bucket]++;
                    }

                    if (dist_nn < max_dist) {
                        const size_t bucket = static_cast<size_t>(dist_nn / delta_r);
                        rdf[bucket]++;
                    }
                }

                if (bt[0] == PERIODIC && bt[1] == PERIODIC && bt[2] == PERIODIC) {
                    const double dist_ppp = (p1.getX() - (p2.getX() + Vec<double>(domain[0], domain[1], domain[2]))).len();
                    const double dist_ppn = (p1.getX() - (p2.getX() + Vec<double>(domain[0], domain[1], -domain[2]))).len();
                    const double dist_pnp = (p1.getX() - (p2.getX() + Vec<double>(domain[0], -domain[1], domain[2]))).len();
                    const double dist_pnn = (p1.getX() - (p2.getX() + Vec<double>(domain[0], -domain[1], -domain[2]))).len();
                    const double dist_npp = (p1.getX() - (p2.getX() - Vec<double>(domain[0], domain[1], domain[2]))).len();
                    const double dist_npn = (p1.getX() - (p2.getX() - Vec<double>(domain[0], domain[1], -domain[2]))).len();
                    const double dist_nnp = (p1.getX() - (p2.getX() - Vec<double>(domain[0], -domain[1], domain[2]))).len();
                    const double dist_nnn = (p1.getX() - (p2.getX() - Vec<double>(domain[0], -domain[1], -domain[2]))).len();

                    if (dist_ppp < max_dist) {
                        const size_t bucket = static_cast<size_t>(dist_ppp / delta_r);
                        rdf[bucket]++;
                    }

                    if (dist_ppn < max_dist) {
                        const size_t bucket = static_cast<size_t>(dist_ppn / delta_r);
                        rdf[bucket]++;
                    }

                    if (dist_pnp < max_dist) {
                        const size_t bucket = static_cast<size_t>(dist_pnp / delta_r);
                        rdf[bucket]++;
                    }

                    if (dist_pnn < max_dist) {
                        const size_t bucket = static_cast<size_t>(dist_pnn / delta_r);
                        rdf[bucket]++;
                    }

                    if (dist_npp < max_dist) {
                        const size_t bucket = static_cast<size_t>(dist_npp / delta_r);
                        rdf[bucket]++;
                    }

                    if (dist_npn < max_dist) {
                        const size_t bucket = static_cast<size_t>(dist_npn / delta_r);
                        rdf[bucket]++;
                    }

                    if (dist_nnp < max_dist) {
                        const size_t bucket = static_cast<size_t>(dist_nnp / delta_r);
                        rdf[bucket]++;
                    }

                    if (dist_nnn < max_dist) {
                        const size_t bucket = static_cast<size_t>(dist_nnn / delta_r);
                        rdf[bucket]++;
                    }
                }
            }
        }

        return rdf;
    }

    void RDF::writeRDF(const ParticleContainer& pc, const std::array<BoundaryType, 6>& bt, const Vec<double> domain, const size_t it) {
        std::vector<size_t> rdf = generateRDF(pc, bt, domain);
        std::fstream file(rdfFile + std::to_string(it));

        for (size_t i = 0; i < rdf.size(); i++) {
            const double r_i = i * delta_r;
            const double r_d_r = (r_i + delta_r);
            file << (rdf[i] / (4.0 * M_PI / 3.0 * (r_d_r * r_d_r * r_d_r - r_i * r_i * r_i))) << "\t";
        }

        file.flush();
        file.close();
    }
}
