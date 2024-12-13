#include "Stepper.h"

#include "boundaries/BoxContainer.h"

Stepper::Stepper(
    const std::array<Boundary*, 6>& boundaries, const std::array<BoundaryType, 6>& bt, const bool is_inf, const std::array<double, 3>& d) {
    bc = boundaries;
    bound_t = bt;
    inf = is_inf;
    domain = d;
}

void Stepper::step(physicsCalculator::Calculator& calc) {
    calc.calculateX();

    for (Particle& p : calc.get_container()) {
        for (size_t i = 0; i < bc.size(); i++) {
            bc[i]->postX(p);
        }
    }

    if (!inf) {
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

    if (bound_t[0] == PERIODIC) {
        BoxContainer& cont = dynamic_cast<BoxContainer&>(calc.get_container());

        cont.iterate_xy_pairs([this, &calc](Particle& p1, Particle& p2) {
            const std::array<double, 3> arr = p1.getX() + std::array<double, 3>({ 0.0, 0.0, domain[2] }) - p2.getX();
            const double dist = ArrayUtils::L2Norm(arr);
            const double force = calc.calculateFAbs(p1, p2, dist);

            // Update the forces for both particles
            p1.setF(force * arr + p1.getF());
            p2.setF(-force * arr + p2.getF());
        });

        if (bound_t[1] == PERIODIC) {
            cont.loop_x_near([this, &calc](Particle& p1, Particle& p2) {
                const std::array<double, 3> arr = p1.getX() + std::array<double, 3>({ 0.0, domain[1], domain[2] }) - p2.getX();
                const double dist = ArrayUtils::L2Norm(arr);
                const double force = calc.calculateFAbs(p1, p2, dist);

                // Update the forces for both particles
                p1.setF(force * arr + p1.getF());
                p2.setF(-force * arr + p2.getF());
            });
            cont.loop_x_far([this, &calc](Particle& p1, Particle& p2) {
                const std::array<double, 3> arr = p1.getX() + std::array<double, 3>({ 0.0, -domain[1], domain[2] }) - p2.getX();
                const double dist = ArrayUtils::L2Norm(arr);
                const double force = calc.calculateFAbs(p1, p2, dist);

                // Update the forces for both particles
                p1.setF(force * arr + p1.getF());
                p2.setF(-force * arr + p2.getF());
            });
        }

        if (bound_t[2] == PERIODIC) {
            cont.loop_y_near([this, &calc](Particle& p1, Particle& p2) {
                const std::array<double, 3> arr = p1.getX() + std::array<double, 3>({ domain[0], 0.0, domain[2] }) - p2.getX();
                const double dist = ArrayUtils::L2Norm(arr);
                const double force = calc.calculateFAbs(p1, p2, dist);

                // Update the forces for both particles
                p1.setF(force * arr + p1.getF());
                p2.setF(-force * arr + p2.getF());
            });
            cont.loop_y_far([this, &calc](Particle& p1, Particle& p2) {
                const std::array<double, 3> arr = p1.getX() + std::array<double, 3>({ -domain[0], 0.0, domain[2] }) - p2.getX();
                const double dist = ArrayUtils::L2Norm(arr);
                const double force = calc.calculateFAbs(p1, p2, dist);

                // Update the forces for both particles
                p1.setF(force * arr + p1.getF());
                p2.setF(-force * arr + p2.getF());
            });
        }
    }

    if (bound_t[1] == PERIODIC) {
        BoxContainer& cont = dynamic_cast<BoxContainer&>(calc.get_container());
        cont.iterate_xz_pairs([this, &calc](Particle& p1, Particle& p2) {
            const std::array<double, 3> arr = p1.getX() + std::array<double, 3>({ 0.0, domain[1], 0.0 }) - p2.getX();
            const double dist = ArrayUtils::L2Norm(arr);
            const double force = calc.calculateFAbs(p1, p2, dist);

            // Update the forces for both particles
            p1.setF(force * arr + p1.getF());
            p2.setF(-force * arr + p2.getF());
        });

        if (bound_t[2] == PERIODIC) {
            cont.loop_z_near([this, &calc](Particle& p1, Particle& p2) {
                const std::array<double, 3> arr = p1.getX() + std::array<double, 3>({ domain[0], domain[1], 0.0 }) - p2.getX();
                const double dist = ArrayUtils::L2Norm(arr);
                const double force = calc.calculateFAbs(p1, p2, dist);

                // Update the forces for both particles
                p1.setF(force * arr + p1.getF());
                p2.setF(-force * arr + p2.getF());
            });
            cont.loop_z_far([this, &calc](Particle& p1, Particle& p2) {
                const std::array<double, 3> arr = p1.getX() + std::array<double, 3>({ -domain[0], -domain[1], 0.0 }) - p2.getX();
                const double dist = ArrayUtils::L2Norm(arr);
                const double force = calc.calculateFAbs(p1, p2, dist);

                // Update the forces for both particles
                p1.setF(force * arr + p1.getF());
                p2.setF(-force * arr + p2.getF());
            });
        }
    }

    if (bound_t[2] == PERIODIC) {
        BoxContainer& cont = dynamic_cast<BoxContainer&>(calc.get_container());

        cont.iterate_yz_pairs([this, &calc](Particle& p1, Particle& p2) {
            const std::array<double, 3> arr = p1.getX() + std::array<double, 3>({ domain[0], 0.0, 0.0 }) - p2.getX();
            const double dist = ArrayUtils::L2Norm(arr);
            const double force = calc.calculateFAbs(p1, p2, dist);

            // Update the forces for both particles
            p1.setF(force * arr + p1.getF());
            p2.setF(-force * arr + p2.getF());
        });
    }

    if (bound_t[0] == PERIODIC && bound_t[1] == PERIODIC && bound_t[2] == PERIODIC) {
        BoxContainer& cont = dynamic_cast<BoxContainer&>(calc.get_container());

        cont.loop_origin_corner([this, &calc](Particle& p1, Particle& p2) {
            const std::array<double, 3> arr = p1.getX() + std::array<double, 3>({ domain[0], domain[1], domain[2] }) - p2.getX();
            const double dist = ArrayUtils::L2Norm(arr);
            const double force = calc.calculateFAbs(p1, p2, dist);

            // Update the forces for both particles
            p1.setF(force * arr + p1.getF());
            p2.setF(-force * arr + p2.getF());
        });

        cont.loop_x_corner([this, &calc](Particle& p1, Particle& p2) {
            const std::array<double, 3> arr = p1.getX() + std::array<double, 3>({ -domain[0], domain[1], domain[2] }) - p2.getX();
            const double dist = ArrayUtils::L2Norm(arr);
            const double force = calc.calculateFAbs(p1, p2, dist);

            // Update the forces for both particles
            p1.setF(force * arr + p1.getF());
            p2.setF(-force * arr + p2.getF());
        });

        cont.loop_y_corner([this, &calc](Particle& p1, Particle& p2) {
            const std::array<double, 3> arr = p1.getX() + std::array<double, 3>({ domain[0], -domain[1], domain[2] }) - p2.getX();
            const double dist = ArrayUtils::L2Norm(arr);
            const double force = calc.calculateFAbs(p1, p2, dist);

            // Update the forces for both particles
            p1.setF(force * arr + p1.getF());
            p2.setF(-force * arr + p2.getF());
        });

        cont.loop_xy_corner([this, &calc](Particle& p1, Particle& p2) {
            const std::array<double, 3> arr = p1.getX() + std::array<double, 3>({ -domain[0], -domain[1], domain[2] }) - p2.getX();
            const double dist = ArrayUtils::L2Norm(arr);
            const double force = calc.calculateFAbs(p1, p2, dist);

            // Update the forces for both particles
            p1.setF(force * arr + p1.getF());
            p2.setF(-force * arr + p2.getF());
        });
    }

    calc.calculateV();
}
