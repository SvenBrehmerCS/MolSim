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
        // TODO:
    };

    ASSERT_NO_THROW(cells.create_list(particles));

    std::list<std::tuple<int, int>> pairs = {
        // TODO:
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
}

// Test if the loop pair method works correctly for a complex 2D problem
TEST(CellList, LoopPairs2D) {
    CellList cells(1.0, 5, 5, 1);
    std::vector<Particle> particles = {
        // TODO:
    };

    ASSERT_NO_THROW(cells.create_list(particles));

    std::list<std::tuple<int, int>> pairs = {
        // TODO:
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

// TODO: Try testing create_list
