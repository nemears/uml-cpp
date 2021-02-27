#include "gtest/gtest.h"
#include "yuml-parsers/modelParser.h"
#include "uml/activity.h"
#include "uml/callBehaviorAction.h"

class CallBehaviorActionParserTest : public ::testing::Test {

};

TEST_F(CallBehaviorActionParserTest, ForwardBehaviorParseTest) {
    ModelParser forwardBehaviorParser = ModelParser::createNewParser();
    YAML::Node forwardBehaviorNode = YAML::LoadFile("../../../../../src/test/yml/callBehaviorActionTests/forwardBehavior.yml");

    ASSERT_NO_THROW(forwardBehaviorParser.parse(forwardBehaviorNode));
    ASSERT_TRUE(forwardBehaviorParser.theEl->ownedElements.size() == 2);
    ASSERT_TRUE(forwardBehaviorParser.theEl->ownedElements.front()->getElementType() == ElementType::OPAQUE_BEHAVIOR);
    ASSERT_TRUE(forwardBehaviorParser.theEl->ownedElements.back()->getElementType() == ElementType::ACTIVITY);
    Activity* act = dynamic_cast<Activity*>(forwardBehaviorParser.theEl->ownedElements.back());
    ASSERT_TRUE(act->nodes.size() == 2);
    ASSERT_TRUE(act->nodes.front()->getElementType() == ElementType::CALL_BEHAVIOR_ACTION);
    CallBehaviorAction* cba = dynamic_cast<CallBehaviorAction*>(act->nodes.front());
    ASSERT_TRUE(cba->getBehavior() != NULL);
    ASSERT_TRUE(cba->getBehavior()->uuid == forwardBehaviorParser.theEl->ownedElements.front()->uuid);
    ASSERT_TRUE(cba->getBehavior() == forwardBehaviorParser.theEl->ownedElements.front());
    ModelParser::deleteParser(&forwardBehaviorParser);
}

TEST_F(CallBehaviorActionParserTest, BackwardsBehaviorParseTest) {
    ModelParser backwardsBehaviorParser = ModelParser::createNewParser();
    YAML::Node backwardsBehaviorNode = YAML::LoadFile("../../../../../src/test/yml/callBehaviorActionTests/backwardsBehavior.yml");

    ASSERT_NO_THROW(backwardsBehaviorParser.parse(backwardsBehaviorNode));
    ASSERT_TRUE(backwardsBehaviorParser.theEl->ownedElements.size() == 2);
    ASSERT_TRUE(backwardsBehaviorParser.theEl->ownedElements.back()->getElementType() == ElementType::OPAQUE_BEHAVIOR);
    ASSERT_TRUE(backwardsBehaviorParser.theEl->ownedElements.front()->getElementType() == ElementType::ACTIVITY);
    Activity* act = dynamic_cast<Activity*>(backwardsBehaviorParser.theEl->ownedElements.front());
    ASSERT_TRUE(act->nodes.size() == 2);
    ASSERT_TRUE(act->nodes.front()->getElementType() == ElementType::CALL_BEHAVIOR_ACTION);
    CallBehaviorAction* cba = dynamic_cast<CallBehaviorAction*>(act->nodes.front());
    ASSERT_TRUE(cba->getBehavior() != NULL);
    ASSERT_TRUE(cba->getBehavior()->uuid == backwardsBehaviorParser.theEl->ownedElements.back()->uuid);
    ASSERT_TRUE(cba->getBehavior() == backwardsBehaviorParser.theEl->ownedElements.back());
    ModelParser::deleteParser(&backwardsBehaviorParser);
}