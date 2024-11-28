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
TEST(XMLTreeReader, Test1) {
    const char* xml = "../res/input.xml";
    const char* xsd_schema = "../res/input.xsd";
    Environment environment;
    ParticleContainer container;
    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

    std::ifstream file(xml);

    if (!file.is_open()) {
        std::cout << "File not found" << std::endl;
    }
    std::ifstream xsd(xsd_schema);
    if (!xsd.is_open()) {
        std::cout << "File not found" << std::endl;
    }

    inputReader::XMLTreeReader reader;
    reader.readFile(container, xml, xsd_schema, environment);
    EXPECT_EQ(environment.get_delta_t(), 0.014);
    EXPECT_EQ(environment.get_t_end(), 1000);
}
