#include <boundaries/InfContainer.h>
#include <gtest/gtest.h>

// Test the iterate pairs method for an inf container in a complex simulation setting.
TEST(InfContainer, IteratePairs) {
    std::vector<Particle> particles = {
        Particle({ 0.5, 0.5, 0.5 }, {}, 1.0, 1),
        Particle({ 1.25, 0.5, 0.5 }, {}, 1.0, 2),
        Particle({ 0.5, 1.25, 0.5 }, {}, 1.0, 3),
        Particle({ 1.1, 1.1, 0.5 }, {}, 1.0, 4),
    };

    InfContainer box = InfContainer(particles);

    std::list<std::tuple<int, int>> pairs = {
        { 1, 2 },
        { 1, 3 },
        { 1, 4 },
        { 2, 3 },
        { 2, 4 },
        { 3, 4 },
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
