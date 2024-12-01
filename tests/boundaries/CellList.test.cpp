#include <boundaries/CellList.h>
#include <gtest/gtest.h>
#include <tuple>
#include <utils/ArrayUtils.h>

// Test if the loop pair method works correctly for a no particles
TEST(CellList, LoopPairsNoParticle) {
    CellList cells(1.0, 5, 4, 3);
    std::vector<Particle> particles = {};

    ASSERT_NO_THROW(cells.create_list(particles));

    std::list<std::tuple<int, int>> pairs = {};

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
}

// Test if the loop pair method works correctly for a single particle
TEST(CellList, LoopPairsSingleParticle) {
    CellList cells(1.0, 5, 4, 3);
    std::vector<Particle> particles = {
        Particle({ 2.0, 1.5, 3.25 }, {}, 1.0, 1),
    };

    ASSERT_NO_THROW(cells.create_list(particles));

    std::list<std::tuple<int, int>> pairs = {};

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
}

// Test if the loop pair method works correctly for a complex 3D problem
TEST(CellList, LoopPairs3D) {
    CellList cells(1.0, 5, 4, 3);
    std::vector<Particle> particles = {
        Particle({ 0.9, 2.9, 0.9 }, {}, 1.0, 1),
        Particle({ 1.1, 2.9, 0.9 }, {}, 1.0, 2),
        Particle({ 0.9, 3.1, 0.9 }, {}, 1.0, 3),
        Particle({ 1.1, 3.1, 0.9 }, {}, 1.0, 4),
        Particle({ 0.9, 2.9, 1.1 }, {}, 1.0, 5),
        Particle({ 1.1, 2.9, 1.1 }, {}, 1.0, 6),
        Particle({ 0.9, 3.1, 1.1 }, {}, 1.0, 7),
        Particle({ 1.1, 3.1, 1.1 }, {}, 1.0, 8),
        Particle({ 3.4, 1.4, 2.3 }, {}, 2.0, 9),
        Particle({ 3.4, 1.6, 2.4 }, {}, 2.0, 10),
        Particle({ 3.6, 1.4, 2.2 }, {}, 2.0, 11),
        Particle({ 3.6, 1.6, 2.5 }, {}, 2.0, 12),
        Particle({ 3.25, 3.5, 1.0 }, {}, 3.0, 13),
        Particle({ 3.75, 3.5, 1.0 }, {}, 3.0, 14),
        Particle({ 4.5, 3.5, 1.0 }, {}, 3.0, 15),
    };

    ASSERT_NO_THROW(cells.create_list(particles));

    std::list<std::tuple<int, int>> pairs = {
        { 1, 2 },
        { 1, 3 },
        { 1, 4 },
        { 1, 5 },
        { 1, 6 },
        { 1, 7 },
        { 1, 8 },
        { 2, 3 },
        { 2, 4 },
        { 2, 5 },
        { 2, 6 },
        { 2, 7 },
        { 2, 8 },
        { 3, 4 },
        { 3, 5 },
        { 3, 6 },
        { 3, 7 },
        { 3, 8 },
        { 4, 5 },
        { 4, 6 },
        { 4, 7 },
        { 4, 8 },
        { 5, 6 },
        { 5, 7 },
        { 5, 8 },
        { 6, 7 },
        { 6, 8 },
        { 7, 8 },
        { 9, 10 },
        { 9, 11 },
        { 9, 12 },
        { 10, 11 },
        { 10, 12 },
        { 11, 12 },
        { 13, 14 },
        { 14, 15 },
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

    for (auto x : pairs) {
        std::cout << "(" << std::get<0>(x) << ", " << std::get<1>(x) << ")\n";
    }
}

// Test that the getters and cell constructor work correctly
TEST(CellList, ConstructorGetter) {
    // Create a cell list with a inner cell count of 10 x 8 x 3
    CellList cells = CellList(3.0, 10, 8, 3);
    std::array<double, 3> corner = { 30.0, 24.0, 9.0 };

    EXPECT_EQ(cells.get_cell_index(0, 0, 0), 0) << "The index was computed wrong.";
    EXPECT_EQ(cells.get_cell_index(1, 4, 2), 72) << "The index was computed wrong.";
    EXPECT_EQ(cells.get_cell_index(2, 1, 2), 107) << "The index was computed wrong.";
    EXPECT_LT(ArrayUtils::L2Norm(cells.get_corner_vector() - corner), 1E-9) << "The corner vector must be computed correctly.";
}

// Test that the create list method works correctly
TEST(CellList, CreateList) {
    CellList cells(2.0, 20, 15, 15);
    std::vector<Particle> particles = {
        Particle({ 1.0, 1.0, 1.0 }, {}, 1.0, 1),
        Particle({ 25.0, 1.0, 1.0 }, {}, 1.0, 2),
        Particle({ 1.0, 25.0, 1.0 }, {}, 1.0, 3),
        Particle({ 1.0, 1.0, 25.0 }, {}, 1.0, 4),
        Particle({ 25.0, 25.0, 1.0 }, {}, 1.0, 5),
        Particle({ 25.0, 1.0, 25.0 }, {}, 1.0, 6),
        Particle({ 1.0, 25.0, 25.0 }, {}, 1.0, 7),
        Particle({ 25.0, 25.0, 25.0 }, {}, 1.0, 8),
    };

    ASSERT_NO_THROW(cells.create_list(particles));

    cells.loop_cell_pairs(
        [](Particle& p1, Particle& p2) { EXPECT_TRUE(false) << "All particles are spread out in the initial condition."; }, particles);

    std::list<std::tuple<int, int>> pairs = {
        { 1, 2 },
        { 1, 3 },
        { 1, 4 },
        { 1, 5 },
        { 1, 6 },
        { 1, 7 },
        { 1, 8 },
        { 2, 3 },
        { 2, 4 },
        { 2, 5 },
        { 2, 6 },
        { 2, 7 },
        { 2, 8 },
        { 3, 4 },
        { 3, 5 },
        { 3, 6 },
        { 3, 7 },
        { 3, 8 },
        { 4, 5 },
        { 4, 6 },
        { 4, 7 },
        { 4, 8 },
        { 5, 6 },
        { 5, 7 },
        { 5, 8 },
        { 6, 7 },
        { 6, 8 },
        { 7, 8 },
    };

    std::vector<Particle> particles_new = {
        Particle({ 10.0, 10.0, 10.0 }, {}, 1.0, 1),
        Particle({ 11.0, 10.0, 10.0 }, {}, 1.0, 2),
        Particle({ 10.0, 11.0, 10.0 }, {}, 1.0, 3),
        Particle({ 10.0, 10.0, 11.0 }, {}, 1.0, 4),
        Particle({ 11.0, 11.0, 10.0 }, {}, 1.0, 5),
        Particle({ 11.0, 10.0, 11.0 }, {}, 1.0, 6),
        Particle({ 10.0, 11.0, 11.0 }, {}, 1.0, 7),
        Particle({ 11.0, 11.0, 11.0 }, {}, 1.0, 8),
    };

    cells.create_list(particles_new);

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
        particles_new);

    EXPECT_TRUE(pairs.size() == 0) << "The pair size should be 0 but it was " << pairs.size();
}
