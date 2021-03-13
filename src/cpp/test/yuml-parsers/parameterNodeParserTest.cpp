#include "gtest/gtest.h"
#include "test/yumlParsersTest.h"
#include "yuml-parsers/modelParser.h"
#include "uml/parameterNode.h"
#include "uml/activity.h"
#include "uml/primitiveType.h"
#include "uml/objectFlow.h"

using namespace UML;

class ParameterNodeParserTest : public ::testing::Test {
   public:
        string ymlPath;
        void SetUp() override {
            ymlPath = YML_FILES_PATH;
        };
};

TEST_F(ParameterNodeParserTest, ParseParameterNodeTest) {
    // Setup
    ModelParser parameterNodeParser = ModelParser::createNewParser();
    YAML::Node parameterNodeNode = YAML::LoadFile(ymlPath + "parameterNodeTests/parameterNode.yml");

    // Test
    ASSERT_NO_THROW(parameterNodeParser.parse(parameterNodeNode));

    // Activity
    ASSERT_TRUE(parameterNodeParser.theEl->getOwnedElements().size() == 1);
    ASSERT_TRUE(parameterNodeParser.theEl->getOwnedElements().front()->getElementType() == ElementType::ACTIVITY);
    Activity* activity = dynamic_cast<Activity*>(parameterNodeParser.theEl->getOwnedElements().front());
    ASSERT_TRUE(activity->getParameters().size() == 1);
    ASSERT_TRUE(activity->nodes.size() == 2);
    ASSERT_TRUE(activity->edges.size() == 1);

    // Parameter
    Parameter* param = activity->getParameters().front();
    cout << boost::lexical_cast<string>(param->getID()) << endl;
    ASSERT_TRUE(param->getID() == boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033"));
    ASSERT_TRUE(param->getType()->isPrimitive());
    ASSERT_TRUE(dynamic_cast<PrimitiveType*>(param->getType())->getPrimitiveType() == PrimitiveType::Primitive::INT);
    ASSERT_TRUE(param->getDirection() == ParameterDirectionKind::IN);

    // Parameter Node
    ASSERT_TRUE(activity->nodes.front()->getElementType() == ElementType::PARAMETER_NODE);
    ParameterNode* paramNode = dynamic_cast<ParameterNode*>(activity->nodes.front());
    ASSERT_TRUE(paramNode->getID() == boost::lexical_cast<boost::uuids::uuid>("2c2fc607-48c6-4879-800b-7e659441cba1"));
    ASSERT_TRUE(paramNode->getParameter() != NULL);
    ASSERT_TRUE(paramNode->getParameter()->getID() == param->getID());
    ASSERT_TRUE(paramNode->getType() != NULL);
    ASSERT_TRUE(paramNode->getType()->isPrimitive());
    ASSERT_TRUE(dynamic_cast<PrimitiveType*>(paramNode->getType())->getPrimitiveType() == PrimitiveType::Primitive::INT);
    ASSERT_TRUE(paramNode->outgoing.size() == 1);
    ASSERT_TRUE(paramNode->outgoing.front()->getElementType() == ElementType::OBJECT_FLOW);
    ASSERT_TRUE(paramNode->outgoing.front()->getID() == boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35"));

    // Object Node
    ASSERT_TRUE(activity->nodes.back()->getElementType() == ElementType::OBJECT_NODE);
    ObjectNode* objNode = dynamic_cast<ObjectNode*>(activity->nodes.back());
    ASSERT_TRUE(objNode->getID() == boost::lexical_cast<boost::uuids::uuid>("7d4b6b0b-f6c2-4670-868c-87709cede18e"));
    ASSERT_TRUE(objNode->getType() != NULL);
    ASSERT_TRUE(objNode->getType()->isPrimitive());
    ASSERT_TRUE(dynamic_cast<PrimitiveType*>(objNode->getType())->getPrimitiveType() == PrimitiveType::Primitive::INT);
    ASSERT_TRUE(objNode->incoming.size() == 1);

    // Object Flow
    ASSERT_TRUE(activity->edges.front()->getElementType() == ElementType::OBJECT_FLOW);
    ObjectFlow* objFlow = dynamic_cast<ObjectFlow*>(activity->edges.front());
    ASSERT_TRUE(objFlow->getID() == boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35"));
    ASSERT_TRUE(objFlow->getSource()->getID() == paramNode->getID());
    ASSERT_TRUE(objFlow->getTarget()->getID() == objNode->getID());
}

TEST_F(ParameterNodeParserTest, EmitParameterNodeTest) {
    // Setup
    Model m;
    Activity a;
    Parameter param;
    ParameterNode paramNode;
    ObjectNode objNode;
    ObjectFlow objFlow;
    PrimitiveType pt;
    m.setID("1bfe131b-0d9a-4e6f-9a9b-1dae55626202");
    a.setID("4b9519d3-cfd4-4bda-b1dc-6c7d0f521647");
    param.setID("2f821a87-6a14-47a0-bf78-cf57e24876d6");
    paramNode.setID("f73c6d44-5436-4021-83a6-ed90345c1f5f");
    objNode.setID("25a0f5f5-0d02-40e6-a70e-c3c606fcfde0");
    objFlow.setID("0734d34f-066f-4029-97d9-e39ac2f40f2b");
    a.getParameters().add(param);
    pt.setPrimitiveType(PrimitiveType::Primitive::INT);
    param.setType(&pt);
    paramNode.setType(&pt);
    objNode.setType(&pt);
    param.setDirection(ParameterDirectionKind::IN);
    paramNode.setParameter(&param);
    paramNode.outgoing.push_back(&objFlow);
    objNode.incoming.push_back(&objFlow);
    objFlow.setSource(&paramNode);
    objFlow.setTarget(&objNode);
    a.nodes.push_back(&paramNode);
    a.nodes.push_back(&objNode);
    a.edges.push_back(&objFlow);
    m.getOwnedElements().add(a);
    string expectedEmit = R""""(model:
  id: 1bfe131b-0d9a-4e6f-9a9b-1dae55626202
  children:
    - activity:
        id: 4b9519d3-cfd4-4bda-b1dc-6c7d0f521647
        parameters:
          - parameter:
              type: INT
              id: 2f821a87-6a14-47a0-bf78-cf57e24876d6
              direction: IN
        nodes:
          - parameterNode:
              parameter: 2f821a87-6a14-47a0-bf78-cf57e24876d6
              id: f73c6d44-5436-4021-83a6-ed90345c1f5f
              type: INT
              outgoing:
                - 0734d34f-066f-4029-97d9-e39ac2f40f2b
          - objectNode:
              id: 25a0f5f5-0d02-40e6-a70e-c3c606fcfde0
              type: INT
              incoming:
                - 0734d34f-066f-4029-97d9-e39ac2f40f2b
        edges:
          - objectFlow:
              id: 0734d34f-066f-4029-97d9-e39ac2f40f2b
              source: f73c6d44-5436-4021-83a6-ed90345c1f5f
              target: 25a0f5f5-0d02-40e6-a70e-c3c606fcfde0)"""";

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