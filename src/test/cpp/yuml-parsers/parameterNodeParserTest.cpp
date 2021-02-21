#include "gtest/gtest.h"
#include "yuml-parsers/modelParser.h"
#include "uml/parameterNode.h"
#include "uml/activity.h"
#include "uml/primitiveType.h"
#include "uml/objectFlow.h"

using namespace UML;

class ParameterNodeTest : public ::testing::Test {

};

TEST_F(ParameterNodeTest, ParseParameterNodeTest) {
    // Setup
    ModelParser parameterNodeParser = ModelParser::createNewParser();
    YAML::Node parameterNodeNode = YAML::LoadFile("../../../../../src/test/yml/parameterNodeTests/parameterNode.yml");

    // Test
    ASSERT_NO_THROW(parameterNodeParser.parse(parameterNodeNode));

    // Activity
    ASSERT_TRUE(parameterNodeParser.theEl->ownedElements.size() == 1);
    ASSERT_TRUE(parameterNodeParser.theEl->ownedElements.front()->getElementType() == ElementType::ACTIVITY);
    Activity* activity = dynamic_cast<Activity*>(parameterNodeParser.theEl->ownedElements.front());
    ASSERT_TRUE(activity->parameters.size() == 1);
    ASSERT_TRUE(activity->nodes.size() == 2);
    ASSERT_TRUE(activity->edges.size() == 1);

    // Parameter
    Parameter* param = activity->parameters.front();
    cout << boost::lexical_cast<string>(param->uuid) << endl;
    ASSERT_TRUE(param->uuid == boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033"));
    ASSERT_TRUE(param->getType()->isPrimitive());
    ASSERT_TRUE(dynamic_cast<PrimitiveType*>(param->getType())->getPrimitiveType() == PrimitiveType::Primitive::INT);
    ASSERT_TRUE(param->getDirection() == ParameterDirectionKind::IN);

    // Parameter Node
    ASSERT_TRUE(activity->nodes.front()->getElementType() == ElementType::PARAMETER_NODE);
    ParameterNode* paramNode = dynamic_cast<ParameterNode*>(activity->nodes.front());
    ASSERT_TRUE(paramNode->uuid == boost::lexical_cast<boost::uuids::uuid>("2c2fc607-48c6-4879-800b-7e659441cba1"));
    ASSERT_TRUE(paramNode->getParameter() != NULL);
    ASSERT_TRUE(paramNode->getParameter()->uuid == param->uuid);
    ASSERT_TRUE(paramNode->getType() != NULL);
    ASSERT_TRUE(paramNode->getType()->isPrimitive());
    ASSERT_TRUE(dynamic_cast<PrimitiveType*>(paramNode->getType())->getPrimitiveType() == PrimitiveType::Primitive::INT);
    ASSERT_TRUE(paramNode->outgoing.size() == 1);
    ASSERT_TRUE(paramNode->outgoing.front()->getElementType() == ElementType::OBJECT_FLOW);
    ASSERT_TRUE(paramNode->outgoing.front()->uuid == boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35"));

    // Object Node
    ASSERT_TRUE(activity->nodes.back()->getElementType() == ElementType::OBJECT_NODE);
    ObjectNode* objNode = dynamic_cast<ObjectNode*>(activity->nodes.back());
    ASSERT_TRUE(objNode->uuid == boost::lexical_cast<boost::uuids::uuid>("7d4b6b0b-f6c2-4670-868c-87709cede18e"));
    ASSERT_TRUE(objNode->getType() != NULL);
    ASSERT_TRUE(objNode->getType()->isPrimitive());
    ASSERT_TRUE(dynamic_cast<PrimitiveType*>(objNode->getType())->getPrimitiveType() == PrimitiveType::Primitive::INT);
    ASSERT_TRUE(objNode->incoming.size() == 1);

    // Object Flow
    ASSERT_TRUE(activity->edges.front()->getElementType() == ElementType::OBJECT_FLOW);
    ObjectFlow* objFlow = dynamic_cast<ObjectFlow*>(activity->edges.front());
    ASSERT_TRUE(objFlow->uuid == boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35"));
    ASSERT_TRUE(objFlow->getSource()->uuid == paramNode->uuid);
    ASSERT_TRUE(objFlow->getTarget()->uuid == objNode->uuid);
}