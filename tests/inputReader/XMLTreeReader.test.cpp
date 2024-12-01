//
// Created by frederik on 11/21/24.
//
#include "inputReader/XMLTreeReader.h"
#include "Environment.h"
#include "boundaries/InfContainer.h"
#include "boundaries/ParticleContainer.h"

#include <bits/fs_fwd.h>
#include <bits/fs_path.h>
#include <fstream>
#include <gtest/gtest.h>

/**
 * // TODO
 * (Sven)
 * Empty XML
 * Error on restricted types(enumerations/dimensions)(isValid)
 *      static_assert(std::is_same<std::underlying_type_t<CalculatorType>, std::underlying_type_t<param_t::calc_type::value>>::value, "EnumTypeA and
 *      EnumTypeB must have the same underlying type.");
 * no value(pos_x) => error (isValid)(Sven) Wrong value types (isValid)
 *
 * (Fred)
 * Non default values = non default values
 *
 * Particles/Cuboids:
 *      correct values and numbers of elements
 *      no element = empty lists
 *
 * -----------------------------
 * (?num elements > size_t?)
 */

// this tests if the non default values given in the xml file, are set correctly
TEST(XMLTreeReader, TestNonDefaultValues) {
    const char* xml = "../tests/res/testNonDefault.xml";

    Environment environment;
    InfContainer container;
    inputReader::XMLTreeReader reader;

    reader.readFile(xml, environment, container);
    EXPECT_EQ(container.size(), 0);
    EXPECT_STREQ(environment.get_output_file_name(), "TestNonDefault");
    EXPECT_EQ(environment.get_output_file_format(), XYZ);
    EXPECT_EQ(environment.get_print_step(), 5);
    EXPECT_EQ(environment.get_calculator_type(), GRAVITY);
    EXPECT_EQ(environment.get_epsilon(), 10);
    EXPECT_EQ(environment.get_sigma(), 2.0);
    EXPECT_EQ(environment.get_delta_t(), 0.5);
    EXPECT_EQ(environment.get_t_end(), 500);
}

// test of the particles are inserted correctly
TEST(XMLTreeReader, TestParticles) {
    const char* xml = "../tests/res/testParticles.xml";

    Environment environment;
    InfContainer container;
    inputReader::XMLTreeReader reader;
    std::array<double, 3> x = { 0.1, 0.2, 0.3 };
    std::array<double, 3> v = { 1.0, 1.5, 1.7 };
    double m = 0.01;

    container.resize(1);

    reader.readFile(xml, environment, container);

    ASSERT_EQ(container.size(), 1);
    EXPECT_EQ(container[0].getX(), x);
    EXPECT_EQ(container[0].getV(), v);
    EXPECT_EQ(container[0].getM(), m);
}

// test if the cuboid is generated correctly
TEST(XMLTreeReader, TestXMLCuboid) {

    const char* xml = "../tests/res/testCuboids.xml";

    Environment environment;
    InfContainer container;
    inputReader::XMLTreeReader reader;

    std::array<double, 3> x0 = { 0, 0, 0 };
    std::array<double, 3> x1 = { 1, 0, 0 };
    std::array<double, 3> x2 = { 0, 1, 0 };
    std::array<double, 3> x3 = { 1, 1, 0 };
    std::array<double, 3> v = { 0.5, 0.7, 0.9 };

    container.resize(4);

    reader.readFile(xml, environment, container);
    ASSERT_EQ(container.size(), 4);
    EXPECT_EQ(container[0].getX(), x0);
    EXPECT_EQ(container[1].getX(), x1);
    EXPECT_EQ(container[2].getX(), x2);
    EXPECT_EQ(container[3].getX(), x3);
    EXPECT_EQ(container[0].getV(), v);
    EXPECT_EQ(container[1].getV(), v);
    EXPECT_EQ(container[2].getV(), v);
    EXPECT_EQ(container[3].getV(), v);
    EXPECT_EQ(container[0].getM(), 2.5);
    EXPECT_EQ(container[1].getM(), 2.5);
    EXPECT_EQ(container[2].getM(), 2.5);
    EXPECT_EQ(container[3].getM(), 2.5);
}

// test if the disc is generated correctly
TEST(XMLTreeReader, TestXMLDisc) {
    const char* xml = "../tests/res/testDisc.xml";
    Environment environment;
    InfContainer container;
    inputReader::XMLTreeReader reader;

    std::array<double, 3> x1 = { -1, 0.0, 0.0 };
    std::array<double, 3> x2 = { 0.0, -1, 0.0 };
    std::array<double, 3> x3 = { 0.0, 0.0, 0.0 };
    std::array<double, 3> x4 = { 0.0, 1, 0.0 };
    std::array<double, 3> x5 = { 1, 0.0, 0.0 };
    std::array<double, 3> v = { 1.0, 1.0, 1.0 };

    container.resize(5);

    reader.readFile(xml, environment, container);

    ASSERT_EQ(container.size(), 5);
    EXPECT_EQ(container[0].getX(), x1);
    EXPECT_EQ(container[1].getX(), x2);
    EXPECT_EQ(container[2].getX(), x3);
    EXPECT_EQ(container[3].getX(), x4);
    EXPECT_EQ(container[4].getX(), x5);
    EXPECT_EQ(container[0].getV(), v);
    EXPECT_EQ(container[1].getV(), v);
    EXPECT_EQ(container[2].getV(), v);
    EXPECT_EQ(container[3].getV(), v);
    EXPECT_EQ(container[4].getV(), v);
    EXPECT_EQ(container[0].getM(), 2.5);
    EXPECT_EQ(container[1].getM(), 2.5);
    EXPECT_EQ(container[2].getM(), 2.5);
    EXPECT_EQ(container[3].getM(), 2.5);
    EXPECT_EQ(container[4].getM(), 2.5);
}

// test of everything combined has the correct value

TEST(XMLTreeReader, TestXMLCombined) {
    const char* xml = "../tests/res/testCombined.xml";

    std::array<double, 3> x1disc = { -1, 0.0, 0.0 };
    std::array<double, 3> x2disc = { 0.0, -1, 0.0 };
    std::array<double, 3> x3disc = { 0.0, 0.0, 0.0 };
    std::array<double, 3> x4disc = { 0.0, 1, 0.0 };
    std::array<double, 3> x5disc = { 1, 0.0, 0.0 };
    std::array<double, 3> vdisc = { 1.0, 1.0, 1.0 };


    std::array<double, 3> x0cuboid = { 0, 0, 0 };
    std::array<double, 3> x1cuboid = { 1, 0, 0 };
    std::array<double, 3> x2cuboid = { 0, 1, 0 };
    std::array<double, 3> x3cuboid = { 1, 1, 0 };
    std::array<double, 3> vcuboid = { 0.5, 0.7, 0.9 };

    std::array<double, 3> xparticle = { 0.1, 0.2, 0.3 };
    std::array<double, 3> vparticle = { 1.0, 1.5, 1.7 };

    Environment environment;
    InfContainer container;
    inputReader::XMLTreeReader reader;

    container.resize(10);

    reader.readFile(xml, environment, container);

    ASSERT_EQ(container.size(), 10);

    EXPECT_EQ(container[0].getX(), xparticle);
    EXPECT_EQ(container[0].getV(), vparticle);
    EXPECT_EQ(container[0].getM(), 0.01);

    EXPECT_EQ(container[1].getX(), x0cuboid);
    EXPECT_EQ(container[2].getX(), x1cuboid);
    EXPECT_EQ(container[3].getX(), x2cuboid);
    EXPECT_EQ(container[4].getX(), x3cuboid);

    EXPECT_EQ(container[5].getX(), x1disc);
    EXPECT_EQ(container[6].getX(), x2disc);
    EXPECT_EQ(container[7].getX(), x3disc);
    EXPECT_EQ(container[8].getX(), x4disc);
    EXPECT_EQ(container[9].getX(), x5disc);

    EXPECT_EQ(container[1].getV(), vcuboid);
    EXPECT_EQ(container[2].getV(), vcuboid);
    EXPECT_EQ(container[3].getV(), vcuboid);
    EXPECT_EQ(container[4].getV(), vcuboid);

    EXPECT_EQ(container[5].getV(), vdisc);
    EXPECT_EQ(container[6].getV(), vdisc);
    EXPECT_EQ(container[7].getV(), vdisc);
    EXPECT_EQ(container[8].getV(), vdisc);
    EXPECT_EQ(container[9].getV(), vdisc);

    EXPECT_EQ(container[1].getM(), 2.5);
    EXPECT_EQ(container[2].getM(), 2.5);
    EXPECT_EQ(container[3].getM(), 2.5);
    EXPECT_EQ(container[4].getM(), 2.5);
    EXPECT_EQ(container[5].getM(), 2.5);
    EXPECT_EQ(container[6].getM(), 2.5);
    EXPECT_EQ(container[7].getM(), 2.5);
    EXPECT_EQ(container[8].getM(), 2.5);
    EXPECT_EQ(container[9].getM(), 2.5);
}
