#include "gtest/gtest.h"
#include "yuml-parsers/modelParser.h"

class CallBehaviorActionParserTest : public ::testing::Test {

};

TEST_F(CallBehaviorActionParserTest, ForwardBehaviorParseTest) {
    ModelParser forwardBehaviorParser = ModelParser::createNewParser();
    YAML::Node forwardBehaviorNode = YAML::LoadFile("../../../../../src/test/yml/callBehaviorActionTests/forwardBehavior.yml");

    ASSERT_NO_THROW(forwardBehaviorParser.parse(forwardBehaviorNode));
    ModelParser::deleteParser(&forwardBehaviorParser);
}