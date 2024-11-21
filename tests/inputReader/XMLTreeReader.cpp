//
// Created by frederik on 11/21/24.
//
#include "inputReader/XMLTreeReader.h"
#include "Environment.h"
#include "ParticleContainer.h"
#include <gtest/gtest.h>


TEST(XMLTreeReader, Test1) {
    const char* xml = "../res/input.xml";
    const char* xsd_schema = "../res/input.xsd";
    Environment environment;
    ParticleContainer container;

    inputReader::XMLTreeReader reader;
    reader.readFile(container, xml, xsd_schema, environment);
}

