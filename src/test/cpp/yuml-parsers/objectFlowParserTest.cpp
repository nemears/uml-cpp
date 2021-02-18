#include "gtest/gtest.h"
#include "yuml-parsers/modelParser.h"

using namespace UML;

class ObjectFlowParserTest : public ::testing::Test {

};

TEST_F(ObjectFlowParserTest, ParseObjectNodeToActionTest) {
    // Setup
    ModelParser objectToActionParser = ModelParser::createNewParser();
    YAML::Node objectToActionNode = YAML::LoadFile("../../../../../src/test/yml/objectFlowTests/objectNodeToAction.yml");

    // Test
    ASSERT_NO_THROW(objectToActionParser.parse(objectToActionNode));
}