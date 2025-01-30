#include <container/BoxContainer.h>
#include <gtest/gtest.h>
#include <physicsCalculator/Tweezers.h>

// Test if the tweezers work correctly if they should not affect any out of four atoms
TEST(Tweezers, NoEffect) {
    std::vector<Particle> particles = {
        Particle({ 0.5, 0.5, 0.5 }, {}, 1),
        Particle({ 1.25, 0.5, 0.5 }, {}, 2),
        Particle({ 0.5, 1.25, 0.5 }, {}, 3),
        Particle({ 1.1, 1.1, 0.5 }, {}, 4),
    };

    for (size_t i = 0; i < particles.size(); i++) {
        particles[i].setF({ 1.0, 2.0, 3.0 });
    }


    BoxContainer box = BoxContainer(particles, 1.0, { 5.0, 5.0, 1.0 }, {});

    physicsCalculator::Tweezers tw = physicsCalculator::Tweezers({}, { 10, 100, 10 }, 100.0);

    tw.apply(box);

    for (size_t i = 0; i < particles.size(); i++) {
        EXPECT_EQ(box[i].getF(), Vec<double>(1.0, 2.0, 3.0)) << "The force on the particles must not have changed at all.";
    }
}

// Test if the tweezers work correctly if they should affect one out of four atoms
TEST(Tweezers, SingleAtom) {
    std::vector<Particle> particles = {
        Particle({ 0.5, 0.5, 0.5 }, {}, 1),
        Particle({ 1.25, 0.5, 0.5 }, {}, 2),
        Particle({ 0.5, 1.25, 0.5 }, {}, 3),
        Particle({ 1.1, 1.1, 0.5 }, {}, 4),
    };

    for (size_t i = 0; i < particles.size(); i++) {
        particles[i].setF({ 1.0, 2.0, 3.0 });
    }


    BoxContainer box = BoxContainer(particles, 1.0, { 5.0, 5.0, 1.0 }, {});

    physicsCalculator::Tweezers tw = physicsCalculator::Tweezers({ 1, 2 }, { 10, 100, 10 }, 100.0);

    tw.apply(box);

    EXPECT_EQ(box[0].getF(), Vec<double>(1.0, 2.0, 3.0)) << "Particle 0 must not change.";
    EXPECT_EQ(box[1].getF(), Vec<double>(11.0, 102.0, 13.0)) << "Particle 1 must change.";
    EXPECT_EQ(box[2].getF(), Vec<double>(11.0, 102.0, 13.0)) << "Particle 2 must change.";
    EXPECT_EQ(box[3].getF(), Vec<double>(1.0, 2.0, 3.0)) << "Particle 3 must not change.";
}

// Test if the tweezers work correctly if they should affect two out of two atoms
TEST(Tweezers, Full) {
std::vector<Particle> particles = {
        Particle({ 0.5, 0.5, 0.5 }, {}, 1),
        Particle({ 1.25, 0.5, 0.5 }, {}, 2),
    };

    for (size_t i = 0; i < particles.size(); i++) {
        particles[i].setF({ 1.0, 2.0, 3.0 });
    }


    BoxContainer box = BoxContainer(particles, 1.0, { 5.0, 5.0, 1.0 }, {});

    physicsCalculator::Tweezers tw = physicsCalculator::Tweezers({ 0, 1 }, { 10, 100, 10 }, 100.0);

    tw.apply(box);

    EXPECT_EQ(box[0].getF(), Vec<double>(11.0, 102.0, 13.0)) << "Particle 0 must change.";
    EXPECT_EQ(box[1].getF(), Vec<double>(11.0, 102.0, 13.0)) << "Particle 1 must change.";
}
