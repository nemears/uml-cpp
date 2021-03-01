#include "gtest/gtest.h"
#include "yuml-parsers/modelParser.h"
#include "uml/activity.h"
#include "uml/objectNode.h"
#include "uml/primitiveType.h"
#include "uml/initialNode.h"
#include "uml/decisionNode.h"
#include "uml/literalBool.h"
#include "uml/action.h"
#include "uml/controlFlow.h"
#include "uml/objectFlow.h"

using namespace UML;

class ActivityEdgeParserTest : public ::testing::Test {

};

TEST_F(ActivityEdgeParserTest, ParseDecisionNodeGuardTest) {
    // Setup
    ModelParser decisionNodeGuardParser = ModelParser::createNewParser();
    YAML::Node decisionNodeGuardNode = YAML::LoadFile("../../../../../src/test/yml/activityEdgeTests/guardForDecision.yml");

    // Test
    ASSERT_NO_THROW(decisionNodeGuardParser.parse(decisionNodeGuardNode));
    ASSERT_TRUE(decisionNodeGuardParser.theEl->ownedElements.size() == 1);
    ASSERT_TRUE(decisionNodeGuardParser.theEl->ownedElements.front()->getElementType() == ElementType::ACTIVITY);

    // Activity
    Activity* activity = dynamic_cast<Activity*>(decisionNodeGuardParser.theEl->ownedElements.front());
    ASSERT_TRUE(activity->nodes.size() == 5);
    ASSERT_TRUE(activity->edges.size() == 4);
    list<ActivityNode*>::iterator nodeIt = activity->nodes.begin();

    // Object Node
    ASSERT_TRUE((*nodeIt)->getElementType() == ElementType::OBJECT_NODE);
    ObjectNode* objNode = dynamic_cast<ObjectNode*>((*nodeIt));
    ASSERT_TRUE(objNode->getType()->isPrimitive());
    ASSERT_TRUE(dynamic_cast<PrimitiveType*>(objNode->getType())->getPrimitiveType() == PrimitiveType::Primitive::BOOL);
    ASSERT_TRUE(objNode->outgoing.size() == 1);
    ASSERT_TRUE(objNode->outgoing.front()->getElementType() == ElementType::OBJECT_FLOW);
    ASSERT_TRUE(objNode->outgoing.front()->getTarget()->getElementType() == ElementType::DECISION_NODE);

    //InitialNode
    ++nodeIt;
    ASSERT_TRUE((*nodeIt)->getElementType() == ElementType::INITIAL_NODE);
    InitialNode* initialNode = dynamic_cast<InitialNode*>((*nodeIt));
    ASSERT_TRUE(initialNode->incoming.size() == 0);
    ASSERT_TRUE(initialNode->outgoing.size() == 1);
    ASSERT_TRUE(initialNode->outgoing.front()->getElementType() == ElementType::CONTROL_FLOW);
    ASSERT_TRUE(initialNode->outgoing.front()->getTarget()->getElementType() == ElementType::DECISION_NODE);

    // DecisionNode
    ++nodeIt;
    ASSERT_TRUE((*nodeIt)->getElementType() == ElementType::DECISION_NODE);
    DecisionNode* decisionNode = dynamic_cast<DecisionNode*>((*nodeIt));
    ASSERT_TRUE(decisionNode->incoming.size() == 2);
    ASSERT_TRUE(decisionNode->outgoing.size() == 2);
    ASSERT_TRUE(decisionNode->incoming.front()->getElementType() == ElementType::OBJECT_FLOW);
    ASSERT_TRUE(decisionNode->incoming.back()->getElementType() == ElementType::CONTROL_FLOW);
    ASSERT_TRUE(decisionNode->outgoing.front()->getElementType() == ElementType::CONTROL_FLOW);
    ASSERT_TRUE(decisionNode->outgoing.back()->getElementType() == ElementType::CONTROL_FLOW);
    ASSERT_TRUE(decisionNode->getDecisionInputFlow() != NULL);
    ASSERT_TRUE(decisionNode->getDecisionInputFlow()->uuid == boost::lexical_cast<boost::uuids::uuid>("563f4740-e107-4d08-8618-2489f0fe1865"));
    ASSERT_TRUE(decisionNode->outgoing.front()->getGuard() != NULL);
    ASSERT_TRUE(decisionNode->outgoing.front()->getGuard()->getElementType() == ElementType::LITERAL_BOOL);
    ASSERT_TRUE(dynamic_cast<LiteralBool*>(decisionNode->outgoing.front()->getGuard())->getValue());
    ASSERT_TRUE(decisionNode->outgoing.back()->getGuard() != NULL);
    ASSERT_TRUE(decisionNode->outgoing.back()->getGuard()->getElementType() == ElementType::LITERAL_BOOL);
    ASSERT_TRUE(dynamic_cast<LiteralBool*>(decisionNode->outgoing.back()->getGuard())->getValue() == false);

    // Delete
    ModelParser::deleteParser(&decisionNodeGuardParser);
}

TEST_F(ActivityEdgeParserTest, ParseBackwardsGuardTest) {
    // Setup
    ModelParser decisionNodeGuardParser = ModelParser::createNewParser();
    // Everything here is the same except as last test except for ordering of edges in this file
    YAML::Node decisionNodeGuardNode = YAML::LoadFile("../../../../../src/test/yml/activityEdgeTests/backwardsEdge.yml");

    // Test
    ASSERT_NO_THROW(decisionNodeGuardParser.parse(decisionNodeGuardNode));

    // Activity
    Activity* activity = dynamic_cast<Activity*>(decisionNodeGuardParser.theEl->ownedElements.front());
    ASSERT_TRUE(activity->nodes.size() == 5);
    ASSERT_TRUE(activity->edges.size() == 4);
    list<ActivityNode*>::iterator nodeIt = activity->nodes.begin();

    // Object Node
    ASSERT_TRUE((*nodeIt)->getElementType() == ElementType::OBJECT_NODE);
    ObjectNode* objNode = dynamic_cast<ObjectNode*>((*nodeIt));
    ASSERT_TRUE(objNode->getType()->isPrimitive());
    ASSERT_TRUE(dynamic_cast<PrimitiveType*>(objNode->getType())->getPrimitiveType() == PrimitiveType::Primitive::BOOL);
    ASSERT_TRUE(objNode->outgoing.size() == 1);
    ASSERT_TRUE(objNode->outgoing.front()->getElementType() == ElementType::OBJECT_FLOW);
    ASSERT_TRUE(objNode->outgoing.front()->getTarget()->getElementType() == ElementType::DECISION_NODE);

    //InitialNode
    ++nodeIt;
    ASSERT_TRUE((*nodeIt)->getElementType() == ElementType::INITIAL_NODE);
    InitialNode* initialNode = dynamic_cast<InitialNode*>((*nodeIt));
    ASSERT_TRUE(initialNode->incoming.size() == 0);
    ASSERT_TRUE(initialNode->outgoing.size() == 1);
    ASSERT_TRUE(initialNode->outgoing.front()->getElementType() == ElementType::CONTROL_FLOW);
    ASSERT_TRUE(initialNode->outgoing.front()->getTarget()->getElementType() == ElementType::DECISION_NODE);

    // DecisionNode
    ++nodeIt;
    ASSERT_TRUE((*nodeIt)->getElementType() == ElementType::DECISION_NODE);
    DecisionNode* decisionNode = dynamic_cast<DecisionNode*>((*nodeIt));
    ASSERT_TRUE(decisionNode->incoming.size() == 2);
    ASSERT_TRUE(decisionNode->outgoing.size() == 2);
    ASSERT_TRUE(decisionNode->incoming.front()->getElementType() == ElementType::OBJECT_FLOW);
    ASSERT_TRUE(decisionNode->incoming.back()->getElementType() == ElementType::CONTROL_FLOW);
    ASSERT_TRUE(decisionNode->outgoing.front()->getElementType() == ElementType::CONTROL_FLOW);
    ASSERT_TRUE(decisionNode->outgoing.back()->getElementType() == ElementType::CONTROL_FLOW);
    ASSERT_TRUE(decisionNode->getDecisionInputFlow() != NULL);
    ASSERT_TRUE(decisionNode->getDecisionInputFlow()->uuid == boost::lexical_cast<boost::uuids::uuid>("563f4740-e107-4d08-8618-2489f0fe1865"));
    ASSERT_TRUE(decisionNode->outgoing.front()->getGuard() != NULL);
    ASSERT_TRUE(decisionNode->outgoing.front()->getGuard()->getElementType() == ElementType::LITERAL_BOOL);
    ASSERT_TRUE(dynamic_cast<LiteralBool*>(decisionNode->outgoing.front()->getGuard())->getValue());
    ASSERT_TRUE(decisionNode->outgoing.back()->getGuard() != NULL);
    ASSERT_TRUE(decisionNode->outgoing.back()->getGuard()->getElementType() == ElementType::LITERAL_BOOL);
    ASSERT_TRUE(dynamic_cast<LiteralBool*>(decisionNode->outgoing.back()->getGuard())->getValue() == false);

    // Delete
    ModelParser::deleteParser(&decisionNodeGuardParser);
}

TEST_F(ActivityEdgeParserTest, EmitEdgeGuardTest) {
    // Setup
    Model m;
    m.setID("fa8cc066-6191-4903-a766-ee91d216c929");
    Activity activity;
    activity.setID("9c16720e-a366-4eef-825a-d46b5232a1d5");
    InitialNode init;
    init.setID("c7f09553-c13a-4d21-9e00-b9364c0aeaed");
    Action action1;
    action1.setID("fa7a57e9-88cf-489c-8345-8351336aec05");
    Action action2;
    action2.setID("7d18ee42-82c6-4f52-8ec4-fab67a75ff35");
    DecisionNode dec;
    dec.setID("c0ab87cc-d00b-4afb-9558-538253b442b2");
    ObjectNode obj;
    obj.setID("16c345b4-5ae2-41ca-a0e7-a9c386ac941d");
    ControlFlow initToDec;
    initToDec.setID("1bfe131b-0d9a-4e6f-9a9b-1dae55626202");
    ControlFlow decToAction1;
    decToAction1.setID("4b9519d3-cfd4-4bda-b1dc-6c7d0f521647");
    ControlFlow decToAction2;
    decToAction2.setID("2f821a87-6a14-47a0-bf78-cf57e24876d6");
    ObjectFlow objToDecision;
    objToDecision.setID("f73c6d44-5436-4021-83a6-ed90345c1f5f");
    PrimitiveType boolType;
    boolType.setPrimitiveType(PrimitiveType::Primitive::BOOL);
    obj.setType(&boolType);
    initToDec.setSource(&init);
    initToDec.setTarget(&dec);
    init.outgoing.push_back(&initToDec);
    dec.incoming.push_back(&initToDec);
    decToAction1.setSource(&dec);
    LiteralBool lbTrue;
    lbTrue.setValue(true);
    decToAction1.setGuard(&lbTrue);
    decToAction1.setTarget(&action1);
    dec.outgoing.push_back(&decToAction1);
    action1.incoming.push_back(&decToAction1);
    decToAction2.setSource(&dec);
    LiteralBool lbFalse;
    lbFalse.setValue(false);
    decToAction2.setGuard(&lbFalse);
    decToAction2.setTarget(&action2);
    dec.outgoing.push_back(&decToAction2);
    action2.incoming.push_back(&decToAction2);
    objToDecision.setSource(&obj);
    objToDecision.setTarget(&dec);
    obj.outgoing.push_back(&objToDecision);
    dec.incoming.push_back(&objToDecision);
    dec.setDecisionInputFlow(&objToDecision);
    activity.nodes.push_back(&init);
    init.setActivity(&activity);
    activity.nodes.push_back(&dec);
    dec.setActivity(&activity);
    activity.nodes.push_back(&obj);
    obj.setActivity(&activity);
    activity.nodes.push_back(&action1);
    action1.setActivity(&activity);
    activity.nodes.push_back(&action2);
    action2.setActivity(&activity);
    activity.edges.push_back(&initToDec);
    activity.edges.push_back(&objToDecision);
    activity.edges.push_back(&decToAction1);
    activity.edges.push_back(&decToAction2);
    m.ownedElements.push_back(&activity);
    activity.setOwner(&m);
    string expectedEmit = R""""(model:
  id: fa8cc066-6191-4903-a766-ee91d216c929
  children:
    - activity:
        id: 9c16720e-a366-4eef-825a-d46b5232a1d5
        nodes:
          - initialNode:
              id: c7f09553-c13a-4d21-9e00-b9364c0aeaed
              outgoing:
                - 1bfe131b-0d9a-4e6f-9a9b-1dae55626202
          - decisionNode:
              id: c0ab87cc-d00b-4afb-9558-538253b442b2
              incoming:
                - 1bfe131b-0d9a-4e6f-9a9b-1dae55626202
                - f73c6d44-5436-4021-83a6-ed90345c1f5f
              outgoing:
                - 4b9519d3-cfd4-4bda-b1dc-6c7d0f521647
                - 2f821a87-6a14-47a0-bf78-cf57e24876d6
          - objectNode:
              id: 16c345b4-5ae2-41ca-a0e7-a9c386ac941d
              type: BOOL
              outgoing:
                - f73c6d44-5436-4021-83a6-ed90345c1f5f
          - action:
              id: fa7a57e9-88cf-489c-8345-8351336aec05
              incoming:
                - 4b9519d3-cfd4-4bda-b1dc-6c7d0f521647
          - action:
              id: 7d18ee42-82c6-4f52-8ec4-fab67a75ff35
              incoming:
                - 2f821a87-6a14-47a0-bf78-cf57e24876d6
        edges:
          - controlFlow:
              id: 1bfe131b-0d9a-4e6f-9a9b-1dae55626202
              source: c7f09553-c13a-4d21-9e00-b9364c0aeaed
              target: c0ab87cc-d00b-4afb-9558-538253b442b2
          - objectFlow:
              id: f73c6d44-5436-4021-83a6-ed90345c1f5f
              source: 16c345b4-5ae2-41ca-a0e7-a9c386ac941d
              target: c0ab87cc-d00b-4afb-9558-538253b442b2
          - controlFlow:
              id: 4b9519d3-cfd4-4bda-b1dc-6c7d0f521647
              source: c0ab87cc-d00b-4afb-9558-538253b442b2
              target: fa7a57e9-88cf-489c-8345-8351336aec05
              guard: true
          - controlFlow:
              id: 2f821a87-6a14-47a0-bf78-cf57e24876d6
              source: c0ab87cc-d00b-4afb-9558-538253b442b2
              target: 7d18ee42-82c6-4f52-8ec4-fab67a75ff35
              guard: false)"""";

    // Test
    ModelParser mp = ModelParser::createNewParser();
    string generatedEmit;
    YAML::Emitter emitter;
    ASSERT_NO_THROW(mp.emit(emitter, &m));
    generatedEmit = emitter.c_str();
    cout << generatedEmit << '\n';
    ASSERT_TRUE(emitter.good());
    ASSERT_EQ(expectedEmit, generatedEmit);
}