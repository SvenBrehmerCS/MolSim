#pragma once

#include <Particle.h>
#include <gtest/gtest.h>

// Test if the particle can be constructed correctly without the type argument
TEST(ParticleConstructor, ParticleDefaultType) {
    const std::array<double, 3> x = { 3.0, 1.5, -14.0 };
    const std::array<double, 3> v = { -7.0, 2.0, -0.01 };
    const double m = 1.75;
    const std::array<double, 3> zero = { 0.0, 0.0, 0.0 };

    Particle p;

    ASSERT_NO_THROW(p = Particle(x, v, m));

    EXPECT_TRUE(p.getX() == x) << "The particle position must be the set position.";
    EXPECT_TRUE(p.getV() == v) << "The particle velocity must be the set velocity.";
    EXPECT_DOUBLE_EQ(p.getM(), m) << "The particle mass must be the set mass.";
    EXPECT_TRUE(p.getOldF() == zero) << "The forces must be initialized to zero";
    EXPECT_TRUE(p.getF() == zero) << "The forces must be initialized to zero";
    EXPECT_TRUE(p.getType() == 0) << "The type must have the default type.";
}

// Test if the particle can be constructed correctly with the type argument
TEST(ParticleConstructor, ParticleWithType) {
    const std::array<double, 3> x = { -2.0, 17.9, -1.33 };
    const std::array<double, 3> v = { -4.21, 2.1, 1.234 };
    const double m = 1.5;
    const std::array<double, 3> zero = { 0.0, 0.0, 0.0 };

    Particle p;

    ASSERT_NO_THROW(p = Particle(x, v, m, 1234));

    EXPECT_TRUE(p.getX() == x) << "The particle position must be the set position.";
    EXPECT_TRUE(p.getV() == v) << "The particle velocity must be the set velocity.";
    EXPECT_DOUBLE_EQ(p.getM(), m) << "The particle mass must be the set mass.";
    EXPECT_TRUE(p.getOldF() == zero) << "The forces must be initialized to zero";
    EXPECT_TRUE(p.getF() == zero) << "The forces must be initialized to zero";
    EXPECT_TRUE(p.getType() == 1234) << "The type must have the provided type.";
}

// Test if the particle can be converted to the string
TEST(ParticleToString, FirstToStringMethod) {
    const std::array<double, 3> x = { -2.0, 17.9, -1.33 };
    const std::array<double, 3> v = { -4.21, 2.1, 1.234 };
    const double m = 1.5;
    const std::array<double, 3> f = { -0.01, -1.2, 0.34 };
    const std::array<double, 3> old_f = { 6.6, 0.2, -1.0 };

    Particle p;

    ASSERT_NO_THROW(p = Particle(x, v, m, 22));
    ASSERT_NO_THROW(p.setF(f));
    ASSERT_NO_THROW(p.setOldF(old_f));

    std::string str;
    ASSERT_NO_THROW(str = p.toString());

    const std::string res = "Particle: X: [-2, 17.9, -1.33] v: [-4.21, 2.1, 1.234] f: [-0.01, -1.2, 0.34] old_f: [6.6, 0.2, -1] type: 22";
    EXPECT_TRUE(str == res) << "The correct string should be returned.";
}
