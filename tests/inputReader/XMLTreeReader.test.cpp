//
// Created by frederik on 11/21/24.
//
#include "inputReader/XMLTreeReader.h"
#include "Environment.h"
#include "ParticleContainer.h"

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
    const char* xsd_schema = "../tests/res/input.xsd";

    Environment environment;
    ParticleContainer container;
    inputReader::XMLTreeReader reader;

    reader.readFile(container, xml, xsd_schema, environment);
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
