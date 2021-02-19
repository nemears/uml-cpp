#include "gtest/gtest.h"
#include "yuml-parsers/modelParser.h"
#include "uml/activity.h"
#include "uml/objectNode.h"

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

    // Object Node
    ASSERT_TRUE(dynamic_cast<Activity*>(objectToActionParser.theEl->ownedElements.front())->nodes.front()->uuid == boost::lexical_cast<boost::uuids::uuid>("9cdae5be-6b75-4284-b1e3-445fcb3dd071"));
    ASSERT_TRUE(dynamic_cast<Activity*>(objectToActionParser.theEl->ownedElements.front())->nodes.front()->getElementType() == ElementType::OBJECT_NODE);
    ASSERT_TRUE(dynamic_cast<ObjectNode*>(dynamic_cast<Activity*>(objectToActionParser.theEl->ownedElements.front())->nodes.front())->getType() != NULL);

    /**
     *  TODO test me!!!
     */
}