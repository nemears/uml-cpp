#include "gtest/gtest.h"
#include "yuml-parsers/modelParser.h"
#include "uml/activity.h"
#include "uml/objectNode.h"
#include "uml/primitiveType.h"
#include "uml/literalInt.h"
#include "uml/expression.h"

using namespace UML;

class ObjectNodeParserTest : public ::testing::Test {

};

TEST_F(ObjectNodeParserTest, ParseLiteralUpperBoundTest) {
    // Setup
    ModelParser literalUpperBoundParser = ModelParser::createNewParser();
    YAML::Node literalUpperBoundNode = YAML::LoadFile("../../../../../src/test/yml/objectNodeTests/literalUpperBound.yml");

    // Test
    ASSERT_TRUE(literalUpperBoundParser.parse(literalUpperBoundNode));
    ASSERT_TRUE(literalUpperBoundParser.theEl->ownedElements.size() == 1);
    ASSERT_TRUE(literalUpperBoundParser.theEl->ownedElements.front()->getElementType() == ElementType::ACTIVITY);

    // Activity
    Activity* activity = dynamic_cast<Activity*>(literalUpperBoundParser.theEl->ownedElements.front());
    ASSERT_TRUE(activity->nodes.size() == 1);
    ASSERT_TRUE(activity->edges.size() == 0);
    ASSERT_TRUE(activity->nodes.front()->getElementType() == ElementType::OBJECT_NODE);

    // Object Node
    ObjectNode* objNode = dynamic_cast<ObjectNode*>(activity->nodes.front());
    ASSERT_TRUE(objNode->getType() != NULL);
    ASSERT_TRUE(objNode->getType()->isPrimitive());
    ASSERT_TRUE(dynamic_cast<PrimitiveType*>(objNode->getType())->getPrimitiveType() == PrimitiveType::Primitive::INT);
    ASSERT_TRUE(objNode->getUpperBound() != NULL);
    ASSERT_TRUE(objNode->getUpperBound()->getElementType() == ElementType::LITERAL_INT);
    ASSERT_TRUE(dynamic_cast<LiteralInt*>(objNode->getUpperBound())->getValue() == 5);

    // Tear Down
    ModelParser::deleteParser(&literalUpperBoundParser);
}

TEST_F(ObjectNodeParserTest, ParseExpressionUpperBoundTest) {
    // Setup
    ModelParser expressionUpperBoundParser = ModelParser::createNewParser();
    YAML::Node expressionUpperBoundNode = YAML::LoadFile("../../../../../src/test/yml/objectNodeTests/expressionUpperBound.yml");

    // Test
    ASSERT_NO_THROW(expressionUpperBoundParser.parse(expressionUpperBoundNode));
    ASSERT_TRUE(expressionUpperBoundParser.theEl->ownedElements.size() == 1);
    ASSERT_TRUE(expressionUpperBoundParser.theEl->ownedElements.front()->getElementType() == ElementType::ACTIVITY);

    // Activity
    Activity* activity = dynamic_cast<Activity*>(expressionUpperBoundParser.theEl->ownedElements.front());
    ASSERT_TRUE(activity->nodes.size() == 1);
    ASSERT_TRUE(activity->edges.size() == 0);
    ASSERT_TRUE(activity->nodes.front()->getElementType() == ElementType::OBJECT_NODE);

    // Object Node
    ObjectNode* objNode = dynamic_cast<ObjectNode*>(activity->nodes.front());
    ASSERT_TRUE(objNode->getType() != NULL);
    ASSERT_TRUE(objNode->getType()->isPrimitive());
    ASSERT_TRUE(dynamic_cast<PrimitiveType*>(objNode->getType())->getPrimitiveType() == PrimitiveType::Primitive::INT);
    ASSERT_TRUE(objNode->getUpperBound() != NULL);
    ASSERT_TRUE(objNode->getUpperBound()->getElementType() == ElementType::EXPRESSION);
    ASSERT_TRUE(dynamic_cast<Expression*>(objNode->getUpperBound())->operands.size() == 1);
    ASSERT_TRUE(dynamic_cast<Expression*>(objNode->getUpperBound())->operands.front()->getElementType() == ElementType::LITERAL_INT);
    ASSERT_TRUE(dynamic_cast<LiteralInt*>(dynamic_cast<Expression*>(objNode->getUpperBound())->operands.front())->getValue() == 5);
    ASSERT_TRUE(dynamic_cast<Expression*>(objNode->getUpperBound())->getSymbol().compare("<") == 0);

    // Tear Down
    ModelParser::deleteParser(&expressionUpperBoundParser);
}