#include "gtest/gtest.h"
#include "yuml-parsers/modelParser.h"
#include "uml/parameter.h"

using namespace UML;

class ParameterParserTest : public ::testing::Test {

};

TEST_F(ParameterParserTest, TestParseDirection) {
    // Setup
    ModelParser directionParser(new map<boost::uuids::uuid, Element*>);
    YAML::Node directionNode = YAML::LoadFile("../../../../../src/test/yml/parameterTests/parameterDirection.yml");

    // Test
    ASSERT_NO_THROW(directionParser.parse(directionNode));
    ASSERT_TRUE(((Parameter*)(*directionParser.elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->getDirection() == ParameterDirectionKind::IN);
}