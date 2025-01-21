#include "Stepper.h"

#include <spdlog/spdlog.h>

#include "boundaries/GhostBoundary.h"
#include "boundaries/HardBoundary.h"
#include "boundaries/NoBoundary.h"
#include "boundaries/PeriodicBoundary.h"
#include "container/BoxContainer.h"

Stepper::Stepper(const std::array<BoundaryType, 6>& bt, const Vec<double>& new_domain, const physicsCalculator::Tweezers& tweezers) {
    bound_t = bt;
    domain = new_domain;
    tw = tweezers;

    // Initialize the particle container.
    for (size_t i = 0; i < 6; i++) {
        const double pos = i < 3 ? 0.0 : new_domain[i % 3];
        switch (bound_t[i]) {
        case INF_CONT:
            bc[i] = std::make_unique<NoBoundary>(pos, i % 3);
            break;
        case HALO:
            bc[i] = std::make_unique<GhostBoundary>(pos, i % 3);
            break;
        case HARD:
            bc[i] = std::make_unique<HardBoundary>(pos, i % 3);
            break;
        case PERIODIC:
            bc[i] = std::make_unique<PeriodicBoundary>(pos, i % 3);
            break;
        case OUTFLOW:
            bc[i] = std::make_unique<NoBoundary>(pos, i % 3);
            out = true;
            break;
        default:
            SPDLOG_CRITICAL("Unsupported boundary type.");
            std::exit(EXIT_FAILURE);
            break;
        }
    }
}

void Stepper::step(physicsCalculator::Calculator& calc, const double t) {
    calc.calculateX();

    for (Particle& p : calc.get_container()) {
        for (size_t i = 0; i < bc.size(); i++) {
            bc[i]->postX(p);
        }
    }

    if (out) {
        calc.get_container().remove_particles_out_of_domain();
    }

    calc.get_container().update_positions();

    calc.calculateOldF();
    calc.calculateF();

    for (Particle& p : calc.get_container()) {
        for (size_t i = 0; i < bc.size(); i++) {
            bc[i]->postF(p, calc);
        }
    }

    if(t <= tw.get_end()) {
        tw.apply(calc.get_container());
    }

    if (bound_t[0] == PERIODIC) {
        BoxContainer& cont = dynamic_cast<BoxContainer&>(calc.get_container());

        cont.iterate_yz_pairs([this, &calc](Particle& p1, Particle& p2) {
            const Vec<double> arr = p1.getX() + Vec(domain[0], 0.0, 0.0) - p2.getX();
            const double dist = arr.len_squ();
            const double force = calc.calculateFAbs(p1, p2, dist);

            // Update the forces for both particles
            p1.setF(-force * arr + p1.getF());
            p2.setF(force * arr + p2.getF());
        });

        if (bound_t[1] == PERIODIC) {
            cont.loop_z_near([this, &calc](Particle& p1, Particle& p2) {
                const Vec<double> arr = p1.getX() + Vec<double>(domain[0], domain[1], 0.0) - p2.getX();
                const double dist = arr.len_squ();
                const double force = calc.calculateFAbs(p1, p2, dist);

                // Update the forces for both particles
                p1.setF(-force * arr + p1.getF());
                p2.setF(force * arr + p2.getF());
            });
            cont.loop_z_far([this, &calc](Particle& p1, Particle& p2) {
                const Vec<double> arr = p1.getX() + Vec<double>(-domain[0], domain[1], 0.0) - p2.getX();
                const double dist = arr.len_squ();
                const double force = calc.calculateFAbs(p1, p2, dist);

                // Update the forces for both particles
                p1.setF(-force * arr + p1.getF());
                p2.setF(force * arr + p2.getF());
            });
        }

        if (bound_t[2] == PERIODIC) {
            cont.loop_y_near([this, &calc](Particle& p1, Particle& p2) {
                const Vec<double> arr = p1.getX() + Vec<double>(domain[0], 0.0, domain[2]) - p2.getX();
                const double dist = arr.len_squ();
                const double force = calc.calculateFAbs(p1, p2, dist);

                // Update the forces for both particles
                p1.setF(-force * arr + p1.getF());
                p2.setF(force * arr + p2.getF());
            });
            cont.loop_y_far([this, &calc](Particle& p1, Particle& p2) {
                const Vec<double> arr = p1.getX() + Vec<double>(-domain[0], 0.0, domain[2]) - p2.getX();
                const double dist = arr.len_squ();
                const double force = calc.calculateFAbs(p1, p2, dist);

                // Update the forces for both particles
                p1.setF(-force * arr + p1.getF());
                p2.setF(force * arr + p2.getF());
            });
        }
    }

    if (bound_t[1] == PERIODIC) {
        BoxContainer& cont = dynamic_cast<BoxContainer&>(calc.get_container());
        cont.iterate_xz_pairs([this, &calc](Particle& p1, Particle& p2) {
            const Vec<double> arr = p1.getX() + Vec<double>(0.0, domain[1], 0.0) - p2.getX();
            const double dist = arr.len_squ();
            const double force = calc.calculateFAbs(p1, p2, dist);

            // Update the forces for both particles
            p1.setF(-force * arr + p1.getF());
            p2.setF(force * arr + p2.getF());
        });

        if (bound_t[2] == PERIODIC) {
            cont.loop_x_near([this, &calc](Particle& p1, Particle& p2) {
                const Vec<double> arr = p1.getX() + Vec<double>(0.0, domain[1], domain[2]) - p2.getX();
                const double dist = arr.len_squ();
                const double force = calc.calculateFAbs(p1, p2, dist);

                // Update the forces for both particles
                p1.setF(-force * arr + p1.getF());
                p2.setF(force * arr + p2.getF());
            });
            cont.loop_x_far([this, &calc](Particle& p1, Particle& p2) {
                const Vec<double> arr = p1.getX() + Vec<double>(0.0, -domain[1], domain[2]) - p2.getX();
                const double dist = arr.len_squ();
                const double force = calc.calculateFAbs(p1, p2, dist);

                // Update the forces for both particles
                p1.setF(-force * arr + p1.getF());
                p2.setF(force * arr + p2.getF());
            });
        }
    }

    if (bound_t[2] == PERIODIC) {
        BoxContainer& cont = dynamic_cast<BoxContainer&>(calc.get_container());

        cont.iterate_xy_pairs([this, &calc](Particle& p1, Particle& p2) {
            const Vec<double> arr = p1.getX() + Vec<double>(0.0, 0.0, domain[2]) - p2.getX();
            const double dist = arr.len_squ();
            const double force = calc.calculateFAbs(p1, p2, dist);

            // Update the forces for both particles
            p1.setF(-force * arr + p1.getF());
            p2.setF(force * arr + p2.getF());
        });
    }

    if (bound_t[0] == PERIODIC && bound_t[1] == PERIODIC && bound_t[2] == PERIODIC) {
        BoxContainer& cont = dynamic_cast<BoxContainer&>(calc.get_container());

        cont.loop_origin_corner([this, &calc](Particle& p1, Particle& p2) {
            const Vec<double> arr = p1.getX() + Vec<double>(domain[0], domain[1], domain[2]) - p2.getX();
            const double dist = arr.len_squ();
            const double force = calc.calculateFAbs(p1, p2, dist);

            // Update the forces for both particles
            p1.setF(-force * arr + p1.getF());
            p2.setF(force * arr + p2.getF());
        });

        cont.loop_x_corner([this, &calc](Particle& p1, Particle& p2) {
            const Vec<double> arr = p1.getX() + Vec<double>(-domain[0], domain[1], domain[2]) - p2.getX();
            const double dist = arr.len_squ();
            const double force = calc.calculateFAbs(p1, p2, dist);

            // Update the forces for both particles
            p1.setF(-force * arr + p1.getF());
            p2.setF(force * arr + p2.getF());
        });

        cont.loop_y_corner([this, &calc](Particle& p1, Particle& p2) {
            const Vec<double> arr = p1.getX() + Vec<double>(domain[0], -domain[1], domain[2]) - p2.getX();
            const double dist = arr.len_squ();
            const double force = calc.calculateFAbs(p1, p2, dist);

            // Update the forces for both particles
            p1.setF(-force * arr + p1.getF());
            p2.setF(force * arr + p2.getF());
        });

        cont.loop_xy_corner([this, &calc](Particle& p1, Particle& p2) {
            const Vec<double> arr = p1.getX() + Vec<double>(-domain[0], -domain[1], domain[2]) - p2.getX();
            const double dist = arr.len_squ();
            const double force = calc.calculateFAbs(p1, p2, dist);

            // Update the forces for both particles
            p1.setF(-force * arr + p1.getF());
            p2.setF(force * arr + p2.getF());
        });
    }

    calc.calculateV();
}
