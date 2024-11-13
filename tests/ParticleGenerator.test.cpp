#include <gtest/gtest.h>
#include <ParticleGenerator.h>

//test if the Particles are initiliazed correctly if there are no previous particles in the container
TEST(ParticleGeneratorTest, ParticleGeneratorGenerateCuboid) {
    ParticleGenerator generator;
    ParticleContainer container;

    std::array<double, 3> x = {0.0, 0.0, 0.0};
    std::array<double, 3> y = {1.0, 1.0, 1.0};
    double mass = 1.0;
    std::array<int, 3> N = {2, 2, 1};
    double h = 1.0;
    double brownian_motion = 0.1;
    int dimension = 2;
    int num_particles = 0;
    int expected_particles_count = 4;

    container.resize(N[0] * N[1] * N[2]);

    Particle particle0;
    particle0.setX({0,0,0});
    particle0.setM(mass);
    particle0.setV({1.0, 1.0, 1.0});

    Particle particle1;
    particle1.setX({h,0,0});
    particle1.setM(mass);
    particle1.setV({1.0, 1.0, 1.0});

    Particle particle2;
    particle2.setX({0,h,0});
    particle2.setM(mass);
    particle2.setV({1.0, 1.0, 1.0});
    Particle particle3;
    particle3.setX({h,h,0});
    particle3.setM(mass);
    particle3.setV({1.0, 1.0, 1.0});

    generator.generateCuboid(container, num_particles, x, y, mass, N, h, brownian_motion, dimension);

    ASSERT_EQ(container.size(), expected_particles_count);
    ASSERT_EQ(container[0].getX(), particle0.getX());
    ASSERT_EQ(container[0].getM(), particle0.getM());
    ASSERT_NEAR(container[0].getV()[0], particle0.getV()[0], 0.5);
    ASSERT_NEAR(container[0].getV()[1], particle0.getV()[1], 0.5);
    ASSERT_NEAR(container[0].getV()[2], particle0.getV()[2], 0.5);

    ASSERT_EQ(container[1].getX(), particle1.getX());
    ASSERT_EQ(container[1].getM(), particle1.getM());
    ASSERT_NEAR(container[1].getV()[0], particle1.getV()[0], 0.5);
    ASSERT_NEAR(container[1].getV()[1], particle1.getV()[1], 0.5);
    ASSERT_NEAR(container[1].getV()[2], particle1.getV()[2], 0.5);

    ASSERT_EQ(container[2].getX(), particle2.getX());
    ASSERT_EQ(container[2].getM(), particle2.getM());
    ASSERT_NEAR(container[2].getV()[0], particle2.getV()[0], 0.5);
    ASSERT_NEAR(container[2].getV()[1], particle2.getV()[1], 0.5);
    ASSERT_NEAR(container[2].getV()[2], particle2.getV()[2], 0.5);

    ASSERT_EQ(container[3].getX(), particle3.getX());
    ASSERT_EQ(container[3].getM(), particle3.getM());
    ASSERT_NEAR(container[3].getV()[0], particle3.getV()[0], 0.5);
    ASSERT_NEAR(container[3].getV()[1], particle3.getV()[1], 0.5);
    ASSERT_NEAR(container[3].getV()[2], particle3.getV()[2], 0.5);
}


//test if the particles are also correctly initilized if there are already particles in the container, and checks if the existing particles aren't changed.
TEST(ParticleGeneratorTest, ParticleGeneratorTest_ParticleGeneratorGenerateCuboid_Test) {
    ParticleGenerator generator;
    ParticleContainer container;

    std::array<double, 3> x = {0.0, 0.0, 0.0};
    std::array<double, 3> y = {1.0, 1.0, 1.0};
    std::array<int, 3> N = {2, 1, 1};
    double mass = 1.0;
    double h = 1.0;
    double brownian_motion = 0.1;
    int dimension = 2;
    int num_particles = 1;

    container.resize(num_particles);
    container[0].setX({2.0,2.0,0});
    container[0].setM(2.0);
    container[0].setV({2.0, 2.0, 2.0});

    Particle particle0;

    particle0.setX({2.0,2.0,0});
    particle0.setM(2.0);
    particle0.setV({2.0, 2.0, 2.0});

    container.resize(num_particles + (N[0] * N[1] * N[2]));


    Particle particle1;
    particle1.setX({0, 0,0});
    particle1.setM(mass);
    particle1.setV({1.0, 1.0, 1.0});

    Particle particle2;
    particle2.setX({h, 0,0});
    particle2.setM(mass);
    particle2.setV({1.0, 1.0, 1.0});

    generator.generateCuboid(container, num_particles, x, y, mass, N, h, brownian_motion, dimension);

    ASSERT_EQ(container.size(), num_particles + (N[0] * N[1] * N[2]));
    ASSERT_EQ(container[0].getX(), particle0.getX());
    ASSERT_EQ(container[0].getM(), particle0.getM());
    ASSERT_EQ(container[0].getV()[0], particle0.getV()[0]);
    ASSERT_EQ(container[0].getV()[1], particle0.getV()[1]);
    ASSERT_EQ(container[0].getV()[2], particle0.getV()[2]);

    ASSERT_EQ(container[1].getX(), particle1.getX());
    ASSERT_EQ(container[1].getM(), particle1.getM());
    ASSERT_NEAR(container[1].getV()[0], particle1.getV()[0], 0.5);
    ASSERT_NEAR(container[1].getV()[1], particle1.getV()[1], 0.5);
    ASSERT_NEAR(container[1].getV()[2], particle1.getV()[2], 0.5);

    ASSERT_EQ(container[2].getX(), particle2.getX());
    ASSERT_EQ(container[2].getM(), particle2.getM());
    ASSERT_NEAR(container[2].getV()[0], particle2.getV()[0], 0.5);
    ASSERT_NEAR(container[2].getV()[1], particle2.getV()[1], 0.5);
    ASSERT_NEAR(container[2].getV()[2], particle2.getV()[2], 0.5);
}

//tests the velocity with a brownian motion of 0
TEST(ParticleGeneratorTest, ParticleGeneratorTest_ParticleGenerateCuboid_brownian_motion_Test) {
    ParticleGenerator generator;
    ParticleContainer container;

    std::array<double, 3> x = {0.0, 0.0, 0.0};
    std::array<double, 3> y = {1.0, 1.0, 1.0};
    double mass = 1.0;
    std::array<int, 3> N = {2, 1, 1};
    double h = 1.0;
    double brownian_motion = 0.0;
    int dimension = 2;
    int num_particles = 0;

    container.resize(num_particles + (N[0] * N[1] * N[2]));

    Particle particle0;
    particle0.setX({0.0,0.0,0});
    particle0.setM(mass);
    particle0.setV({1.0, 1.0, 1.0});

    Particle particle1;
    particle1.setX({h, 0,0});
    particle1.setM(mass);
    particle1.setV({1.0, 1.0, 1.0});

    generator.generateCuboid(container, num_particles, x, y, mass, N, h, brownian_motion, dimension);

    ASSERT_EQ(container.size(), 2);
    ASSERT_EQ(container[0].getX(), particle0.getX());
    ASSERT_EQ(container[0].getM(), particle0.getM());
    ASSERT_EQ(container[0].getV()[0], particle0.getV()[0]);
    ASSERT_EQ(container[0].getV()[1], particle0.getV()[1]);
    ASSERT_EQ(container[0].getV()[2], particle0.getV()[2]);
    ASSERT_EQ(container[1].getX(), particle1.getX());
    ASSERT_EQ(container[1].getM(), particle1.getM());
    ASSERT_EQ(container[1].getV()[0], particle1.getV()[0]);
    ASSERT_EQ(container[1].getV()[1], particle1.getV()[1]);
    ASSERT_EQ(container[1].getV()[2], particle1.getV()[2]);
}
