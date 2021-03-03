#include "gtest/gtest.h"
#include "yuml-parsers/modelParser.h"
#include "uml/activity.h"
#include "uml/objectNode.h"
#include "uml/primitiveType.h"
#include "uml/literalInt.h"
#include "uml/expression.h"
#include "uml/literalBool.h"

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

TEST_F(ObjectNodeParserTest, EmitLiteralUpperBoundTest) {
    // Setup
    Model m;
    m.setID("fa8cc066-6191-4903-a766-ee91d216c929");
    Activity a;
    a.setID("c7f09553-c13a-4d21-9e00-b9364c0aeaed");
    ObjectNode o;
    o.setID("9c16720e-a366-4eef-825a-d46b5232a1d5");
    PrimitiveType pt;
    pt.setPrimitiveType(PrimitiveType::Primitive::BOOL);
    LiteralBool b;
    b.setValue(true);
    o.setType(&pt);
    o.setUpperBound(&b);
    a.nodes.push_back(&o);
    o.setActivity(&a);
    m.ownedElements.push_back(&a);
    a.setOwner(&m);
    ModelParser emitLiteralUpperValue = ModelParser::createNewParser();
    string expectedEmit = R""""(model:
  id: fa8cc066-6191-4903-a766-ee91d216c929
  children:
    - activity:
        id: c7f09553-c13a-4d21-9e00-b9364c0aeaed
        nodes:
          - objectNode:
              id: 9c16720e-a366-4eef-825a-d46b5232a1d5
              type: BOOL
              upperBound:
                literalBool: true)"""";

    // Test
    string generatedEmit;
    YAML::Emitter emitter;
    ASSERT_NO_THROW(emitLiteralUpperValue.emit(emitter, &m));
    generatedEmit = emitter.c_str();
    cout << generatedEmit << '\n';
    ASSERT_TRUE(emitter.good());
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(ObjectNodeParserTest, EmitExpressionUpperBound) {
    // Setup
    Model m;
    m.setID("fa8cc066-6191-4903-a766-ee91d216c929");
    Activity a;
    a.setID("c7f09553-c13a-4d21-9e00-b9364c0aeaed");
    ObjectNode o;
    o.setID("9c16720e-a366-4eef-825a-d46b5232a1d5");
    PrimitiveType pt;
    pt.setPrimitiveType(PrimitiveType::Primitive::BOOL);
    Expression expr;
    expr.setID("fa7a57e9-88cf-489c-8345-8351336aec05");
    LiteralBool lb;
    lb.setValue(false);
    expr.operands.push_back(&lb);
    expr.setSymbol("!");
    expr.setType(&pt);
    o.setType(&pt);
    o.setUpperBound(&expr);
    a.nodes.push_back(&o);
    o.setActivity(&a);
    m.ownedElements.push_back(&a);
    a.setOwner(&m);
    ModelParser emitExpressionUpperValue = ModelParser::createNewParser();
    string expectedEmit = R""""(model:
  id: fa8cc066-6191-4903-a766-ee91d216c929
  children:
    - activity:
        id: c7f09553-c13a-4d21-9e00-b9364c0aeaed
        nodes:
          - objectNode:
              id: 9c16720e-a366-4eef-825a-d46b5232a1d5
              type: BOOL
              upperBound:
                expression:
                  type: BOOL
                  id: fa7a57e9-88cf-489c-8345-8351336aec05
                  operand:
                    - literalBool: false
                  symbol: "!")"""";

    // Test
    string generatedEmit;
    YAML::Emitter emitter;
    ASSERT_NO_THROW(emitExpressionUpperValue.emit(emitter, &m));
    generatedEmit = emitter.c_str();
    cout << generatedEmit << '\n';
    ASSERT_TRUE(emitter.good());
    ASSERT_EQ(expectedEmit, generatedEmit);
}