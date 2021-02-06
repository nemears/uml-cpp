#include "gtest/gtest.h"
#include "yuml-parsers/modelParser.h"

using namespace UML;

class ParameterParserTest : public ::testing::Test {

};

TEST_F(ParameterParserTest, TestParseDirection) {
    ModelParser directionParser(new map<boost::uuids::uuid, Element*>);
    YAML::Node directionNode = YAML::LoadFile("../../../../../src/test/yml/parameterTests/parameterDirection.yml");
    ASSERT_NO_THROW(directionParser.parse(directionNode));
}