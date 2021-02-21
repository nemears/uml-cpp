#include "gtest/gtest.h"
#include "yuml-parsers/modelParser.h"
#include "uml/parameterNode.h"
#include "uml/activity.h"

using namespace UML;

class ParameterNodeTest : public ::testing::Test {

};

TEST_F(ParameterNodeTest, ParseParameterNodeTest) {
    // Setup
    ModelParser parameterNodeParser = ModelParser::createNewParser();
    YAML::Node parameterNodeNode = YAML::LoadFile("../../../../../src/test/yml/parameterNodeTests/parameterNode.yml");

    // Test
    ASSERT_NO_THROW(parameterNodeParser.parse(parameterNodeNode));

    // Activity
    ASSERT_TRUE(parameterNodeParser.theEl->ownedElements.size() == 1);
    ASSERT_TRUE(parameterNodeParser.theEl->ownedElements.front()->getElementType() == ElementType::ACTIVITY);
    ASSERT_TRUE(dynamic_cast<Activity*>(parameterNodeParser.theEl->ownedElements.front())->parameters.size() == 1);
}