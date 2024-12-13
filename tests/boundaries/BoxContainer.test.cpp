#include <container/BoxContainer.h>
#include <gtest/gtest.h>

// Test the iterate pairs method for a box container in a complex simulation setting.
TEST(BoxContainer, IteratePairs) {
    std::vector<Particle> particles = {
        Particle({ 0.5, 0.5, 0.5 }, {}, 1.0, 1),
        Particle({ 1.25, 0.5, 0.5 }, {}, 1.0, 2),
        Particle({ 0.5, 1.25, 0.5 }, {}, 1.0, 3),
        Particle({ 1.1, 1.1, 0.5 }, {}, 1.0, 4),
        Particle({ 1.9, 1.9, 0.5 }, {}, 1.0, 5),
        Particle({ 3.5, 3.5, 0.5 }, {}, 1.0, 6),
        Particle({ 3.5, 3.75, 0.5 }, {}, 1.0, 7),
        Particle({ 3.5, 3.25, 0.5 }, {}, 1.0, 8),
    };

    BoxContainer box = BoxContainer(particles, 1.0, { 5.0, 5.0, 1.0 });

    std::list<std::tuple<int, int>> pairs = {
        { 1, 2 },
        { 1, 3 },
        { 1, 4 },
        { 2, 4 },
        { 3, 4 },
        { 6, 7 },
        { 6, 8 },
        { 7, 8 },
    };

    box.iterate_pairs([&pairs](Particle& p1, Particle& p2) {
        std::tuple<int, int> rm = {
            static_cast<int>(std::min(p1.getType(), p2.getType())),
            static_cast<int>(std::max(p1.getType(), p2.getType())),
        };

        EXPECT_TRUE(std::find(pairs.begin(), pairs.end(), rm) != pairs.end())
            << "Iterated over an illegal pair: (" << std::get<0>(rm) << ", " << std::get<1>(rm) << ")";

        pairs.remove(rm);
    });

    EXPECT_TRUE(pairs.size() == 0) << "The pair size should be 0 but it was " << pairs.size();
}

// Test the box container constructor
TEST(BoxContainer, Constructor) {
    std::vector<Particle> particles = {
        Particle({ 1.3, 2.1, 0.0 }, { 3.0, 2.0, 4.0 }, 0.5, 0),
        Particle({ 0.0, -1.3, -2.0 }, { 1.0, 2.2, -3.2 }, 3.0, 1),
    };

    BoxContainer box(particles, 2.1, { 10.5, 12.2, 10.5 });
    const std::array<double, 3> domain = { 10.5, 12.2, 10.5 };

    EXPECT_EQ(box.getRC(), 2.1) << "The getter for the cutoff distance returned a wrong value";
    EXPECT_TRUE(box.get_corner_vector() == domain) << "The getter for the domain returned a wrong value";

    for (size_t i = 0; i < particles.size(); i++) {
        EXPECT_TRUE(particles[i] == box[i]) << "The particles must maintain their provided order, and all particles must be available.";
    }
}

// Test if update position works correctly
TEST(BoxContainer, UpdatePosition) {
    std::vector<Particle> particles = {
        Particle({ 1.0, 1.0, 0.0 }, {}, 1.0, 1),
        Particle({ 19.0, 1.0, 0.0 }, {}, 1.0, 2),
        Particle({ 1.0, 19.0, 0.0 }, {}, 1.0, 3),
        Particle({ 19.0, 19.0, 0.0 }, {}, 1.0, 4),
    };

    BoxContainer box = BoxContainer(particles, 2.0, { 40.0, 40.0, 10.0 });

    box.iterate_pairs([](Particle& p1, Particle& p2) { EXPECT_TRUE(false) << "All particles are spread out in the initial condition."; });

    std::list<std::tuple<int, int>> pairs = {
        { 1, 2 },
        { 1, 3 },
        { 1, 4 },
        { 2, 3 },
        { 2, 4 },
        { 3, 4 },
    };

    box[0] = Particle({ 9.9, 9.9, 0.0 }, {}, 1.0, 1);
    box[1] = Particle({ 10.1, 9.9, 0.0 }, {}, 1.0, 2);
    box[2] = Particle({ 9.9, 10.1, 0.0 }, {}, 1.0, 3);
    box[3] = Particle({ 10.1, 10.1, 0.0 }, {}, 1.0, 4);

    box.update_positions();

    box.iterate_pairs([&pairs](Particle& p1, Particle& p2) {
        std::tuple<int, int> rm = {
            static_cast<int>(std::min(p1.getType(), p2.getType())),
            static_cast<int>(std::max(p1.getType(), p2.getType())),
        };

        EXPECT_TRUE(std::find(pairs.begin(), pairs.end(), rm) != pairs.end())
            << "Iterated over an illegal pair: (" << std::get<0>(rm) << ", " << std::get<1>(rm) << ")";

        pairs.remove(rm);
    });

    EXPECT_TRUE(pairs.size() == 0) << "The pair size should be 0 but it was " << pairs.size();
}
