#include "gtest/gtest.h"
#include "yuml-parsers/modelParser.h"
#include "uml/activity.h"
#include "uml/objectNode.h"
#include "uml/primitiveType.h"
#include "uml/initialNode.h"
#include "uml/decisionNode.h"
#include "uml/literalBool.h"

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
    ASSERT_TRUE(decisionNode->outgoing.front()->getGuard() != NULL);
    ASSERT_TRUE(decisionNode->outgoing.front()->getGuard()->getElementType() == ElementType::LITERAL_BOOL);
    ASSERT_TRUE(dynamic_cast<LiteralBool*>(decisionNode->outgoing.front()->getGuard())->getValue());
    ASSERT_TRUE(decisionNode->outgoing.back()->getGuard() != NULL);
    ASSERT_TRUE(decisionNode->outgoing.back()->getGuard()->getElementType() == ElementType::LITERAL_BOOL);
    ASSERT_TRUE(dynamic_cast<LiteralBool*>(decisionNode->outgoing.back()->getGuard())->getValue() == false);

    // Delete
    ModelParser::deleteParser(&decisionNodeGuardParser);
}