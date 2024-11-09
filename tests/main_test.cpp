//
// Created by frederik on 11/2/24.
//

#include <gtest/gtest.h>
#include <Particle.h>

TEST(getType, Get_Type) {
    Particle particle(1);
    EXPECT_EQ(particle.getType(), 1);
}
TEST(getType, GetType2) {
    Particle particle(1);
    EXPECT_NE(particle.getType(), 2);
}
