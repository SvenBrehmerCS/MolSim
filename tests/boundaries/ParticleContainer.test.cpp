
#include <boundaries/ParticleContainer.h>
#include <gtest/gtest.h>

/**
 * @class TestContainer
 *
 * @brief Define a particle container that can be used for testing the particle container methods.
 */
class TestContainer : public ParticleContainer {
public:
    /**
     * Define the default infinity container constructor.
     */
    TestContainer()
        : ParticleContainer() { }

    /**
     * Define the infinity container constructor, using a particle vector.
     *
     * @param new_particles The particles vector.
     */
    TestContainer(std::vector<Particle> new_particles)
        : ParticleContainer(new_particles) { }

    /**
     * Define the default infinity container constructor.
     */
    ~TestContainer() = default;

    /**
     * Iterate over all pairs and apply the provided method to both particles of the pair.
     *
     * @param iterator The iterator lambda that should loop over all the pairs.
     */
    void iterate_pairs(std::function<particle_pair_it> iterator) { (void)iterator; }

    /**
     * Update the particle positions in their cells.
     */
    void update_positions() { }
};

// Test if the particle container constructor works correctly
TEST(ParticleContainerConstructor, EmptyInitialization) {
    TestContainer pc = TestContainer();

    ASSERT_TRUE(pc.size() == 0) << "The particle container must be constructed with no particles.";
}

// Test if the particle container constructor works correctly
TEST(ParticleContainerConstructor, CreateFromVector) {
    std::vector<Particle> pv = {
        Particle({ 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, 1.0, 1),
        Particle({ -1.0, 0.0, 0.0 }, { 0.0, -1.0, 0.0 }, 1.0, 1),
        Particle({ 0.0, 1.0, 0.0 }, { 1.0, 0.0, 0.0 }, 1.0, 1),
        Particle({ 0.0, -1.0, 0.0 }, { -1.0, 0.0, 0.0 }, 1.0, 1),
        Particle({ 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, 0.75, 2),
    };

    TestContainer pc = TestContainer(pv);

    auto pi = pc.begin();
    for (const auto& p : pv) {
        ASSERT_TRUE(*(pi++) == p) << "The particle container must be constructed from the correct particles.";
    }
}

// Test if the particle container [] operator works correctly
TEST(ParticleContainerIndexOperator, CorrectAccess) {
    std::vector<Particle> pv = {
        Particle({ 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, 1.0, 1),
        Particle({ -1.0, 0.0, 0.0 }, { 0.0, -1.0, 0.0 }, 1.0, 1),
        Particle({ 0.0, 1.0, 0.0 }, { 1.0, 0.0, 0.0 }, 1.0, 1),
        Particle({ 0.0, -1.0, 0.0 }, { -1.0, 0.0, 0.0 }, 1.0, 1),
        Particle({ 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, 0.75, 2),
    };

    TestContainer pc = TestContainer(pv);

    ASSERT_NO_THROW(Particle p = pc[0]);

    for (size_t i = 0; i < pv.size(); i++) {
        ASSERT_TRUE(pv[i] == pc[i]) << "The particle container must map correctly to the underlying data structure.";
    }
}

// Test if the begin function of ParticleContainer equals that of the underlying data structure
TEST(ParticleContainerIterator, CorrectBegin) {
    std::vector<Particle> pv = {
        Particle({ 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, 1.0, 1),
        Particle({ -1.0, 0.0, 0.0 }, { 0.0, -1.0, 0.0 }, 1.0, 1),
        Particle({ 0.0, 1.0, 0.0 }, { 1.0, 0.0, 0.0 }, 1.0, 1),
        Particle({ 0.0, -1.0, 0.0 }, { -1.0, 0.0, 0.0 }, 1.0, 1),
        Particle({ 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, 0.75, 2),
    };

    TestContainer pc = TestContainer(pv);

    std::vector<Particle>::iterator pci;
    ASSERT_NO_THROW(pci = pc.begin());

    ASSERT_TRUE(pci->toString() == pv.begin()->toString()) << "The begin function must return the same iterator as the begin function of vector.";
}

// Test if the end function of ParticleContainer equals that of the underlying data structure
TEST(ParticleContainerIterator, CorrectEnd) {
    std::vector<Particle> pv = {
        Particle({ 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, 1.0, 1),
        Particle({ -1.0, 0.0, 0.0 }, { 0.0, -1.0, 0.0 }, 1.0, 1),
        Particle({ 0.0, 1.0, 0.0 }, { 1.0, 0.0, 0.0 }, 1.0, 1),
        Particle({ 0.0, -1.0, 0.0 }, { -1.0, 0.0, 0.0 }, 1.0, 1),
        Particle({ 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, 0.75, 2),
    };

    TestContainer pc = TestContainer(pv);

    std::vector<Particle>::iterator pci;
    ASSERT_NO_THROW(pci = pc.end());

    ASSERT_TRUE((--pci)->toString() == (--pv.end())->toString()) << "The end function must return the same iterator as the end function of vector.";
}

// Test if the iterator of ParticleContainer equals that of the underlying data structure
TEST(ParticleContainerIterator, CorrectIterator) {
    std::vector<Particle> pv = {
        Particle({ 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, 1.0, 1),
        Particle({ -1.0, 0.0, 0.0 }, { 0.0, -1.0, 0.0 }, 1.0, 1),
        Particle({ 0.0, 1.0, 0.0 }, { 1.0, 0.0, 0.0 }, 1.0, 1),
        Particle({ 0.0, -1.0, 0.0 }, { -1.0, 0.0, 0.0 }, 1.0, 1),
        Particle({ 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, 0.75, 2),
    };
    TestContainer pc = TestContainer(pv);

    std::vector<Particle>::const_iterator pci;
    ASSERT_NO_THROW(pci = pc.end());
    ASSERT_NO_THROW(pci = pc.begin());

    for (auto& p : pv) {
        ASSERT_TRUE(*(pci++) == p) << "ParticleContainer should use the same iterator as the underlying data structure.";
    }
}

// Test if the constant iterator of ParticleContainer equals that of the underlying data structure
TEST(ParticleContainerIterator, CorrectConstIterator) {
    std::vector<Particle> pv = {
        Particle({ 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, 1.0, 1),
        Particle({ -1.0, 0.0, 0.0 }, { 0.0, -1.0, 0.0 }, 1.0, 1),
        Particle({ 0.0, 1.0, 0.0 }, { 1.0, 0.0, 0.0 }, 1.0, 1),
        Particle({ 0.0, -1.0, 0.0 }, { -1.0, 0.0, 0.0 }, 1.0, 1),
        Particle({ 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, 0.75, 2),
    };

    TestContainer pc = TestContainer(pv);

    std::vector<Particle>::const_iterator pci;
    ASSERT_NO_THROW(pci = pc.end());
    EXPECT_TRUE((--pci)->toString() == (--pv.end())->toString()) << "The end function must return the same iterator as the end function of vector.";
    ASSERT_NO_THROW(pci = pc.begin());
    EXPECT_TRUE(pci->toString() == pv.begin()->toString()) << "The begin function must return the same iterator as the begin function of vector.";


    for (const auto& p : pc) {
        ASSERT_TRUE(*(pci++) == p) << "ParticleContainer should use the same constant iterator as the underlying data structure.";
    }
}

// Test if the size function works correctly
TEST(ParticleContainerSize, CorrectSize) {
    std::vector<Particle> pv = {
        Particle({ 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, 1.0, 1),
        Particle({ -1.0, 0.0, 0.0 }, { 0.0, -1.0, 0.0 }, 1.0, 1),
        Particle({ 0.0, 1.0, 0.0 }, { 1.0, 0.0, 0.0 }, 1.0, 1),
        Particle({ 0.0, -1.0, 0.0 }, { -1.0, 0.0, 0.0 }, 1.0, 1),
        Particle({ 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, 0.75, 2),
    };

    TestContainer pc = TestContainer(pv);

    size_t s;
    ASSERT_NO_THROW(s = pc.size());

    ASSERT_TRUE(s == pv.size());
}

// Test if we can properly resize the particle container
TEST(ParticleContainerResize, CorrectResize) {
    std::vector<Particle> pv = {
        Particle({ 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, 1.0, 1),
        Particle({ -1.0, 0.0, 0.0 }, { 0.0, -1.0, 0.0 }, 1.0, 1),
        Particle({ 0.0, 1.0, 0.0 }, { 1.0, 0.0, 0.0 }, 1.0, 1),
        Particle({ 0.0, -1.0, 0.0 }, { -1.0, 0.0, 0.0 }, 1.0, 1),
        Particle({ 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, 0.75, 2),
    };
    TestContainer pc = TestContainer(pv);

    ASSERT_NO_THROW(pc.resize(5));

    EXPECT_TRUE(pc.size() == 5) << "Resize must not change the data structure's size when given the current size";

    ASSERT_NO_THROW(pc.resize(3));
    pv.resize(3);

    EXPECT_TRUE(pc.size() == 3) << "Resize must shrink the data structure correctly";
    auto pi = pc.begin();
    for (auto& p : pv) {
        EXPECT_TRUE(*(pi++) == p) << "Resize must shrink the data structure correctly";
    }

    ASSERT_NO_THROW(pc.resize(5));
    pv.resize(5);

    EXPECT_TRUE(pc.size() == 5) << "Resize must expand the data structure correctly";
    pi = pc.begin();
    for (auto& p : pv) {
        EXPECT_TRUE(*(pi++) == p) << "Resize must expand the data structure correctly";
    }
}

// TODO: Test remove outside domain.