#include <Particle.h>
#include <gtest/gtest.h>

// Test if the particle can be constructed correctly without the type argument
TEST(ParticleConstructor, ParticleDefaultType) {
    const Vec<double> x = { 3.0, 1.5, -14.0 };
    const Vec<double> v = { -7.0, 2.0, -0.01 };
    const Vec<double> zero = { 0.0, 0.0, 0.0 };

    Particle p;

    ASSERT_NO_THROW(p = Particle(x, v));

    EXPECT_TRUE(p.getX() == x) << "The particle position must be the set position.";
    EXPECT_TRUE(p.getV() == v) << "The particle velocity must be the set velocity.";
    EXPECT_TRUE(p.getOldF() == zero) << "The forces must be initialized to zero";
    EXPECT_TRUE(p.getF() == zero) << "The forces must be initialized to zero";
    EXPECT_TRUE(p.getType() == 0) << "The type must have the default type.";
}

// Test if the particle can be constructed correctly with the type argument
TEST(ParticleConstructor, ParticleWithType) {
    const Vec<double> x = { -2.0, 17.9, -1.33 };
    const Vec<double> v = { -4.21, 2.1, 1.234 };
    const Vec<double> zero = { 0.0, 0.0, 0.0 };

    Particle p;

    ASSERT_NO_THROW(p = Particle(x, v, 1234));

    EXPECT_TRUE(p.getX() == x) << "The particle position must be the set position.";
    EXPECT_TRUE(p.getV() == v) << "The particle velocity must be the set velocity.";
    EXPECT_TRUE(p.getOldF() == zero) << "The forces must be initialized to zero";
    EXPECT_TRUE(p.getF() == zero) << "The forces must be initialized to zero";
    EXPECT_TRUE(p.getType() == 1234) << "The type must have the provided type.";
}

// Test if the particle can be converted to the string
TEST(ParticleToString, FirstToStringMethod) {
    const Vec<double> x = { -2.0, 17.9, -1.33 };
    const Vec<double> v = { -4.21, 2.1, 1.234 };
    const Vec<double> f = { -0.01, -1.2, 0.34 };
    const Vec<double> old_f = { 6.6, 0.2, -1.0 };

    Particle p;

    ASSERT_NO_THROW(p = Particle(x, v, 22));
    ASSERT_NO_THROW(p.setF(f));
    ASSERT_NO_THROW(p.setOldF(old_f));

    std::string str;
    ASSERT_NO_THROW(str = p.toString());

    const std::string res = "Particle: X: (-2, 17.9, -1.33) v: (-4.21, 2.1, 1.234) f: (-0.01, -1.2, 0.34) old_f: (6.6, 0.2, -1) type: 22";
    EXPECT_TRUE(str == res) << "The correct string should be returned."
                            << "(Expected: " << res << ", Got: " << str << ")";
}

TEST(ParticleEquals, CorrectEquals) {
    const Vec<double> x1 = { -2.0, 17.9, -1.33 };
    const Vec<double> v1 = { -4.21, 2.1, 1.234 };
    const Vec<double> f1 = { -0.01, -1.2, 0.34 };
    const Vec<double> old_f1 = { 6.6, 0.2, -1.0 };
    const Vec<double> x2 = { -2.0, 17.9, -1.33 };
    const Vec<double> v2 = { -4.21, 2.1, 1.234 };
    const Vec<double> f2 = { -0.01, -1.2, 0.34 };
    const Vec<double> old_f2 = { 6.6, 0.2, -1.0 };
    const Vec<double> x3 = { 2.0, 15.9, -1.69 };
    const Vec<double> v3 = { -5.21, -2.1, 1.24 };
    const Vec<double> f3 = { 0.0, -8.2, -3.141 };
    const Vec<double> old_f3 = { 6.9, 2., -34.8 };

    Particle p1;
    Particle p2;
    Particle p3;

    p1 = Particle(x1, v1, 22);
    p1.setF(f1);
    p1.setOldF(old_f1);
    p2 = Particle(x2, v2, 22);
    p2.setF(f2);
    p2.setOldF(old_f2);
    p3 = Particle(x3, v3, 35);
    p3.setF(f3);
    p3.setOldF(old_f3);

    bool equal, different;
    ASSERT_NO_THROW(equal = (p1 == p2));
    ASSERT_NO_THROW(different = (p1 == p3));

    EXPECT_TRUE(equal) << "Should not give false negatives.";
    EXPECT_FALSE(different) << "Should not give false negatives.";
}
