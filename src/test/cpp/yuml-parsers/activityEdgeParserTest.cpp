#include "gtest/gtest.h"
#include "yuml-parsers/modelParser.h"

using namespace UML;

class ActivityEdgeParserTest : public ::testing::Test {

};

TEST_F(ActivityEdgeParserTest, ParseDecisionNodeGuardTest) {
    // Setup
    ModelParser decisionNodeGuardParser = ModelParser::createNewParser();
    YAML::Node decisionNodeGuardNode = YAML::LoadFile("../../../../../src/test/yml/activityEdgeTests/guardForDecision.yml");

    // Test
    ASSERT_NO_THROW(decisionNodeGuardParser.parse(decisionNodeGuardNode));

    // Delete
    ModelParser::deleteParser(&decisionNodeGuardParser);
}