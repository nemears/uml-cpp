#include "gtest/gtest.h"
#include "test/yumlParsersTest.h"
#include "yuml-parsers/modelParser.h"
#include "uml/activity.h"
#include "uml/objectNode.h"
#include "uml/primitiveType.h"
#include "uml/action.h"
#include "uml/objectFlow.h"

using namespace UML;

class ObjectFlowParserTest : public ::testing::Test {
  public:
        string ymlPath;
        void SetUp() override {
            ymlPath = YML_FILES_PATH;
        };
};

TEST_F(ObjectFlowParserTest, ParseObjectNodeToActionTest) {
    // Setup
    ModelParser objectToActionParser = ModelParser::createNewParser();
    YAML::Node objectToActionNode = YAML::LoadFile(ymlPath + "objectFlowTests/objectNodeToAction.yml");

    // Test
    // Activity
    ASSERT_NO_THROW(objectToActionParser.parse(objectToActionNode));
    ASSERT_TRUE(objectToActionParser.theEl->getOwnedElements().size() == 1);
    ASSERT_TRUE(objectToActionParser.theEl->getOwnedElements().front()->getElementType() == ElementType::ACTIVITY);
    ASSERT_TRUE(dynamic_cast<Activity*>(objectToActionParser.theEl->getOwnedElements().front())->nodes.size() ==3); 
    ASSERT_TRUE(dynamic_cast<Activity*>(objectToActionParser.theEl->getOwnedElements().front())->edges.size() ==1); 

    // Object Node
    ASSERT_TRUE(dynamic_cast<Activity*>(objectToActionParser.theEl->getOwnedElements().front())->nodes.front()->getID() == boost::lexical_cast<boost::uuids::uuid>("9cdae5be-6b75-4284-b1e3-445fcb3dd071"));
    ASSERT_TRUE(dynamic_cast<Activity*>(objectToActionParser.theEl->getOwnedElements().front())->nodes.front()->getElementType() == ElementType::OBJECT_NODE);
    ASSERT_TRUE(dynamic_cast<ObjectNode*>(dynamic_cast<Activity*>(objectToActionParser.theEl->getOwnedElements().front())->nodes.front())->getType() != NULL);
    ASSERT_TRUE(dynamic_cast<ObjectNode*>(dynamic_cast<Activity*>(objectToActionParser.theEl->getOwnedElements().front())->nodes.front())->getType()->isPrimitive());
    ASSERT_TRUE(dynamic_cast<PrimitiveType*>(dynamic_cast<ObjectNode*>(dynamic_cast<Activity*>(objectToActionParser.theEl->getOwnedElements().front())->nodes.front())->getType())->getPrimitiveType() == PrimitiveType::Primitive::INT);
    ASSERT_TRUE(dynamic_cast<Activity*>(objectToActionParser.theEl->getOwnedElements().front())->nodes.front()->outgoing.size() == 1);
    ASSERT_TRUE(dynamic_cast<Activity*>(objectToActionParser.theEl->getOwnedElements().front())->nodes.front()->outgoing.front()->getElementType() == ElementType::OBJECT_FLOW);

    // Object Flow
    ASSERT_EQ(dynamic_cast<Activity*>(objectToActionParser.theEl->getOwnedElements().front())->nodes.front()->outgoing.front()->getID(), dynamic_cast<Activity*>(objectToActionParser.theEl->getOwnedElements().front())->edges.front()->getID());
    ASSERT_EQ(dynamic_cast<Activity*>(objectToActionParser.theEl->getOwnedElements().front())->edges.front()->getSource()->getID(), dynamic_cast<Activity*>(objectToActionParser.theEl->getOwnedElements().front())->nodes.front()->getID());

    // Action
    list<ActivityNode*>::iterator nodeIt = dynamic_cast<Activity*>(objectToActionParser.theEl->getOwnedElements().front())->nodes.begin();
    ++nodeIt;
    ASSERT_TRUE((*nodeIt)->getElementType() == ElementType::ACTION);
    Action* act = dynamic_cast<Action*>(*nodeIt);
    ASSERT_TRUE(act->inputs.size() == 1);
    ASSERT_TRUE(act->inputs.front()->getID() == boost::lexical_cast<boost::uuids::uuid>("7d4b6b0b-f6c2-4670-868c-87709cede18e"));

    // Input Pin
    InputPin* inputPin = dynamic_cast<InputPin*>(act->inputs.front());
    ASSERT_EQ(inputPin->getID(), dynamic_cast<Activity*>(objectToActionParser.theEl->getOwnedElements().front())->nodes.back()->getID());
    ASSERT_TRUE(inputPin->getType() != NULL);
    ASSERT_TRUE(inputPin->getType()->isPrimitive());
    ASSERT_TRUE(dynamic_cast<PrimitiveType*>(inputPin->getType())->getPrimitiveType() == PrimitiveType::Primitive::INT);
    ASSERT_TRUE(inputPin->incoming.size() == 1);
    ASSERT_EQ(inputPin->incoming.front()->getID(), dynamic_cast<Activity*>(objectToActionParser.theEl->getOwnedElements().front())->edges.front()->getID());
    ASSERT_EQ(inputPin->getID(), dynamic_cast<Activity*>(objectToActionParser.theEl->getOwnedElements().front())->edges.front()->getTarget()->getID());
}

TEST_F(ObjectFlowParserTest, ParseBackwardsOutputPinTest) {
    // Setup
    ModelParser backwardsOutputParser = ModelParser::createNewParser();
    YAML::Node backwardsOutputNode = YAML::LoadFile(ymlPath + "objectFlowTests/backwardsOutput.yml");

    // Test
    ASSERT_NO_THROW(backwardsOutputParser.parse(backwardsOutputNode));

    //Activity
    ASSERT_TRUE(backwardsOutputParser.theEl->getOwnedElements().size() == 1);
    ASSERT_TRUE(backwardsOutputParser.theEl->getOwnedElements().front()->getElementType() == ElementType::ACTIVITY);
    ASSERT_TRUE(dynamic_cast<Activity*>(backwardsOutputParser.theEl->getOwnedElements().front())->nodes.size() ==3); 
    ASSERT_TRUE(dynamic_cast<Activity*>(backwardsOutputParser.theEl->getOwnedElements().front())->edges.size() ==1); 

    // Action
    ASSERT_TRUE(dynamic_cast<Activity*>(backwardsOutputParser.theEl->getOwnedElements().front())->nodes.front()->getElementType() == ElementType::ACTION);
    Action* act = dynamic_cast<Action*>(dynamic_cast<Activity*>(backwardsOutputParser.theEl->getOwnedElements().front())->nodes.front());
    ASSERT_TRUE(act->outputs.size() == 1);
    ASSERT_TRUE(act->outputs.front()->getID() == boost::lexical_cast<boost::uuids::uuid>("7d4b6b0b-f6c2-4670-868c-87709cede18e"));

    // OutputPin
    list<ActivityNode*>::iterator outIt = dynamic_cast<Activity*>(backwardsOutputParser.theEl->getOwnedElements().front())->nodes.begin();
    ++outIt;
    ASSERT_TRUE((*outIt)->getElementType() == ElementType::OUTPUT_PIN);
    OutputPin* op = dynamic_cast<OutputPin*>((*outIt));
    ASSERT_TRUE(op->getID() == act->outputs.front()->getID());
    ASSERT_TRUE(op->outgoing.size() == 1);
    ASSERT_TRUE(op->outgoing.front()->getID() == dynamic_cast<Activity*>(backwardsOutputParser.theEl->getOwnedElements().front())->edges.front()->getID());

    // ObjectFlow
    ASSERT_TRUE(op->outgoing.front()->getElementType() == ElementType::OBJECT_FLOW);
    ObjectFlow* of = dynamic_cast<ObjectFlow*>(op->outgoing.front());
    ASSERT_TRUE(of->getSource()->getID() == op->getID());
    ASSERT_TRUE(of->getID() == boost::lexical_cast<boost::uuids::uuid>("9cdae5be-6b75-4284-b1e3-445fcb3dd071"));

    // ObjectNode
    ++outIt;
    ASSERT_TRUE((*outIt)->getElementType() == ElementType::OBJECT_NODE);
    ObjectNode* ob = dynamic_cast<ObjectNode*>((*outIt));
    ASSERT_TRUE(ob->incoming.size() == 1);
    ASSERT_TRUE(ob->incoming.front()->getID() == of->getID());
    ASSERT_TRUE(of->getTarget()->getID() == ob->getID());
    ASSERT_TRUE(ob->getActivity()->getID() == backwardsOutputParser.theEl->getOwnedElements().front()->getID());
}

TEST_F(ObjectFlowParserTest, EmitObjectNodeToActionTest) {
    // Setup
    Model m;
    m.setID("16c345b4-5ae2-41ca-a0e7-a9c386ac941d");
    Activity a;
    a.setID("563f4740-e107-4d08-8618-2489f0fe1865");
    ObjectNode o;
    o.setID("c0ab87cc-d00b-4afb-9558-538253b442b2");
    Action act;
    act.setID("d9ab2f06-4c2c-4330-9e1b-7eaee423a66a");
    InputPin i;
    i.setID("7d18ee42-82c6-4f52-8ec4-fab67a75ff35");
    ObjectFlow of;
    of.setID("32bb88f4-1ee7-41f9-aa30-6403111061f2");
    act.inputs.push_back(&i);
    o.outgoing.push_back(&of);
    PrimitiveType pt;
    pt.setPrimitiveType(PrimitiveType::Primitive::INT);
    o.setType(&pt);
    i.incoming.push_back(&of);
    i.setType(&pt);
    of.setSource(&o);
    of.setTarget(&i);
    a.nodes.push_back(&o);
    a.nodes.push_back(&act);
    a.nodes.push_back(&i);
    a.edges.push_back(&of);
    m.getOwnedElements().add(a);
    ModelParser emitObjectNodeToActionParser = ModelParser::createNewParser();
    string expectedEmit = R""""(model:
  id: 16c345b4-5ae2-41ca-a0e7-a9c386ac941d
  children:
    - activity:
        id: 563f4740-e107-4d08-8618-2489f0fe1865
        nodes:
          - objectNode:
              id: c0ab87cc-d00b-4afb-9558-538253b442b2
              type: INT
              outgoing:
                - 32bb88f4-1ee7-41f9-aa30-6403111061f2
          - action:
              id: d9ab2f06-4c2c-4330-9e1b-7eaee423a66a
              inputs:
                - 7d18ee42-82c6-4f52-8ec4-fab67a75ff35
          - inputPin:
              id: 7d18ee42-82c6-4f52-8ec4-fab67a75ff35
              type: INT
              incoming:
                - 32bb88f4-1ee7-41f9-aa30-6403111061f2
        edges:
          - objectFlow:
              id: 32bb88f4-1ee7-41f9-aa30-6403111061f2
              source: c0ab87cc-d00b-4afb-9558-538253b442b2
              target: 7d18ee42-82c6-4f52-8ec4-fab67a75ff35)"""";

    // Test
    string generatedEmit;
    YAML::Emitter emitter;
    ASSERT_NO_THROW(emitObjectNodeToActionParser.emit(emitter, &m));
    generatedEmit = emitter.c_str();
    cout << generatedEmit << '\n';
    ASSERT_TRUE(emitter.good());
    ASSERT_EQ(expectedEmit, generatedEmit);
}