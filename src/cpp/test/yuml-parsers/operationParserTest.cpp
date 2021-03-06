#include "gtest/gtest.h"
#include "test/yumlParsersTest.h"
#include "uml/model.h"
#include "uml/class.h"
#include "uml/operation.h"
#include "uml/parameter.h"
#include "uml/primitiveType.h"
#include "uml/opaqueBehavior.h"
#include "yuml-parsers/modelParser.h"
#include "uml/activity.h"
#include "uml/primitiveType.h"
#include "uml/initialNode.h"
#include "uml/parameterNode.h"
#include "uml/action.h"
#include "uml/decisionNode.h"
#include "uml/joinNode.h"
#include "uml/finalNode.h"
#include "uml/objectFlow.h"
#include "uml/controlFlow.h"

using namespace UML;

class OperationParserTest : public ::testing::Test {
   public:
        string ymlPath;
        void SetUp() override {
            ymlPath = YML_FILES_PATH;
        };
};

TEST_F(OperationParserTest, ParseInvalidParameterTest) {
  // Setup
  ModelParser invalidParameterParser = ModelParser::createNewParser();
  YAML::Node invalidParameterNode = YAML::LoadFile(ymlPath + "operationTests/invalidParameterList.yml");

  // Test
  EXPECT_THROW(invalidParameterParser.parse(invalidParameterNode), ElementParser::InvalidNodeTypeException);
}

TEST_F(OperationParserTest, ParseInvalidMethodTest) {
  ModelParser invalidMethodParser = ModelParser::createNewParser();
  YAML::Node invalidMethodNode = YAML::LoadFile(ymlPath + "operationTests/invalidMethodList.yml");

  EXPECT_THROW(invalidMethodParser.parse(invalidMethodNode), ElementParser::InvalidNodeTypeException);
}

TEST_F(OperationParserTest, ParseInvalidTypeTest) {
  // Setup
  ModelParser invalidTypeParser = ModelParser::createNewParser();
  YAML::Node invalidTypeNode = YAML::LoadFile(ymlPath + "operationTests/invalidType.yml");

  // Test
  EXPECT_THROW(invalidTypeParser.parse(invalidTypeNode), ElementParser::InvalidIdentifierException);
}

TEST_F(OperationParserTest, EmitOperationWithLiteralParameter) {
  // Setup
  Model m;
  m.setID("190d1cb9-13dc-44e6-a064-126891ae0033");
  Class c;
  c.setID("16c345b4-5ae2-41ca-a0e7-a9c386ac941d");
  Operation o;
  o.setID("7d18ee42-82c6-4f52-8ec4-fab67a75ff35");
  Parameter p;
  p.setID("563f4740-e107-4d08-8618-2489f0fe1865");
  p.setName("test");
  PrimitiveType pt;
  pt.setPrimitiveType(PrimitiveType::Primitive::STRING);
  p.setType(&pt);
  o.parameters.push_back(&p);
  c.operations.push_back(&o);
  m.ownedElements.push_back(&c);

  ModelParser emitOperationWithParameterTestParser = ModelParser::createNewParser();
  string expectedEmit = R""""(model:
  id: 190d1cb9-13dc-44e6-a064-126891ae0033
  children:
    - class:
        id: 16c345b4-5ae2-41ca-a0e7-a9c386ac941d
        operations:
          - operation:
              id: 7d18ee42-82c6-4f52-8ec4-fab67a75ff35
              parameters:
                - parameter:
                    type: STRING
                    name: test
                    id: 563f4740-e107-4d08-8618-2489f0fe1865)"""";

  string generatedEmit;
  YAML::Emitter emitter;
  ASSERT_NO_THROW(emitOperationWithParameterTestParser.emit(emitter, &m));
  generatedEmit = emitter.c_str();
  cout << generatedEmit << '\n';
  ASSERT_TRUE(emitter.good());
  ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(OperationParserTest, EmitOperationWithInstanceParameter) {
  // Setup
  Model m;
  m.setID("190d1cb9-13dc-44e6-a064-126891ae0033");
  Class c2;
  c2.setID("d9ab2f06-4c2c-4330-9e1b-7eaee423a66a");
  Class c;
  c.setID("16c345b4-5ae2-41ca-a0e7-a9c386ac941d");
  Operation o;
  o.setID("7d18ee42-82c6-4f52-8ec4-fab67a75ff35");
  Parameter p;
  p.setID("563f4740-e107-4d08-8618-2489f0fe1865");
  p.setName("test");
  p.setType(&c2);
  o.parameters.push_back(&p);
  c.operations.push_back(&o);
  m.ownedElements.push_back(&c2);
  m.ownedElements.push_back(&c);

  ModelParser emitOperationWithInstanceParameterTestParser = ModelParser::createNewParser();
  string expectedEmit = R""""(model:
  id: 190d1cb9-13dc-44e6-a064-126891ae0033
  children:
    - class:
        id: d9ab2f06-4c2c-4330-9e1b-7eaee423a66a
    - class:
        id: 16c345b4-5ae2-41ca-a0e7-a9c386ac941d
        operations:
          - operation:
              id: 7d18ee42-82c6-4f52-8ec4-fab67a75ff35
              parameters:
                - parameter:
                    type: d9ab2f06-4c2c-4330-9e1b-7eaee423a66a
                    name: test
                    id: 563f4740-e107-4d08-8618-2489f0fe1865)"""";

  string generatedEmit;
  YAML::Emitter emitter;
  ASSERT_NO_THROW(emitOperationWithInstanceParameterTestParser.emit(emitter, &m));
  generatedEmit = emitter.c_str();
  cout << generatedEmit << '\n';
  ASSERT_TRUE(emitter.good());
  ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(OperationParserTest, EmitOperationWithBlankOpaqueBehaviorTest) {
  // Setup
  Model m;
  m.setID("190d1cb9-13dc-44e6-a064-126891ae0033");
  Class c;
  c.setID("16c345b4-5ae2-41ca-a0e7-a9c386ac941d");
  Operation o;
  o.setID("7d18ee42-82c6-4f52-8ec4-fab67a75ff35");
  OpaqueBehavior ob;
  ob.setID("d9ab2f06-4c2c-4330-9e1b-7eaee423a66a");
  ob.setName("foo");
  o.methods.push_back(&ob);
  c.operations.push_back(&o);
  m.ownedElements.push_back(&c);

  ModelParser emitOperationWithBlankOpaqueBehaviorTestParser = ModelParser::createNewParser();
  string expectedEmit = R""""(model:
  id: 190d1cb9-13dc-44e6-a064-126891ae0033
  children:
    - class:
        id: 16c345b4-5ae2-41ca-a0e7-a9c386ac941d
        operations:
          - operation:
              id: 7d18ee42-82c6-4f52-8ec4-fab67a75ff35
              methods:
                - opaqueBehavior:
                    name: foo
                    id: d9ab2f06-4c2c-4330-9e1b-7eaee423a66a)"""";

  string generatedEmit;
  YAML::Emitter emitter;
  ASSERT_NO_THROW(emitOperationWithBlankOpaqueBehaviorTestParser.emit(emitter, &m));
  generatedEmit = emitter.c_str();
  cout << generatedEmit << '\n';
  ASSERT_TRUE(emitter.good());
  ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(OperationParserTest, EmitAbstractMethodTest) {
  // Setup
  Model m;
  Class c;
  Operation o;
  Behavior b;
  o.methods.push_back(&b);
  c.operations.push_back(&o);
  m.ownedElements.push_back(&c);
  ModelParser abstractMethodParser = ModelParser::createNewParser();

  EXPECT_THROW(abstractMethodParser.emitDocument(&m), ElementParser::AbstractTypeEmitException);
}

TEST_F(OperationParserTest, ParseBackwardsMethodTest) {
  // Setup
  ModelParser backwardsMethodParser = ModelParser::createNewParser();
  YAML::Node backwardsMethodsNode = YAML::LoadFile(ymlPath + "operationTests/backwardsMethod.yml");

  //Test
  ASSERT_NO_THROW(backwardsMethodParser.parse(backwardsMethodsNode));
  ASSERT_TRUE(backwardsMethodParser.theEl->ownedElements.size() == 2);

  // Class
  ASSERT_TRUE(backwardsMethodParser.theEl->ownedElements.front()->getElementType() == ElementType::CLASS);
  ASSERT_TRUE(dynamic_cast<Class*>(backwardsMethodParser.theEl->ownedElements.front())->operations.size() == 1);
  
  // Operation
  Operation* op = dynamic_cast<Class*>(backwardsMethodParser.theEl->ownedElements.front())->operations.front();
  ASSERT_TRUE(op->methods.size() == 1);
  ASSERT_TRUE(op->methods.front()->getElementType() == ElementType::ACTIVITY);
  ASSERT_TRUE(op->methods.front()->uuid == boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d"));
  
  // Activity
  ASSERT_TRUE(backwardsMethodParser.theEl->ownedElements.back()->getElementType() == ElementType::ACTIVITY);
  Activity* a = dynamic_cast<Activity*>(backwardsMethodParser.theEl->ownedElements.back());
  ASSERT_TRUE(a->uuid == boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d"));
}

TEST_F(OperationParserTest, ParseForwardMethodTest) {
  // Setup
  ModelParser forwardsMethodParser = ModelParser::createNewParser();
  YAML::Node forwardsMethodsNode = YAML::LoadFile(ymlPath + "operationTests/forwardsMethod.yml");

  // Test
  ASSERT_NO_THROW(forwardsMethodParser.parse(forwardsMethodsNode));
  ASSERT_TRUE(forwardsMethodParser.theEl->ownedElements.size() == 2);

  // Class
  ASSERT_TRUE(forwardsMethodParser.theEl->ownedElements.back()->getElementType() == ElementType::CLASS);
  ASSERT_TRUE(dynamic_cast<Class*>(forwardsMethodParser.theEl->ownedElements.back())->operations.size() == 1);
  
  // Operation
  Operation* op = dynamic_cast<Class*>(forwardsMethodParser.theEl->ownedElements.back())->operations.front();
  ASSERT_TRUE(op->methods.size() == 1);
  ASSERT_TRUE(op->methods.front()->getElementType() == ElementType::ACTIVITY);
  ASSERT_TRUE(op->methods.front()->uuid == boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d"));
  
  // Activity
  ASSERT_TRUE(forwardsMethodParser.theEl->ownedElements.front()->getElementType() == ElementType::ACTIVITY);
  Activity* a = dynamic_cast<Activity*>(forwardsMethodParser.theEl->ownedElements.front());
  ASSERT_TRUE(a->uuid == boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d"));
}

TEST_F(OperationParserTest, ParseActvityCorrespondedToOperationTest) {
  // Setup
  ModelParser actvityAndOperationParser = ModelParser::createNewParser();
  YAML::Node activityAndOperationNode = YAML::LoadFile(ymlPath + "operationTests/activityMethod.yml");

  // Test
  ASSERT_NO_THROW(actvityAndOperationParser.parse(activityAndOperationNode));
  ASSERT_TRUE(actvityAndOperationParser.theEl->ownedElements.size() == 1);
  ASSERT_TRUE(actvityAndOperationParser.theEl->ownedElements.front()->getElementType() == ElementType::CLASS);

  // Class
  Class* clazz = dynamic_cast<Class*>(actvityAndOperationParser.theEl->ownedElements.front());
  ASSERT_TRUE(clazz->operations.size() == 1);
  
  // Operation
  Operation * op = clazz->operations.front();
  ASSERT_TRUE(op->parameters.size() == 2);
  ASSERT_TRUE(op->methods.size() == 1);

  //Parameters
  ASSERT_TRUE(op->parameters.front()->uuid == boost::lexical_cast<boost::uuids::uuid>("1bfe131b-0d9a-4e6f-9a9b-1dae55626202"));
  Parameter* inParam = op->parameters.front();
  ASSERT_TRUE(inParam->getType() != NULL);
  ASSERT_TRUE(inParam->getType()->isPrimitive());
  ASSERT_TRUE(dynamic_cast<PrimitiveType*>(inParam->getType())->getPrimitiveType() == PrimitiveType::Primitive::INT);
  ASSERT_TRUE(inParam->getDirection() == ParameterDirectionKind::IN);

  ASSERT_TRUE(op->parameters.back()->uuid == boost::lexical_cast<boost::uuids::uuid>("4b9519d3-cfd4-4bda-b1dc-6c7d0f521647"));
  Parameter* retParam = op->parameters.back();
  ASSERT_TRUE(retParam->getType() != NULL);
  ASSERT_TRUE(retParam->getType()->isPrimitive());
  ASSERT_TRUE(dynamic_cast<PrimitiveType*>(retParam->getType())->getPrimitiveType() == PrimitiveType::Primitive::BOOL);
  ASSERT_TRUE(retParam->getDirection() == ParameterDirectionKind::RETURN);

  // Activity
  ASSERT_TRUE(op->methods.front()->getElementType() == ElementType::ACTIVITY);
  ASSERT_TRUE(op->methods.front()->uuid == boost::lexical_cast<boost::uuids::uuid>("54e8f139-9581-48a4-8021-32ff00606c93"));
  Activity* activity = dynamic_cast<Activity*>(op->methods.front());
  ASSERT_TRUE(activity->parameters.size() == 2);
  ASSERT_TRUE(activity->parameters.front() == inParam);
  ASSERT_TRUE(activity->parameters.back() == retParam);
  ASSERT_TRUE(activity->nodes.size() == 8);
  ASSERT_TRUE(activity->edges.size() == 7);
  list<ActivityNode*>::iterator nodeIt = activity->nodes.begin();
  list<ActivityEdge*>::iterator edgeIt = activity->edges.begin();

  // InitialNode
  ASSERT_TRUE((*nodeIt)->uuid == boost::lexical_cast<boost::uuids::uuid>("25a0f5f5-0d02-40e6-a70e-c3c606fcfde0"));
  ASSERT_TRUE((*nodeIt)->getElementType() == ElementType::INITIAL_NODE);
  InitialNode* initNode = dynamic_cast<InitialNode*>((*nodeIt));

  // ParameterNode
  ++nodeIt;
  ASSERT_TRUE((*nodeIt)->uuid == boost::lexical_cast<boost::uuids::uuid>("2f821a87-6a14-47a0-bf78-cf57e24876d6"));
  ASSERT_TRUE((*nodeIt)->getElementType() == ElementType::PARAMETER_NODE);
  ParameterNode* paramNode = dynamic_cast<ParameterNode*>((*nodeIt));
  ASSERT_TRUE(paramNode->getParameter() != NULL);
  ASSERT_TRUE(paramNode->getParameter() == inParam);
  ASSERT_TRUE(paramNode->getType() != NULL);
  ASSERT_TRUE(paramNode->getType()->isPrimitive());
  ASSERT_TRUE(dynamic_cast<PrimitiveType*>(paramNode->getType())->getPrimitiveType() == PrimitiveType::Primitive::INT);

  // Action 1
  ++nodeIt;
  ASSERT_TRUE((*nodeIt)->uuid == boost::lexical_cast<boost::uuids::uuid>("0734d34f-066f-4029-97d9-e39ac2f40f2b"));
  ASSERT_TRUE((*nodeIt)->getElementType() == ElementType::ACTION);
  Action* action1 = dynamic_cast<Action*>((*nodeIt));
  ASSERT_TRUE(action1->getName().compare("isValid") == 0);

  // InputPin
  ++nodeIt;
  ASSERT_TRUE((*nodeIt)->uuid == boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d"));
  ASSERT_TRUE((*nodeIt)->getElementType() == ElementType::INPUT_PIN);
  InputPin* inputPin = dynamic_cast<InputPin*>((*nodeIt));
  ASSERT_TRUE(inputPin->getType() != NULL);
  ASSERT_TRUE(inputPin->getType()->isPrimitive());
  ASSERT_TRUE(dynamic_cast<PrimitiveType*>(inputPin->getType())->getPrimitiveType() == PrimitiveType::Primitive::INT);

  // DecisionNode
  ++nodeIt;
  ASSERT_TRUE((*nodeIt)->uuid == boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35"));
  ASSERT_TRUE((*nodeIt)->getElementType() == ElementType::DECISION_NODE);
  DecisionNode* decision = dynamic_cast<DecisionNode*>((*nodeIt));

  // Action 2
  ++nodeIt;
  ASSERT_TRUE((*nodeIt)->uuid == boost::lexical_cast<boost::uuids::uuid>("563f4740-e107-4d08-8618-2489f0fe1865"));
  ASSERT_TRUE((*nodeIt)->getElementType() == ElementType::ACTION);
  Action* action2 = dynamic_cast<Action*>((*nodeIt));
  ASSERT_TRUE(action2->getName().compare("actA") == 0);

  // Join Node
  ++nodeIt;
  ASSERT_TRUE((*nodeIt)->uuid == boost::lexical_cast<boost::uuids::uuid>("2fd853ac-e07e-48e5-96a9-21f5a251e571"));
  ASSERT_TRUE((*nodeIt)->getElementType() == ElementType::JOIN_NODE);
  JoinNode* joinNode = dynamic_cast<JoinNode*>((*nodeIt));

  // Final Node
  ++nodeIt;
  ASSERT_TRUE((*nodeIt)->uuid == boost::lexical_cast<boost::uuids::uuid>("880fffc9-d291-48c4-8cb6-e8ac6bd4a454"));
  ASSERT_TRUE((*nodeIt)->getElementType() == ElementType::FINAL_NODE);
  FinalNode* finalNode = dynamic_cast<FinalNode*>((*nodeIt));

  // Edges
  // ParameterNode to isValid InputPin
  ASSERT_TRUE((*edgeIt)->uuid == boost::lexical_cast<boost::uuids::uuid>("f73c6d44-5436-4021-83a6-ed90345c1f5f"));
  ASSERT_TRUE((*edgeIt)->getElementType() == ElementType::OBJECT_FLOW);
  ObjectFlow* paramNodeToInputPin = dynamic_cast<ObjectFlow*>((*edgeIt));
  ASSERT_TRUE(paramNodeToInputPin->getSource() == paramNode);
  ASSERT_TRUE(paramNode->outgoing.front() = paramNodeToInputPin);
  ASSERT_TRUE(paramNodeToInputPin->getTarget() == inputPin);
  ASSERT_TRUE(inputPin->incoming.front() == paramNodeToInputPin);
  
  // InitialNode to isValid
  ++edgeIt;
  ASSERT_TRUE((*edgeIt)->uuid == boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033"));
  ASSERT_TRUE((*edgeIt)->getElementType() == ElementType::CONTROL_FLOW);
  ControlFlow* initialNodeToAction = dynamic_cast<ControlFlow*>((*edgeIt));
  ASSERT_TRUE(initialNodeToAction->getSource() == initNode);
  ASSERT_TRUE(initNode->outgoing.front() = initialNodeToAction);
  ASSERT_TRUE(initialNodeToAction->getTarget() == action1);
  ASSERT_TRUE(action1->incoming.front() == initialNodeToAction);

  // isValid to DecisionNode
  ++edgeIt;
  ASSERT_TRUE((*edgeIt)->uuid == boost::lexical_cast<boost::uuids::uuid>("c0ab87cc-d00b-4afb-9558-538253b442b2"));
  ASSERT_TRUE((*edgeIt)->getElementType() == ElementType::CONTROL_FLOW);
  ControlFlow* action1ToDecision = dynamic_cast<ControlFlow*>((*edgeIt));
  ASSERT_TRUE(action1ToDecision->getSource() == action1);
  ASSERT_TRUE(action1->outgoing.front() = action1ToDecision);
  ASSERT_TRUE(action1ToDecision->getTarget() == decision);
  ASSERT_TRUE(decision->incoming.front() == action1ToDecision);

  // DecisionNode to ActA
  ++edgeIt;
  ASSERT_TRUE((*edgeIt)->uuid == boost::lexical_cast<boost::uuids::uuid>("d9ab2f06-4c2c-4330-9e1b-7eaee423a66a"));
  ASSERT_TRUE((*edgeIt)->getElementType() == ElementType::CONTROL_FLOW);
  ControlFlow* decisionToAction2 = dynamic_cast<ControlFlow*>((*edgeIt));
  ASSERT_TRUE(decisionToAction2->getSource() == decision);
  ASSERT_TRUE(decision->outgoing.front() = decisionToAction2);
  ASSERT_TRUE(decisionToAction2->getTarget() == action2);
  ASSERT_TRUE(action2->incoming.front() == decisionToAction2);

  // DecisionNode to JoinNode
  ++edgeIt;
  ASSERT_TRUE((*edgeIt)->uuid == boost::lexical_cast<boost::uuids::uuid>("4680c886-40c6-45ed-9ae2-6aa37aa096e0"));
  ASSERT_TRUE((*edgeIt)->getElementType() == ElementType::CONTROL_FLOW);
  ControlFlow* decisionToJoin = dynamic_cast<ControlFlow*>((*edgeIt));
  ASSERT_TRUE(decisionToJoin->getSource() == decision);
  ASSERT_TRUE(decision->outgoing.front() = decisionToJoin);
  ASSERT_TRUE(decisionToJoin->getTarget() == joinNode);
  ASSERT_TRUE(joinNode->incoming.front() == decisionToJoin);

  // Action2 to JoinNode
  ++edgeIt;
  ASSERT_TRUE((*edgeIt)->uuid == boost::lexical_cast<boost::uuids::uuid>("ddb5dde3-4c5a-47be-99db-9b7fb7247793"));
  ASSERT_TRUE((*edgeIt)->getElementType() == ElementType::CONTROL_FLOW);
  ControlFlow* action2ToJoin = dynamic_cast<ControlFlow*>((*edgeIt));
  ASSERT_TRUE(action2ToJoin->getSource() == action2);
  ASSERT_TRUE(action2->outgoing.front() = action2ToJoin);
  ASSERT_TRUE(action2ToJoin->getTarget() == joinNode);
  ASSERT_TRUE(joinNode->incoming.back() == action2ToJoin);

  // JoinNode to Final
  ++edgeIt;
  ASSERT_TRUE((*edgeIt)->uuid == boost::lexical_cast<boost::uuids::uuid>("f5c03bef-5578-4551-a0da-162d8734666d"));
  ASSERT_TRUE((*edgeIt)->getElementType() == ElementType::CONTROL_FLOW);
  ControlFlow* joinToFinal = dynamic_cast<ControlFlow*>((*edgeIt));
  ASSERT_TRUE(joinToFinal->getSource() == joinNode);
  ASSERT_TRUE(joinNode->outgoing.front() = joinToFinal);
  ASSERT_TRUE(joinToFinal->getTarget() == finalNode);
  ASSERT_TRUE(finalNode->incoming.front() == joinToFinal);
}