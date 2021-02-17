#include "gtest/gtest.h"
#include "yuml-parsers/modelParser.h"
#include "uml/activity.h"

using namespace UML;

class ControlFlowParserTest : public ::testing::Test {

};

TEST_F(ControlFlowParserTest, ActionToActionFlowTest) {
    // Setup
    ModelParser actionToActionFlowParser = ModelParser::createNewParser();
    YAML::Node actionToActionFlowNode = YAML::LoadFile("../../../../../src/test/yml/controlFlowTests/actionToAction.yml");

    // Test
    ASSERT_NO_THROW(actionToActionFlowParser.parse(actionToActionFlowNode));
    ASSERT_TRUE(actionToActionFlowParser.theEl->ownedElements.size() == 1);

    // Test activity
    ASSERT_TRUE(actionToActionFlowParser.theEl->ownedElements.front()->getElementType() == ElementType::ACTIVITY);
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToActionFlowParser.theEl->ownedElements.front())->nodes.size() == 2);
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToActionFlowParser.theEl->ownedElements.front())->nodes.front()->uuid == boost::lexical_cast<boost::uuids::uuid>("2c2fc607-48c6-4879-800b-7e659441cba1"));
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToActionFlowParser.theEl->ownedElements.front())->nodes.back()->uuid == boost::lexical_cast<boost::uuids::uuid>("7d4b6b0b-f6c2-4670-868c-87709cede18e"));
    
    // Test first action
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToActionFlowParser.theEl->ownedElements.front())->nodes.front()->outgoing.size() == 1);
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToActionFlowParser.theEl->ownedElements.front())->nodes.front()->outgoing.front() != 0);

    // Test control flow
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToActionFlowParser.theEl->ownedElements.front())->nodes.front()->outgoing.front()->getSource()->uuid == boost::lexical_cast<boost::uuids::uuid>("2c2fc607-48c6-4879-800b-7e659441cba1"));
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToActionFlowParser.theEl->ownedElements.front())->nodes.front()->outgoing.front()->getTarget()->uuid == boost::lexical_cast<boost::uuids::uuid>("7d4b6b0b-f6c2-4670-868c-87709cede18e"));

    // Test second action
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToActionFlowParser.theEl->ownedElements.front())->nodes.back()->incoming.size() == 1);
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToActionFlowParser.theEl->ownedElements.front())->nodes.back()->incoming.front() != 0);

    // Test control flow
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToActionFlowParser.theEl->ownedElements.front())->nodes.back()->incoming.front()->getSource()->uuid == boost::lexical_cast<boost::uuids::uuid>("2c2fc607-48c6-4879-800b-7e659441cba1"));
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToActionFlowParser.theEl->ownedElements.front())->nodes.back()->incoming.front()->getTarget()->uuid == boost::lexical_cast<boost::uuids::uuid>("7d4b6b0b-f6c2-4670-868c-87709cede18e"));
}