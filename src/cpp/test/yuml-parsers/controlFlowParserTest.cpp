#include "gtest/gtest.h"
#include "test/yumlParsersTest.h"
#include "yuml-parsers/modelParser.h"
#include "uml/activity.h"
#include "uml/action.h"
#include "uml/controlFlow.h"
#include "uml/initialNode.h"
#include "uml/decisionNode.h"
#include "uml/joinNode.h"
#include "uml/finalNode.h"

using namespace UML;

class ControlFlowParserTest : public ::testing::Test {
  public:
        string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(ControlFlowParserTest, ActionToActionFlowTest) {
    // Setup
    ModelParser actionToActionFlowParser = ModelParser::createNewParser();
    YAML::Node actionToActionFlowNode = YAML::LoadFile(ymlPath + "controlFlowTests/actionToAction.yml");

    // Test
    ASSERT_NO_THROW(actionToActionFlowParser.parse(actionToActionFlowNode));
    ASSERT_TRUE(actionToActionFlowParser.theEl->getOwnedElements().size() == 1);

    // Test activity
    ASSERT_TRUE(actionToActionFlowParser.theEl->getOwnedElements().front()->getElementType() == ElementType::ACTIVITY);
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToActionFlowParser.theEl->getOwnedElements().front())->getNodes().size() == 2);
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToActionFlowParser.theEl->getOwnedElements().front())->getNodes().front()->getID() == boost::lexical_cast<boost::uuids::uuid>("2c2fc607-48c6-4879-800b-7e659441cba1"));
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToActionFlowParser.theEl->getOwnedElements().front())->getNodes().back()->getID() == boost::lexical_cast<boost::uuids::uuid>("7d4b6b0b-f6c2-4670-868c-87709cede18e"));
    
    // Test first action
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToActionFlowParser.theEl->getOwnedElements().front())->getNodes().front()->getOutgoing().size() == 1);
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToActionFlowParser.theEl->getOwnedElements().front())->getNodes().front()->getOutgoing().front() != 0);
    ASSERT_EQ(dynamic_cast<Activity*>(actionToActionFlowParser.theEl->getOwnedElements().front())->getID(), dynamic_cast<Activity*>(actionToActionFlowParser.theEl->getOwnedElements().front())->getNodes().front()->getActivity()->getID());
    
    // Test control flow
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToActionFlowParser.theEl->getOwnedElements().front())->getNodes().front()->getOutgoing().front()->getSource()->getID() == boost::lexical_cast<boost::uuids::uuid>("2c2fc607-48c6-4879-800b-7e659441cba1"));
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToActionFlowParser.theEl->getOwnedElements().front())->getNodes().front()->getOutgoing().front()->getTarget()->getID() == boost::lexical_cast<boost::uuids::uuid>("7d4b6b0b-f6c2-4670-868c-87709cede18e"));

    // Test second action
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToActionFlowParser.theEl->getOwnedElements().front())->getNodes().back()->getIncoming().size() == 1);
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToActionFlowParser.theEl->getOwnedElements().front())->getNodes().back()->getIncoming().front() != 0);
    ASSERT_EQ(dynamic_cast<Activity*>(actionToActionFlowParser.theEl->getOwnedElements().front())->getID(), dynamic_cast<Activity*>(actionToActionFlowParser.theEl->getOwnedElements().front())->getNodes().back()->getActivity()->getID());
    
    // Test control flow
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToActionFlowParser.theEl->getOwnedElements().front())->getNodes().back()->getIncoming().front()->getSource()->getID() == boost::lexical_cast<boost::uuids::uuid>("2c2fc607-48c6-4879-800b-7e659441cba1"));
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToActionFlowParser.theEl->getOwnedElements().front())->getNodes().back()->getIncoming().front()->getTarget()->getID() == boost::lexical_cast<boost::uuids::uuid>("7d4b6b0b-f6c2-4670-868c-87709cede18e"));
}

TEST_F(ControlFlowParserTest, ActionToSelfTest) {
    // Setup
    ModelParser actionToSelfFlowParser = ModelParser::createNewParser();
    YAML::Node actionToSelfFlowNode = YAML::LoadFile(ymlPath + "controlFlowTests/actionToSelf.yml");

    // Test
    ASSERT_NO_THROW(actionToSelfFlowParser.parse(actionToSelfFlowNode));
    ASSERT_TRUE(actionToSelfFlowParser.theEl->getOwnedElements().size() == 1);

    // Test activity
    ASSERT_TRUE(actionToSelfFlowParser.theEl->getOwnedElements().front()->getElementType() == ElementType::ACTIVITY);
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToSelfFlowParser.theEl->getOwnedElements().front())->getNodes().size() == 1);
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToSelfFlowParser.theEl->getOwnedElements().front())->getNodes().front()->getID() == boost::lexical_cast<boost::uuids::uuid>("2c2fc607-48c6-4879-800b-7e659441cba1"));
    
    // Test first action
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToSelfFlowParser.theEl->getOwnedElements().front())->getNodes().front()->getOutgoing().size() == 1);
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToSelfFlowParser.theEl->getOwnedElements().front())->getNodes().front()->getOutgoing().front() != 0);
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToSelfFlowParser.theEl->getOwnedElements().front())->getNodes().front()->getIncoming().size() == 1);
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToSelfFlowParser.theEl->getOwnedElements().front())->getNodes().front()->getIncoming().front() != 0);
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToSelfFlowParser.theEl->getOwnedElements().front())->getNodes().front()->getIncoming().front()->getID() == dynamic_cast<Activity*>(actionToSelfFlowParser.theEl->getOwnedElements().front())->getEdges().front()->getID());

    // Test control flow
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToSelfFlowParser.theEl->getOwnedElements().front())->getNodes().front()->getOutgoing().front()->getSource()->getID() == boost::lexical_cast<boost::uuids::uuid>("2c2fc607-48c6-4879-800b-7e659441cba1"));
    ASSERT_TRUE(dynamic_cast<Activity*>(actionToSelfFlowParser.theEl->getOwnedElements().front())->getNodes().front()->getOutgoing().front()->getTarget()->getID() == boost::lexical_cast<boost::uuids::uuid>("2c2fc607-48c6-4879-800b-7e659441cba1"));

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
    act.getIncoming().add(cf);
    act.getOutgoing().add(cf);
    a.getNodes().add(act);
    a.getEdges().add(cf);
    m.getOwnedElements().add(a);
    ModelParser emitActionToSelfParser = ModelParser::createNewParser();
    string expectedEmit = R""""(model:
  id: 16c345b4-5ae2-41ca-a0e7-a9c386ac941d
  children:
    - activity:
        id: 563f4740-e107-4d08-8618-2489f0fe1865
        children:
          - c0ab87cc-d00b-4afb-9558-538253b442b2
          - 7d18ee42-82c6-4f52-8ec4-fab67a75ff35
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
    act.getOutgoing().add(cf);
    act2.getIncoming().add(cf);
    a.getNodes().add(act);
    a.getNodes().add(act2);
    a.getEdges().add(cf);
    m.getOwnedElements().add(a);
    ModelParser emitActionToActionParser = ModelParser::createNewParser();
    string expectedEmit = R""""(model:
  id: 16c345b4-5ae2-41ca-a0e7-a9c386ac941d
  children:
    - activity:
        id: 563f4740-e107-4d08-8618-2489f0fe1865
        children:
          - c0ab87cc-d00b-4afb-9558-538253b442b2
          - d9ab2f06-4c2c-4330-9e1b-7eaee423a66a
          - 7d18ee42-82c6-4f52-8ec4-fab67a75ff35
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

TEST_F(ControlFlowParserTest, EmitDecsionAndJoinTest) {
  Model m;
  m.setID("2f64bc10-4517-4b97-a021-a38379f55656");
  Activity a;
  a.setID("fa128118-7efc-4881-8ed9-38bbed3110f2");
  InitialNode i;
  i.setID("b4b80cfd-4649-4b94-b17d-9f6bd67f2fd8");
  DecisionNode d;
  d.setID("4fab86da-e35b-441d-a1ea-0d02ecb94133");
  Action act1;
  act1.setID("ac8ffe61-ee91-4331-9cdf-87b03f89a895");
  Action act2;
  act2.setID("59125ce2-89cb-4ccc-84f4-b311694153bb");
  JoinNode j;
  j.setID("606a8afb-b620-46e1-80fd-cdeea48af9bd");
  FinalNode f;
  f.setID("f0aa7518-8a8e-44a7-b06b-fc7da5616873");
  ControlFlow i2d;
  i2d.setID("76a2bc3b-7135-4de5-b22f-b0f4d6b45d2e");
  ControlFlow d2a1;
  d2a1.setID("5b2299a5-924f-4a80-a30c-de1992f9dc75");
  ControlFlow d2a2;
  d2a2.setID("c0c38491-d273-498e-962e-dc8125565884");
  ControlFlow a12j;
  a12j.setID("1937bc94-db2d-47f8-9615-2d28fc408094");
  ControlFlow a22j;
  a22j.setID("5a3b3462-3f0a-47a3-8123-b1005954c2d1");
  ControlFlow j2f;
  j2f.setID("7f85ebe1-3a0c-4bbb-95e5-ae7e9f512caa");
  i.getOutgoing().add(i2d);
  d.getIncoming().add(i2d);
  i2d.setSource(&i);
  i2d.setTarget(&d);
  d.getOutgoing().add(d2a1);
  d.getOutgoing().add(d2a2);
  d2a1.setSource(&d);
  d2a2.setSource(&d);
  act1.getIncoming().add(d2a1);
  d2a1.setTarget(&act1);
  act2.getIncoming().add(d2a2);
  d2a2.setTarget(&act2);
  act1.getOutgoing().add(a12j);
  a12j.setSource(&act1);
  act2.getOutgoing().add(a22j);
  a22j.setSource(&act2);
  j.getIncoming().add(a12j);
  j.getIncoming().add(a22j);
  a12j.setTarget(&j);
  a22j.setTarget(&j);
  j.getOutgoing().add(j2f);
  j2f.setSource(&j);
  f.getIncoming().add(j2f);
  j2f.setTarget(&f);
  a.getNodes().add(i);
  a.getNodes().add(d);
  a.getNodes().add(act1);
  a.getNodes().add(act2);
  a.getNodes().add(j);
  a.getNodes().add(f);
  a.getEdges().add(i2d);
  a.getEdges().add(d2a1);
  a.getEdges().add(d2a2);
  a.getEdges().add(a12j);
  a.getEdges().add(a22j);
  a.getEdges().add(j2f);
  m.getOwnedElements().add(a);

  ModelParser emitActionToActionParser = ModelParser::createNewParser();
    string expectedEmit = R""""(model:
  id: 2f64bc10-4517-4b97-a021-a38379f55656
  children:
    - activity:
        id: fa128118-7efc-4881-8ed9-38bbed3110f2
        children:
          - b4b80cfd-4649-4b94-b17d-9f6bd67f2fd8
          - 4fab86da-e35b-441d-a1ea-0d02ecb94133
          - ac8ffe61-ee91-4331-9cdf-87b03f89a895
          - 59125ce2-89cb-4ccc-84f4-b311694153bb
          - 606a8afb-b620-46e1-80fd-cdeea48af9bd
          - f0aa7518-8a8e-44a7-b06b-fc7da5616873
          - 76a2bc3b-7135-4de5-b22f-b0f4d6b45d2e
          - 5b2299a5-924f-4a80-a30c-de1992f9dc75
          - c0c38491-d273-498e-962e-dc8125565884
          - 1937bc94-db2d-47f8-9615-2d28fc408094
          - 5a3b3462-3f0a-47a3-8123-b1005954c2d1
          - 7f85ebe1-3a0c-4bbb-95e5-ae7e9f512caa
        nodes:
          - initialNode:
              id: b4b80cfd-4649-4b94-b17d-9f6bd67f2fd8
              outgoing:
                - 76a2bc3b-7135-4de5-b22f-b0f4d6b45d2e
          - decisionNode:
              id: 4fab86da-e35b-441d-a1ea-0d02ecb94133
              incoming:
                - 76a2bc3b-7135-4de5-b22f-b0f4d6b45d2e
              outgoing:
                - 5b2299a5-924f-4a80-a30c-de1992f9dc75
                - c0c38491-d273-498e-962e-dc8125565884
          - action:
              id: ac8ffe61-ee91-4331-9cdf-87b03f89a895
              incoming:
                - 5b2299a5-924f-4a80-a30c-de1992f9dc75
              outgoing:
                - 1937bc94-db2d-47f8-9615-2d28fc408094
          - action:
              id: 59125ce2-89cb-4ccc-84f4-b311694153bb
              incoming:
                - c0c38491-d273-498e-962e-dc8125565884
              outgoing:
                - 5a3b3462-3f0a-47a3-8123-b1005954c2d1
          - joinNode:
              id: 606a8afb-b620-46e1-80fd-cdeea48af9bd
              incoming:
                - 1937bc94-db2d-47f8-9615-2d28fc408094
                - 5a3b3462-3f0a-47a3-8123-b1005954c2d1
              outgoing:
                - 7f85ebe1-3a0c-4bbb-95e5-ae7e9f512caa
          - finalNode:
              id: f0aa7518-8a8e-44a7-b06b-fc7da5616873
              incoming:
                - 7f85ebe1-3a0c-4bbb-95e5-ae7e9f512caa
        edges:
          - controlFlow:
              id: 76a2bc3b-7135-4de5-b22f-b0f4d6b45d2e
              source: b4b80cfd-4649-4b94-b17d-9f6bd67f2fd8
              target: 4fab86da-e35b-441d-a1ea-0d02ecb94133
          - controlFlow:
              id: 5b2299a5-924f-4a80-a30c-de1992f9dc75
              source: 4fab86da-e35b-441d-a1ea-0d02ecb94133
              target: ac8ffe61-ee91-4331-9cdf-87b03f89a895
          - controlFlow:
              id: c0c38491-d273-498e-962e-dc8125565884
              source: 4fab86da-e35b-441d-a1ea-0d02ecb94133
              target: 59125ce2-89cb-4ccc-84f4-b311694153bb
          - controlFlow:
              id: 1937bc94-db2d-47f8-9615-2d28fc408094
              source: ac8ffe61-ee91-4331-9cdf-87b03f89a895
              target: 606a8afb-b620-46e1-80fd-cdeea48af9bd
          - controlFlow:
              id: 5a3b3462-3f0a-47a3-8123-b1005954c2d1
              source: 59125ce2-89cb-4ccc-84f4-b311694153bb
              target: 606a8afb-b620-46e1-80fd-cdeea48af9bd
          - controlFlow:
              id: 7f85ebe1-3a0c-4bbb-95e5-ae7e9f512caa
              source: 606a8afb-b620-46e1-80fd-cdeea48af9bd
              target: f0aa7518-8a8e-44a7-b06b-fc7da5616873)"""";

    // Test
    string generatedEmit;
    YAML::Emitter emitter;
    ASSERT_NO_THROW(emitActionToActionParser.emit(emitter, &m));
    generatedEmit = emitter.c_str();
    cout << generatedEmit << '\n';
    ASSERT_TRUE(emitter.good());
    ASSERT_EQ(expectedEmit, generatedEmit);
}