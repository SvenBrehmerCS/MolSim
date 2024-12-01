#include <boundaries/CellList.h>
#include <gtest/gtest.h>
#include <tuple>
#include <utils/ArrayUtils.h>

// Test if the loop pair method works correctly
TEST(CellList, LoopPairs1) {
    CellList cells(1.0, 5, 5, 1);
    std::vector<Particle> particles = {
        Particle({ 0.25, 0.25, 0.5 }, {}, 1.0, 1),
        Particle({ 0.75, 0.75, 0.5 }, {}, 1.0, 2),
        Particle({ 1.25, 0.25, 0.5 }, {}, 1.0, 3),
        Particle({ 1.75, 0.75, 0.5 }, {}, 1.0, 4),
        Particle({ 1.25, 1.25, 0.5 }, {}, 1.0, 5),
        Particle({ 2.5, 2.5, 0.5 }, {}, 1.0, 6),
        Particle({ 4.75, 3.25, 0.5 }, {}, 1.0, 7),
        Particle({ 4.25, 3.75, 0.5 }, {}, 1.0, 8),
        Particle({ 4.5, 4.1, 0.5 }, {}, 1.0, 9),
        Particle({ 4.1, 0.1, 0.5 }, {}, 1.0, 10),
        Particle({ 4.9, 0.9, 0.5 }, {}, 1.0, 11),
    };

    ASSERT_NO_THROW(cells.create_list(particles));

    std::list<std::tuple<int, int>> pairs = {
        { 1, 2 },
        { 1, 3 },
        { 2, 3 },
        { 2, 4 },
        { 2, 5 },
        { 3, 4 },
        { 3, 5 },
        { 4, 5 },
        { 7, 8 },
        { 7, 9 },
        { 8, 9 },
    };

    cells.loop_cell_pairs(
        [&pairs](Particle& p1, Particle& p2) {
            std::tuple<int, int> rm = {
                static_cast<int>(std::min(p1.getType(), p2.getType())),
                static_cast<int>(std::max(p1.getType(), p2.getType())),
            };

            EXPECT_TRUE(std::find(pairs.begin(), pairs.end(), rm) != pairs.end())
                << "Iterated over an illegal pair: (" << std::get<0>(rm) << ", " << std::get<1>(rm) << ")";

            pairs.remove(rm);
        },
        particles);

    EXPECT_TRUE(pairs.size() == 0) << "The pair size should be 0 but it was " << pairs.size();

    for (auto p : pairs) {
        std::cout << "(" << std::get<0>(p) << ", " << std::get<1>(p) << ")\n";
    }
}
