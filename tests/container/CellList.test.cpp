#include <container/CellList.h>
#include <gtest/gtest.h>
#include <tuple>
#include <utils/ArrayUtils.h>

// Test if the loop pair method works correctly for a no particles
TEST(CellList, LoopPairsNoParticle) {
    CellList cells(1.0, { 5.0, 4.0, 3.0 });
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

#pragma omp critical
            pairs.remove(rm);
        },
        particles);

    EXPECT_TRUE(pairs.size() == 0) << "The pair size should be 0 but it was " << pairs.size();
}

// Test if the loop pair method works correctly for a single particle
TEST(CellList, LoopPairsSingleParticle) {
    CellList cells(1.0, { 5.0, 4.0, 3.0 });
    std::vector<Particle> particles = {
        Particle({ 2.0, 1.5, 3.25 }, {}, 1),
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

#pragma omp critical
            pairs.remove(rm);
        },
        particles);

    EXPECT_TRUE(pairs.size() == 0) << "The pair size should be 0 but it was " << pairs.size();
}

// Test if the loop pair method works correctly for a complex 3D problem
TEST(CellList, LoopPairs3D) {
    CellList cells(1.0, { 5.0, 4.0, 3.0 });
    std::vector<Particle> particles = {
        Particle({ 0.9, 2.9, 0.9 }, {}, 1),
        Particle({ 1.1, 2.9, 0.9 }, {}, 2),
        Particle({ 0.9, 3.1, 0.9 }, {}, 3),
        Particle({ 1.1, 3.1, 0.9 }, {}, 4),
        Particle({ 0.9, 2.9, 1.1 }, {}, 5),
        Particle({ 1.1, 2.9, 1.1 }, {}, 6),
        Particle({ 0.9, 3.1, 1.1 }, {}, 7),
        Particle({ 1.1, 3.1, 1.1 }, {}, 8),
        Particle({ 3.4, 1.4, 2.3 }, {}, 9),
        Particle({ 3.4, 1.6, 2.4 }, {}, 10),
        Particle({ 3.6, 1.4, 2.2 }, {}, 11),
        Particle({ 3.6, 1.6, 2.5 }, {}, 12),
        Particle({ 3.25, 3.5, 1.0 }, {}, 13),
        Particle({ 3.75, 3.5, 1.0 }, {}, 14),
        Particle({ 4.5, 3.5, 1.0 }, {}, 15),
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

#pragma omp critical
            pairs.remove(rm);
        },
        particles);

    EXPECT_TRUE(pairs.size() == 0) << "The pair size should be 0 but it was " << pairs.size();
}

// Test that the getters and cell constructor work correctly
TEST(CellList, ConstructorGetter) {
    // Create a cell list with a inner cell count of 10 x 8 x 3
    CellList cells = CellList(3.0, { 30.0, 24.0, 9.0 });
    Vec<double> corner = { 30.0, 24.0, 9.0 };

    EXPECT_EQ(cells.get_cell_index(0, 0, 0), 0) << "The index was computed wrong.";
    EXPECT_EQ(cells.get_cell_index(1, 4, 2), 72) << "The index was computed wrong.";
    EXPECT_EQ(cells.get_cell_index(2, 1, 2), 107) << "The index was computed wrong.";
    EXPECT_LT((cells.get_corner_vector() - corner).len(), 1E-9) << "The corner vector must be computed correctly.";
}

// Test that the create list method works correctly
TEST(CellList, CreateList) {
    CellList cells(2.0, { 40.0, 30.0, 31.0 });
    std::vector<Particle> particles = {
        Particle({ 1.0, 1.0, 1.0 }, {}, 1),
        Particle({ 25.0, 1.0, 1.0 }, {}, 2),
        Particle({ 1.0, 25.0, 1.0 }, {}, 3),
        Particle({ 1.0, 1.0, 25.0 }, {}, 4),
        Particle({ 25.0, 25.0, 1.0 }, {}, 5),
        Particle({ 25.0, 1.0, 25.0 }, {}, 6),
        Particle({ 1.0, 25.0, 25.0 }, {}, 7),
        Particle({ 25.0, 25.0, 25.0 }, {}, 8),
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
        Particle({ 10.0, 10.0, 10.0 }, {}, 1),
        Particle({ 11.0, 10.0, 10.0 }, {}, 2),
        Particle({ 10.0, 11.0, 10.0 }, {}, 3),
        Particle({ 10.0, 10.0, 11.0 }, {}, 4),
        Particle({ 11.0, 11.0, 10.0 }, {}, 5),
        Particle({ 11.0, 10.0, 11.0 }, {}, 6),
        Particle({ 10.0, 11.0, 11.0 }, {}, 7),
        Particle({ 11.0, 11.0, 11.0 }, {}, 8),
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

#pragma omp critical
            pairs.remove(rm);
        },
        particles_new);

    EXPECT_EQ(pairs.size(), 0) << "The pair size should be 0 but it was " << pairs.size();
}

// Test if the iterator for the halo cells work correctly.
TEST(CellList, IterateHalo) {
    CellList cells(2.0, { 12.0, 12.0, 12.0 });
    std::vector<Particle> particles = {
        Particle({ -1.0, -1.0, -1.0 }, {}, 1),
        Particle({ 11.0, 2.0, 4.0 }, {}, 2),
        Particle({ 3.0, 13.0, 8.0 }, {}, 3),
        Particle({ 10.0, 13.0, -1.0 }, {}, 4),
        Particle({ 4.0, 6.0, 1.0 }, {}, 5),
        Particle({ 13, 13.0, 13.0 }, {}, 6),
        Particle({ 10.0, 12.0, 6.0 }, {}, 7),
        Particle({ 3.0, 1.0, 5.0 }, {}, 8),
    };

    ASSERT_NO_THROW(cells.create_list(particles));

    std::list<int> indices = { 1, 3, 4, 6, 7 };

    cells.loop_halo(
        [&indices](Particle& p1) {
            const int rm = p1.getType();
            EXPECT_TRUE(std::find(indices.begin(), indices.end(), rm) != indices.end()) << "Iterated over an illegal particle: " << rm;
            indices.remove(rm);
        },
        particles);

    EXPECT_EQ(indices.size(), 0) << "The indices size should be 0 but it was " << indices.size();
}

// Test if the iterator for the boundary cells work correctly.
TEST(CellList, IterateBoundary) {
    CellList cells(3, { 21.0, 21.0, 21.0 });
    std::vector<Particle> particles = {
        Particle({ -1.0, 1, -1.0 }, {}, 1),
        Particle({ 20.0, 2.0, 2.0 }, {}, 2),
        Particle({ 3.0, 13.0, 8.0 }, {}, 3),
        Particle({ 10.0, 13.0, -1.0 }, {}, 4),
        Particle({ 2.0, 2, 1.0 }, {}, 5),
        Particle({ 13, 13.0, 13.0 }, {}, 6),
        Particle({ 10.0, 20.0, 12.0 }, {}, 7),
        Particle({ 3.0, 1.0, 5.0 }, {}, 8),
    };

    ASSERT_NO_THROW(cells.create_list(particles));

    std::list<int> indices = { 2, 5, 7, 8 };

    cells.loop_boundary(
        [&indices](Particle& p1) {
            const int rm = p1.getType();
            EXPECT_TRUE(std::find(indices.begin(), indices.end(), rm) != indices.end()) << "Iterated over an illegal particle: " << rm;
            indices.remove(rm);
        },
        particles);

    EXPECT_EQ(indices.size(), 0) << "The indices size should be 0 but it was " << indices.size();
}

// Test if the iterator for the inner cells work correctly.
TEST(CellList, IterateInner) {
    CellList cells(3.0, { 21.0, 21.0, 21.0 });
    std::vector<Particle> particles = {
        Particle({ -1.0, -1.0, -1.0 }, {}, 1),
        Particle({ 23.0, 2.0, 4.0 }, {}, 2),
        Particle({ 3.0, 13.0, 8.0 }, {}, 3),
        Particle({ 10.0, 13.0, -1.0 }, {}, 4),
        Particle({ 4.0, 6.0, 1.0 }, {}, 5),
        Particle({ 13, 13.0, 13.0 }, {}, 6),
        Particle({ 10.0, 23.0, 12.0 }, {}, 7),
        Particle({ 3.0, 1.0, 5.0 }, {}, 8),
    };

    ASSERT_NO_THROW(cells.create_list(particles));

    std::list<int> indices = { 3, 5, 6, 8 };

    cells.loop_inner(
        [&indices](Particle& p1) {
            const int rm = p1.getType();
            EXPECT_TRUE(std::find(indices.begin(), indices.end(), rm) != indices.end()) << "Iterated over an illegal particle: " << rm;
            indices.remove(rm);
        },
        particles);

    EXPECT_EQ(indices.size(), 0) << "The indices size should be 0 but it was " << indices.size();
}

// Test if looping through the xy plains works correctly. This test tests the cutoff distance, and all possible iteration configurations.
TEST(CellList, IterateXYPlain) {
    CellList cells(2.0, { 16.0, 16.0, 16.0 });

    std::vector<Particle> particles = {
        Particle({ 13.0, 3.0, 0.1 }, {}, 1),
        Particle({ 11.9, 4.1, 15.9 }, {}, 2),
        Particle({ 11.9, 3.0, 15.9 }, {}, 3),
        Particle({ 11.9, 1.9, 15.9 }, {}, 4),
        Particle({ 13.0, 4.1, 15.9 }, {}, 5),
        Particle({ 13.0, 3.0, 15.9 }, {}, 6),
        Particle({ 13.0, 1.9, 15.9 }, {}, 7),
        Particle({ 14.1, 4.1, 15.9 }, {}, 8),
        Particle({ 14.1, 3.0, 15.9 }, {}, 9),
        Particle({ 14.1, 1.9, 15.9 }, {}, 10),
        Particle({ 13.0, 4.2, 15.9 }, {}, 11),
        Particle({ 14.2, 3.0, 15.9 }, {}, 12),
        Particle({ 10.1, 4.1, 15.9 }, {}, 13),
        Particle({ 10.1, 3.0, 15.9 }, {}, 14),
        Particle({ 10.1, 1.9, 15.9 }, {}, 15),
        Particle({ 13.0, 4.1, 14.01 }, {}, 16),
        Particle({ 13.0, 3.0, 14.01 }, {}, 17),
        Particle({ 13.0, 1.9, 14.01 }, {}, 18),
        Particle({ 15.9, 4.1, 15.9 }, {}, 19),
        Particle({ 15.9, 3.0, 15.9 }, {}, 20),
        Particle({ 15.9, 1.9, 15.9 }, {}, 21),
        Particle({ 8.0, 8.0, 8.0 }, {}, 22),
        Particle({ 8.2, 8.2, 8.2 }, {}, 23),
    };

    std::list<std::tuple<int, int>> pairs = {
        { 1, 2 },
        { 1, 3 },
        { 1, 4 },
        { 1, 5 },
        { 1, 6 },
        { 1, 7 },
        { 1, 8 },
        { 1, 9 },
        { 1, 10 },
        { 1, 11 },
        { 1, 12 },
    };

    cells.create_list(particles);

    cells.loop_xy_pairs(
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

    EXPECT_EQ(pairs.size(), 0) << "The pair size should be 0 but it was " << pairs.size();
}

// Test if looping through the xz plains works correctly. This test tests the cutoff distance, and all possible iteration configurations.
TEST(CellList, IterateXZPlain) {
    CellList cells(2.0, { 16.0, 16.0, 16.0 });

    std::vector<Particle> particles = {
        Particle({ 13.0, 0.1, 3.0 }, {}, 1),
        Particle({ 11.9, 15.9, 4.1 }, {}, 2),
        Particle({ 11.9, 15.9, 3.0 }, {}, 3),
        Particle({ 11.9, 15.9, 1.9 }, {}, 4),
        Particle({ 13.0, 15.9, 4.1 }, {}, 5),
        Particle({ 13.0, 15.9, 3.0 }, {}, 6),
        Particle({ 13.0, 15.9, 1.9 }, {}, 7),
        Particle({ 14.1, 15.9, 4.1 }, {}, 8),
        Particle({ 14.1, 15.9, 3.0 }, {}, 9),
        Particle({ 14.1, 15.9, 1.9 }, {}, 10),
        Particle({ 13.0, 15.9, 4.2 }, {}, 11),
        Particle({ 14.2, 15.9, 3.0 }, {}, 12),
        Particle({ 10.1, 15.9, 4.1 }, {}, 13),
        Particle({ 10.1, 15.9, 3.0 }, {}, 14),
        Particle({ 10.1, 15.9, 1.9 }, {}, 15),
        Particle({ 13.0, 14.01, 4.1 }, {}, 16),
        Particle({ 13.0, 14.01, 3.0 }, {}, 17),
        Particle({ 13.0, 14.01, 1.9 }, {}, 18),
        Particle({ 15.9, 15.9, 4.1 }, {}, 19),
        Particle({ 15.9, 15.9, 3.0 }, {}, 20),
        Particle({ 15.9, 15.9, 1.9 }, {}, 21),
        Particle({ 8.0, 8.0, 8.0 }, {}, 22),
        Particle({ 8.2, 8.2, 8.2 }, {}, 23),
    };

    std::list<std::tuple<int, int>> pairs = {
        { 1, 2 },
        { 1, 3 },
        { 1, 4 },
        { 1, 5 },
        { 1, 6 },
        { 1, 7 },
        { 1, 8 },
        { 1, 9 },
        { 1, 10 },
        { 1, 11 },
        { 1, 12 },
    };

    cells.create_list(particles);

    cells.loop_xz_pairs(
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

    EXPECT_EQ(pairs.size(), 0) << "The pair size should be 0 but it was " << pairs.size();
}

// Test if looping through the yz plains works correctly. This test tests the cutoff distance, and all possible iteration configurations.
TEST(CellList, IterateYZPlain) {
    CellList cells(2.0, { 16.0, 16.0, 16.0 });

    std::vector<Particle> particles = {
        Particle({ 0.1, 13.0, 3.0 }, {}, 1),
        Particle({ 15.9, 11.9, 4.1 }, {}, 2),
        Particle({ 15.9, 11.9, 3.0 }, {}, 3),
        Particle({ 15.9, 11.9, 1.9 }, {}, 4),
        Particle({ 15.9, 13.0, 4.1 }, {}, 5),
        Particle({ 15.9, 13.0, 3.0 }, {}, 6),
        Particle({ 15.9, 13.0, 1.9 }, {}, 7),
        Particle({ 15.9, 14.1, 4.1 }, {}, 8),
        Particle({ 15.9, 14.1, 3.0 }, {}, 9),
        Particle({ 15.9, 14.1, 1.9 }, {}, 10),
        Particle({ 15.9, 13.0, 4.2 }, {}, 11),
        Particle({ 15.9, 14.2, 3.0 }, {}, 12),
        Particle({ 15.9, 10.1, 4.1 }, {}, 13),
        Particle({ 15.9, 10.1, 3.0 }, {}, 14),
        Particle({ 15.9, 10.1, 1.9 }, {}, 15),
        Particle({ 14.01, 13.0, 4.1 }, {}, 16),
        Particle({ 14.01, 13.0, 3.0 }, {}, 17),
        Particle({ 14.01, 13.0, 1.9 }, {}, 18),
        Particle({ 15.9, 15.9, 4.1 }, {}, 19),
        Particle({ 15.9, 15.9, 3.0 }, {}, 20),
        Particle({ 15.9, 15.9, 1.9 }, {}, 21),
        Particle({ 8.0, 8.0, 8.0 }, {}, 22),
        Particle({ 8.2, 8.2, 8.2 }, {}, 23),
    };

    std::list<std::tuple<int, int>> pairs = {
        { 1, 2 },
        { 1, 3 },
        { 1, 4 },
        { 1, 5 },
        { 1, 6 },
        { 1, 7 },
        { 1, 8 },
        { 1, 9 },
        { 1, 10 },
        { 1, 11 },
        { 1, 12 },
    };

    cells.create_list(particles);

    cells.loop_yz_pairs(
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

    EXPECT_EQ(pairs.size(), 0) << "The pair size should be 0 but it was " << pairs.size();
}

// Test if looping through the near x axis works correctly. This test tests the cutoff distance, and all possible iteration configurations.
TEST(CellList, IterateNearXAxis) {
    CellList cells(2.0, { 16.0, 16.0, 16.0 });

    std::vector<Particle> particles = {
        Particle({ 7.0, 0.1, 0.1 }, {}, 1),
        Particle({ 5.9, 15.9, 15.9 }, {}, 2),
        Particle({ 7.0, 15.9, 15.9 }, {}, 3),
        Particle({ 8.1, 15.9, 15.9 }, {}, 4),
        Particle({ 7.1, 15.9, 15.9 }, {}, 5),
        Particle({ 8.2, 15.9, 15.9 }, {}, 6),
        Particle({ 4.1, 15.9, 15.9 }, {}, 7),
        Particle({ 7.0, 14.1, 14.1 }, {}, 8),
        Particle({ 9.9, 15.9, 15.9 }, {}, 9),
    };

    std::list<std::tuple<int, int>> pairs = {
        { 1, 2 },
        { 1, 3 },
        { 1, 4 },
        { 1, 5 },
        { 1, 6 },
    };

    cells.create_list(particles);

    cells.loop_x_near(
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

    EXPECT_EQ(pairs.size(), 0) << "The pair size should be 0 but it was " << pairs.size();
}

// Test if looping through the far x axis works correctly. This test tests the cutoff distance, and all possible iteration configurations.
TEST(CellList, IterateFarXAxis) {
    CellList cells(2.0, { 16.0, 16.0, 16.0 });

    std::vector<Particle> particles = {
        Particle({ 7.0, 0.1, 15.9 }, {}, 1),
        Particle({ 5.9, 15.9, 0.1 }, {}, 2),
        Particle({ 7.0, 15.9, 0.1 }, {}, 3),
        Particle({ 8.1, 15.9, 0.1 }, {}, 4),
        Particle({ 7.1, 15.9, 0.1 }, {}, 5),
        Particle({ 8.2, 15.9, 0.1 }, {}, 6),
        Particle({ 4.1, 15.9, 0.1 }, {}, 7),
        Particle({ 7.0, 14.1, 1.9 }, {}, 8),
        Particle({ 9.9, 15.9, 0.1 }, {}, 9),
        Particle({ 8.0, 8.0, 8.0 }, {}, 10),
        Particle({ 8.2, 8.2, 8.2 }, {}, 11),
    };

    std::list<std::tuple<int, int>> pairs = {
        { 1, 2 },
        { 1, 3 },
        { 1, 4 },
        { 1, 5 },
        { 1, 6 },
    };

    cells.create_list(particles);

    cells.loop_x_far(
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

    EXPECT_EQ(pairs.size(), 0) << "The pair size should be 0 but it was " << pairs.size();
}

// Test if looping through the near y axis works correctly. This test tests the cutoff distance, and all possible iteration configurations.
TEST(CellList, IterateNearYAxis) {
    CellList cells(2.0, { 16.0, 16.0, 16.0 });

    std::vector<Particle> particles = {
        Particle({ 0.1, 7.0, 0.1 }, {}, 1),
        Particle({ 15.9, 5.9, 15.9 }, {}, 2),
        Particle({ 15.9, 7.0, 15.9 }, {}, 3),
        Particle({ 15.9, 8.1, 15.9 }, {}, 4),
        Particle({ 15.9, 7.1, 15.9 }, {}, 5),
        Particle({ 15.9, 8.2, 15.9 }, {}, 6),
        Particle({ 15.9, 4.1, 15.9 }, {}, 7),
        Particle({ 14.1, 7.0, 14.1 }, {}, 8),
        Particle({ 15.9, 9.9, 15.9 }, {}, 9),
        Particle({ 8.0, 8.0, 8.0 }, {}, 10),
        Particle({ 8.2, 8.2, 8.2 }, {}, 11),
    };

    std::list<std::tuple<int, int>> pairs = {
        { 1, 2 },
        { 1, 3 },
        { 1, 4 },
        { 1, 5 },
        { 1, 6 },
    };

    cells.create_list(particles);

    cells.loop_y_near(
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

    EXPECT_EQ(pairs.size(), 0) << "The pair size should be 0 but it was " << pairs.size();
}

// Test if looping through the far y axis works correctly. This test tests the cutoff distance, and all possible iteration configurations.
TEST(CellList, IterateFarYAxis) {
    CellList cells(2.0, { 16.0, 16.0, 16.0 });

    std::vector<Particle> particles = {
        Particle({ 0.1, 7.0, 15.9 }, {}, 1),
        Particle({ 15.9, 5.9, 0.1 }, {}, 2),
        Particle({ 15.9, 7.0, 0.1 }, {}, 3),
        Particle({ 15.9, 8.1, 0.1 }, {}, 4),
        Particle({ 15.9, 7.1, 0.1 }, {}, 5),
        Particle({ 15.9, 8.2, 0.1 }, {}, 6),
        Particle({ 15.9, 4.1, 0.1 }, {}, 7),
        Particle({ 14.1, 7.0, 1.9 }, {}, 8),
        Particle({ 15.9, 9.9, 0.1 }, {}, 9),
        Particle({ 8.0, 8.0, 8.0 }, {}, 10),
        Particle({ 8.2, 8.2, 8.2 }, {}, 11),
    };

    std::list<std::tuple<int, int>> pairs = {
        { 1, 2 },
        { 1, 3 },
        { 1, 4 },
        { 1, 5 },
        { 1, 6 },
    };

    cells.create_list(particles);

    cells.loop_y_far(
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

    EXPECT_EQ(pairs.size(), 0) << "The pair size should be 0 but it was " << pairs.size();
}

// Test if looping through the near z axis works correctly. This test tests the cutoff distance, and all possible iteration configurations.
TEST(CellList, IterateNearZAxis) {
    CellList cells(2.0, { 16.0, 16.0, 16.0 });

    std::vector<Particle> particles = {
        Particle({ 0.1, 0.1, 7.0 }, {}, 1),
        Particle({ 15.9, 15.9, 5.9 }, {}, 2),
        Particle({ 15.9, 15.9, 7.0 }, {}, 3),
        Particle({ 15.9, 15.9, 8.1 }, {}, 4),
        Particle({ 15.9, 15.9, 7.1 }, {}, 5),
        Particle({ 15.9, 15.9, 8.2 }, {}, 6),
        Particle({ 15.9, 15.9, 4.1 }, {}, 7),
        Particle({ 14.1, 14.1, 7.0 }, {}, 8),
        Particle({ 15.9, 15.9, 9.9 }, {}, 9),
        Particle({ 8.0, 8.0, 8.0 }, {}, 10),
        Particle({ 8.2, 8.2, 8.2 }, {}, 11),
    };

    std::list<std::tuple<int, int>> pairs = {
        { 1, 2 },
        { 1, 3 },
        { 1, 4 },
        { 1, 5 },
        { 1, 6 },
    };

    cells.create_list(particles);

    cells.loop_z_near(
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

    EXPECT_EQ(pairs.size(), 0) << "The pair size should be 0 but it was " << pairs.size();
}

// Test if looping through the far z axis works correctly. This test tests the cutoff distance, and all possible iteration configurations.
TEST(CellList, IterateFarZAxis) {
    CellList cells(2.0, { 16.0, 16.0, 16.0 });

    std::vector<Particle> particles = {
        Particle({ 0.1, 15.9, 7.0 }, {}, 1),
        Particle({ 15.9, 0.1, 5.9 }, {}, 2),
        Particle({ 15.9, 0.1, 7.0 }, {}, 3),
        Particle({ 15.9, 0.1, 8.1 }, {}, 4),
        Particle({ 15.9, 0.1, 7.1 }, {}, 5),
        Particle({ 15.9, 0.1, 8.2 }, {}, 6),
        Particle({ 15.9, 0.1, 4.1 }, {}, 7),
        Particle({ 14.1, 1.9, 7.0 }, {}, 8),
        Particle({ 15.9, 0.1, 9.9 }, {}, 9),
        Particle({ 8.0, 8.0, 8.0 }, {}, 10),
        Particle({ 8.2, 8.2, 8.2 }, {}, 11),
    };

    std::list<std::tuple<int, int>> pairs = {
        { 1, 2 },
        { 1, 3 },
        { 1, 4 },
        { 1, 5 },
        { 1, 6 },
    };

    cells.create_list(particles);

    cells.loop_z_far(
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

    EXPECT_EQ(pairs.size(), 0) << "The pair size should be 0 but it was " << pairs.size();
}

// Test if looping through the origin cells works correctly. This test tests the cutoff distance, and all possible iteration configurations.
TEST(CellList, IterateOriginCell) {
    CellList cells(2.0, { 16.0, 16.0, 16.0 });

    std::vector<Particle> particles = {
        Particle({ 0.1, 0.1, 0.1 }, {}, 1),
        Particle({ 15.9, 15.9, 15.9 }, {}, 2),
        Particle({ 15.0, 15.0, 15.0 }, {}, 3),
        Particle({ 1.0, 1.0, 1.0 }, {}, 4),
        Particle({ 15.0, 5.0, 15.0 }, {}, 5),
        Particle({ 15.0, 4.0, 15.0 }, {}, 6),
    };

    std::list<std::tuple<int, int>> pairs = {
        { 1, 2 },
        { 1, 3 },
        { 2, 4 },
    };

    cells.create_list(particles);

    cells.loop_origin_corner(
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

    EXPECT_EQ(pairs.size(), 0) << "The pair size should be 0 but it was " << pairs.size();
}

// Test if looping through the x cells works correctly. This test tests the cutoff distance, and all possible iteration configurations.
TEST(CellList, IterateXCell) {
    CellList cells(2.0, { 16.0, 16.0, 16.0 });

    std::vector<Particle> particles = {
        Particle({ 15.9, 0.1, 0.1 }, {}, 1),
        Particle({ 0.1, 15.9, 15.9 }, {}, 2),
        Particle({ 1.0, 15.0, 15.0 }, {}, 3),
        Particle({ 15.0, 1.0, 1.0 }, {}, 4),
        Particle({ 15.0, 5.0, 15.0 }, {}, 5),
        Particle({ 15.0, 4.0, 15.0 }, {}, 6),
    };

    std::list<std::tuple<int, int>> pairs = {
        { 1, 2 },
        { 1, 3 },
        { 2, 4 },
    };

    cells.create_list(particles);

    cells.loop_x_corner(
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

    EXPECT_EQ(pairs.size(), 0) << "The pair size should be 0 but it was " << pairs.size();
}

// Test if looping through the y cells works correctly. This test tests the cutoff distance, and all possible iteration configurations.
TEST(CellList, IterateYCell) {
    CellList cells(2.0, { 16.0, 16.0, 16.0 });

    std::vector<Particle> particles = {
        Particle({ 0.1, 15.9, 0.1 }, {}, 1),
        Particle({ 15.9, 0.1, 15.9 }, {}, 2),
        Particle({ 15.0, 1.0, 15.0 }, {}, 3),
        Particle({ 1.0, 15.0, 1.0 }, {}, 4),
        Particle({ 15.0, 5.0, 15.0 }, {}, 5),
        Particle({ 15.0, 4.0, 15.0 }, {}, 6),
    };

    std::list<std::tuple<int, int>> pairs = {
        { 1, 2 },
        { 1, 3 },
        { 2, 4 },
    };

    cells.create_list(particles);

    cells.loop_y_corner(
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

    EXPECT_EQ(pairs.size(), 0) << "The pair size should be 0 but it was " << pairs.size();
}

// Test if looping through the xy cells works correctly. This test tests the cutoff distance, and all possible iteration configurations.
TEST(CellList, IterateXYCell) {
    CellList cells(2.0, { 16.0, 16.0, 16.0 });

    std::vector<Particle> particles = {
        Particle({ 15.9, 15.9, 0.1 }, {}, 1),
        Particle({ 0.1, 0.1, 15.9 }, {}, 2),
        Particle({ 1.0, 1.0, 15.0 }, {}, 3),
        Particle({ 15.0, 15.0, 1.0 }, {}, 4),
        Particle({ 15.0, 5.0, 15.0 }, {}, 5),
        Particle({ 15.0, 4.0, 15.0 }, {}, 6),
    };

    std::list<std::tuple<int, int>> pairs = {
        { 1, 2 },
        { 1, 3 },
        { 2, 4 },
    };

    cells.create_list(particles);

    cells.loop_xy_corner(
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

    EXPECT_EQ(pairs.size(), 0) << "The pair size should be 0 but it was " << pairs.size();
}
