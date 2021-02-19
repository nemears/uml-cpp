#include "gtest/gtest.h"
#include "yuml-parsers/modelParser.h"
#include "uml/activity.h"
#include "uml/action.h"
#include "uml/controlFlow.h"

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
    ASSERT_EQ(dynamic_cast<Activity*>(actionToActionFlowParser.theEl->ownedElements.front())->uuid, dynamic_cast<Activity*>(actionToActionFlowParser.theEl->ownedElements.front())->nodes.front()->getActivity()->uuid);
    
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

TEST_F(ControlFlowParserTest, ActionToSelfTest) {
    // Setup
    ModelParser actionToSelfFlowParser = ModelParser::createNewParser();
    YAML::Node actionToSelfFlowNode = YAML::LoadFile("../../../../../src/test/yml/controlFlowTests/actionToSelf.yml");

    // Test
    ASSERT_NO_THROW(actionToSelfFlowParser.parse(actionToSelfFlowNode));
    ASSERT_TRUE(actionToSelfFlowParser.theEl->ownedElements.size() == 1);

    // Test activity
    ASSERT_TRUE(actionToSelfFlowParser.theEl->ownedElements.front()->getElementType() == ElementType::ACTIVITY);
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToSelfFlowParser.theEl->ownedElements.front())->nodes.size() == 1);
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToSelfFlowParser.theEl->ownedElements.front())->nodes.front()->uuid == boost::lexical_cast<boost::uuids::uuid>("2c2fc607-48c6-4879-800b-7e659441cba1"));
    
    // Test first action
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToSelfFlowParser.theEl->ownedElements.front())->nodes.front()->outgoing.size() == 1);
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToSelfFlowParser.theEl->ownedElements.front())->nodes.front()->outgoing.front() != 0);
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToSelfFlowParser.theEl->ownedElements.front())->nodes.front()->incoming.size() == 1);
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToSelfFlowParser.theEl->ownedElements.front())->nodes.front()->incoming.front() != 0);
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToSelfFlowParser.theEl->ownedElements.front())->nodes.front()->incoming.front()->uuid == dynamic_cast<Activity*>(actionToSelfFlowParser.theEl->ownedElements.front())->edges.front()->uuid);

    // Test control flow
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToSelfFlowParser.theEl->ownedElements.front())->nodes.front()->outgoing.front()->getSource()->uuid == boost::lexical_cast<boost::uuids::uuid>("2c2fc607-48c6-4879-800b-7e659441cba1"));
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToSelfFlowParser.theEl->ownedElements.front())->nodes.front()->outgoing.front()->getTarget()->uuid == boost::lexical_cast<boost::uuids::uuid>("2c2fc607-48c6-4879-800b-7e659441cba1"));

}

TEST_F(ControlFlowParserTest, EmitActionToSelfTest) {
    // Setup
    Model m;
    m.setID("16c345b4-5ae2-41ca-a0e7-a9c386ac941d");
    Activity a;
    a.setID("563f4740-e107-4d08-8618-2489f0fe1865");
    Action act;
    act.setID("c0ab87cc-d00b-4afb-9558-538253b442b2");
    ControlFlow cf;
    cf.setID("7d18ee42-82c6-4f52-8ec4-fab67a75ff35");
    cf.setSource(&act);
    cf.setTarget(&act);
    act.incoming.push_back(&cf);
    act.outgoing.push_back(&cf);
    a.nodes.push_back(&act);
    a.edges.push_back(&cf);
    m.ownedElements.push_back(&a);
    ModelParser emitActionToSelfParser = ModelParser::createNewParser();
    string expectedEmit = R""""(model:
  id: 16c345b4-5ae2-41ca-a0e7-a9c386ac941d
  children:
    - activity:
        id: 563f4740-e107-4d08-8618-2489f0fe1865
        nodes:
          - action:
              id: c0ab87cc-d00b-4afb-9558-538253b442b2
              incoming:
                - 7d18ee42-82c6-4f52-8ec4-fab67a75ff35
              outgoing:
                - 7d18ee42-82c6-4f52-8ec4-fab67a75ff35
        edges:
          - controlFlow:
              id: 7d18ee42-82c6-4f52-8ec4-fab67a75ff35
              source: c0ab87cc-d00b-4afb-9558-538253b442b2
              target: c0ab87cc-d00b-4afb-9558-538253b442b2)"""";

    // Test
    string generatedEmit;
    YAML::Emitter emitter;
    ASSERT_NO_THROW(emitActionToSelfParser.emit(emitter, &m));
    generatedEmit = emitter.c_str();
    cout << generatedEmit << '\n';
    ASSERT_TRUE(emitter.good());
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(ControlFlowParserTest, EmitActionToActionTest) {
    Model m;
    m.setID("16c345b4-5ae2-41ca-a0e7-a9c386ac941d");
    Activity a;
    a.setID("563f4740-e107-4d08-8618-2489f0fe1865");
    Action act;
    act.setID("c0ab87cc-d00b-4afb-9558-538253b442b2");
    Action act2;
    act2.setID("d9ab2f06-4c2c-4330-9e1b-7eaee423a66a");
    ControlFlow cf;
    cf.setID("7d18ee42-82c6-4f52-8ec4-fab67a75ff35");
    cf.setSource(&act);
    cf.setTarget(&act2);
    act.outgoing.push_back(&cf);
    act2.incoming.push_back(&cf);
    a.nodes.push_back(&act);
    a.nodes.push_back(&act2);
    a.edges.push_back(&cf);
    m.ownedElements.push_back(&a);
    ModelParser emitActionToActionParser = ModelParser::createNewParser();
    string expectedEmit = R""""(model:
  id: 16c345b4-5ae2-41ca-a0e7-a9c386ac941d
  children:
    - activity:
        id: 563f4740-e107-4d08-8618-2489f0fe1865
        nodes:
          - action:
              id: c0ab87cc-d00b-4afb-9558-538253b442b2
              outgoing:
                - 7d18ee42-82c6-4f52-8ec4-fab67a75ff35
          - action:
              id: d9ab2f06-4c2c-4330-9e1b-7eaee423a66a
              incoming:
                - 7d18ee42-82c6-4f52-8ec4-fab67a75ff35
        edges:
          - controlFlow:
              id: 7d18ee42-82c6-4f52-8ec4-fab67a75ff35
              source: c0ab87cc-d00b-4afb-9558-538253b442b2
              target: d9ab2f06-4c2c-4330-9e1b-7eaee423a66a)"""";

    // Test
    string generatedEmit;
    YAML::Emitter emitter;
    ASSERT_NO_THROW(emitActionToActionParser.emit(emitter, &m));
    generatedEmit = emitter.c_str();
    cout << generatedEmit << '\n';
    ASSERT_TRUE(emitter.good());
    ASSERT_EQ(expectedEmit, generatedEmit);
}