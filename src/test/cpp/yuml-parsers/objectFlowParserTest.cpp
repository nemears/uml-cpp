#include "gtest/gtest.h"
#include "yuml-parsers/modelParser.h"
#include "uml/activity.h"

using namespace UML;

class ObjectFlowParserTest : public ::testing::Test {

};

TEST_F(ObjectFlowParserTest, ParseObjectNodeToActionTest) {
    // Setup
    ModelParser objectToActionParser = ModelParser::createNewParser();
    YAML::Node objectToActionNode = YAML::LoadFile("../../../../../src/test/yml/objectFlowTests/objectNodeToAction.yml");

    // Test
    ASSERT_NO_THROW(objectToActionParser.parse(objectToActionNode));
    ASSERT_TRUE(objectToActionParser.theEl->ownedElements.size() == 1);
    ASSERT_TRUE(objectToActionParser.theEl->ownedElements.front()->getElementType() == ElementType::ACTIVITY);
    /** TODO this should be 3 but inputPin is parsed inside actions scope, 
     *  TODO Element::getOwner()  ActivityNode::OwningActivity() etc
     *  TODO above methods implementatino in parser
     */
    ASSERT_TRUE(dynamic_cast<Activity*>(objectToActionParser.theEl->ownedElements.front())->nodes.size() ==2); 
    /**
     *  TODO test me!!!
     */
}