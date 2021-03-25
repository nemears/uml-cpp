#include "gtest/gtest.h"
#include "test/yumlParsersTest.h"
#include "yuml-parsers/modelParser.h"
#include "uml/activity.h"
#include "uml/callBehaviorAction.h"
#include "uml/opaqueBehavior.h"

class CallBehaviorActionParserTest : public ::testing::Test {
    public:
        string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(CallBehaviorActionParserTest, ForwardBehaviorParseTest) {
    ModelParser forwardBehaviorParser = ModelParser::createNewParser();
    YAML::Node forwardBehaviorNode = YAML::LoadFile(ymlPath + "callBehaviorActionTests/forwardBehavior.yml");

    ASSERT_NO_THROW(forwardBehaviorParser.parse(forwardBehaviorNode));
    ASSERT_TRUE(forwardBehaviorParser.theEl->getOwnedElements().size() == 2);
    ASSERT_TRUE(forwardBehaviorParser.theEl->getOwnedElements().front()->getElementType() == ElementType::OPAQUE_BEHAVIOR);
    ASSERT_TRUE(forwardBehaviorParser.theEl->getOwnedElements().back()->getElementType() == ElementType::ACTIVITY);
    Activity* act = dynamic_cast<Activity*>(forwardBehaviorParser.theEl->getOwnedElements().back());
    ASSERT_TRUE(act->getNodes().size() == 2);
    ASSERT_TRUE(act->getNodes().front()->getElementType() == ElementType::CALL_BEHAVIOR_ACTION);
    CallBehaviorAction* cba = dynamic_cast<CallBehaviorAction*>(act->getNodes().front());
    ASSERT_TRUE(cba->getBehavior() != NULL);
    ASSERT_TRUE(cba->getBehavior()->getID() == forwardBehaviorParser.theEl->getOwnedElements().front()->getID());
    ASSERT_TRUE(cba->getBehavior() == forwardBehaviorParser.theEl->getOwnedElements().front());
    ModelParser::deleteParser(&forwardBehaviorParser);
}

TEST_F(CallBehaviorActionParserTest, BackwardsBehaviorParseTest) {
    ModelParser backwardsBehaviorParser = ModelParser::createNewParser();
    YAML::Node backwardsBehaviorNode = YAML::LoadFile(ymlPath + "callBehaviorActionTests/backwardsBehavior.yml");

    ASSERT_NO_THROW(backwardsBehaviorParser.parse(backwardsBehaviorNode));
    ASSERT_TRUE(backwardsBehaviorParser.theEl->getOwnedElements().size() == 2);
    ASSERT_TRUE(backwardsBehaviorParser.theEl->getOwnedElements().back()->getElementType() == ElementType::OPAQUE_BEHAVIOR);
    ASSERT_TRUE(backwardsBehaviorParser.theEl->getOwnedElements().front()->getElementType() == ElementType::ACTIVITY);
    Activity* act = dynamic_cast<Activity*>(backwardsBehaviorParser.theEl->getOwnedElements().front());
    ASSERT_TRUE(act->getNodes().size() == 2);
    ASSERT_TRUE(act->getNodes().front()->getElementType() == ElementType::CALL_BEHAVIOR_ACTION);
    CallBehaviorAction* cba = dynamic_cast<CallBehaviorAction*>(act->getNodes().front());
    ASSERT_TRUE(cba->getBehavior() != NULL);
    ASSERT_TRUE(cba->getBehavior()->getID() == backwardsBehaviorParser.theEl->getOwnedElements().back()->getID());
    ASSERT_TRUE(cba->getBehavior() == backwardsBehaviorParser.theEl->getOwnedElements().back());
    ModelParser::deleteParser(&backwardsBehaviorParser);
}

TEST_F(CallBehaviorActionParserTest, EmitBehaviorTest) {
    Model m;
    m.setID("fa8cc066-6191-4903-a766-ee91d216c929");
    OpaqueBehavior ob;
    ob.setID("9c16720e-a366-4eef-825a-d46b5232a1d5");
    ob.bodies.push_back("return true");
    Activity act;
    act.setID("c7f09553-c13a-4d21-9e00-b9364c0aeaed");
    CallBehaviorAction cba;
    cba.setID("fa7a57e9-88cf-489c-8345-8351336aec05");
    act.getNodes().add(cba);
    cba.setActivity(&act);
    ob.setOwner(&m);
    act.setOwner(&m);
    ModelParser emitBehaviorParser = ModelParser::createNewParser();
    
    string expectedEmit = R""""(model:
  id: fa8cc066-6191-4903-a766-ee91d216c929
  children:
    - opaqueBehavior:
        id: 9c16720e-a366-4eef-825a-d46b5232a1d5
        body: return true
    - activity:
        id: c7f09553-c13a-4d21-9e00-b9364c0aeaed
        nodes:
          - callBehaviorAction:
              id: fa7a57e9-88cf-489c-8345-8351336aec05)"""";

    // Test
    string generatedEmit;
    YAML::Emitter emitter;
    ASSERT_NO_THROW(emitBehaviorParser.emit(emitter, &m));
    generatedEmit = emitter.c_str();
    cout << generatedEmit << '\n';
    ASSERT_TRUE(emitter.good());
    ASSERT_EQ(expectedEmit, generatedEmit);
}