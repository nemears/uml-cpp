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
    YAML::Node backwardsBehaviorNode = YAML::LoadFile(ymlPath + "callBehaviorActionTests/backwardsBehavior.yml");

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
    act.nodes.push_back(&cba);
    cba.setActivity(&act);
    ob.setOwner(&m);
    m.ownedElements.push_back(&ob);
    m.ownedElements.push_back(&act);
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