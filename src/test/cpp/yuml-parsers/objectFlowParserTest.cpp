#include "gtest/gtest.h"
#include "yuml-parsers/modelParser.h"
#include "uml/activity.h"
#include "uml/objectNode.h"
#include "uml/primitiveType.h"
#include "uml/action.h"

using namespace UML;

class ObjectFlowParserTest : public ::testing::Test {

};

TEST_F(ObjectFlowParserTest, ParseObjectNodeToActionTest) {
    // Setup
    ModelParser objectToActionParser = ModelParser::createNewParser();
    YAML::Node objectToActionNode = YAML::LoadFile("../../../../../src/test/yml/objectFlowTests/objectNodeToAction.yml");

    // Test
    // Activity
    ASSERT_NO_THROW(objectToActionParser.parse(objectToActionNode));
    ASSERT_TRUE(objectToActionParser.theEl->ownedElements.size() == 1);
    ASSERT_TRUE(objectToActionParser.theEl->ownedElements.front()->getElementType() == ElementType::ACTIVITY);
    ASSERT_TRUE(dynamic_cast<Activity*>(objectToActionParser.theEl->ownedElements.front())->nodes.size() ==3); 
    ASSERT_TRUE(dynamic_cast<Activity*>(objectToActionParser.theEl->ownedElements.front())->edges.size() ==1); 

    // Object Node
    ASSERT_TRUE(dynamic_cast<Activity*>(objectToActionParser.theEl->ownedElements.front())->nodes.front()->uuid == boost::lexical_cast<boost::uuids::uuid>("9cdae5be-6b75-4284-b1e3-445fcb3dd071"));
    ASSERT_TRUE(dynamic_cast<Activity*>(objectToActionParser.theEl->ownedElements.front())->nodes.front()->getElementType() == ElementType::OBJECT_NODE);
    ASSERT_TRUE(dynamic_cast<ObjectNode*>(dynamic_cast<Activity*>(objectToActionParser.theEl->ownedElements.front())->nodes.front())->getType() != NULL);
    ASSERT_TRUE(dynamic_cast<ObjectNode*>(dynamic_cast<Activity*>(objectToActionParser.theEl->ownedElements.front())->nodes.front())->getType()->isPrimitive());
    ASSERT_TRUE(dynamic_cast<PrimitiveType*>(dynamic_cast<ObjectNode*>(dynamic_cast<Activity*>(objectToActionParser.theEl->ownedElements.front())->nodes.front())->getType())->getPrimitiveType() == PrimitiveType::Primitive::INT);
    ASSERT_TRUE(dynamic_cast<Activity*>(objectToActionParser.theEl->ownedElements.front())->nodes.front()->outgoing.size() == 1);
    ASSERT_TRUE(dynamic_cast<Activity*>(objectToActionParser.theEl->ownedElements.front())->nodes.front()->outgoing.front()->getElementType() == ElementType::OBJECT_FLOW);

    // Object Flow
    ASSERT_EQ(dynamic_cast<Activity*>(objectToActionParser.theEl->ownedElements.front())->nodes.front()->outgoing.front()->uuid, dynamic_cast<Activity*>(objectToActionParser.theEl->ownedElements.front())->edges.front()->uuid);
    ASSERT_EQ(dynamic_cast<Activity*>(objectToActionParser.theEl->ownedElements.front())->edges.front()->getSource()->uuid, dynamic_cast<Activity*>(objectToActionParser.theEl->ownedElements.front())->nodes.front()->uuid);

    // Action
    list<ActivityNode*>::iterator nodeIt = dynamic_cast<Activity*>(objectToActionParser.theEl->ownedElements.front())->nodes.begin();
    ++nodeIt;
    ASSERT_TRUE((*nodeIt)->getElementType() == ElementType::ACTION);
    Action* act = dynamic_cast<Action*>(*nodeIt);
    ASSERT_TRUE(act->inputs.size() == 1);
    ASSERT_TRUE(act->inputs.front()->uuid == boost::lexical_cast<boost::uuids::uuid>("7d4b6b0b-f6c2-4670-868c-87709cede18e"));

    // Input Pin
    InputPin* inputPin = dynamic_cast<InputPin*>(act->inputs.front());
    ASSERT_EQ(inputPin->uuid, dynamic_cast<Activity*>(objectToActionParser.theEl->ownedElements.front())->nodes.back()->uuid);
    ASSERT_TRUE(inputPin->getType() != NULL);
    ASSERT_TRUE(inputPin->getType()->isPrimitive());
    ASSERT_TRUE(dynamic_cast<PrimitiveType*>(inputPin->getType())->getPrimitiveType() == PrimitiveType::Primitive::INT);
    ASSERT_TRUE(inputPin->incoming.size() == 1);
    ASSERT_EQ(inputPin->incoming.front()->uuid, dynamic_cast<Activity*>(objectToActionParser.theEl->ownedElements.front())->edges.front()->uuid);
    ASSERT_EQ(inputPin->uuid, dynamic_cast<Activity*>(objectToActionParser.theEl->ownedElements.front())->edges.front()->getTarget()->uuid);
}

TEST_F(ObjectFlowParserTest, ParseBackwardsOutputPinTest) {
    // Setup
    ModelParser backwardsOutputParser = ModelParser::createNewParser();
    YAML::Node backwardsOutputNode = YAML::LoadFile("../../../../../src/test/yml/objectFlowTests/backwardsOutput.yml");

    // Test
    ASSERT_NO_THROW(backwardsOutputParser.parse(backwardsOutputNode));
}