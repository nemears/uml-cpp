#include "gtest/gtest.h"
#include "test/yumlParsersTest.h"
#include "yuml-parsers/modelParser.h"
#include "uml/activity.h"
#include "uml/objectNode.h"
#include "uml/primitiveType.h"
#include "uml/literalInt.h"
#include "uml/expression.h"
#include "uml/literalBool.h"

using namespace UML;

class ObjectNodeParserTest : public ::testing::Test {
  public:
        string ymlPath;
        void SetUp() override {
            ymlPath = YML_FILES_PATH;
        };
};

TEST_F(ObjectNodeParserTest, ParseLiteralUpperBoundTest) {
    // Setup
    ModelParser literalUpperBoundParser = ModelParser::createNewParser();
    YAML::Node literalUpperBoundNode = YAML::LoadFile(ymlPath + "objectNodeTests/literalUpperBound.yml");

    // Test
    ASSERT_TRUE(literalUpperBoundParser.parse(literalUpperBoundNode));
    ASSERT_TRUE(literalUpperBoundParser.theEl->getOwnedElements().size() == 1);
    ASSERT_TRUE(literalUpperBoundParser.theEl->getOwnedElements().front()->getElementType() == ElementType::ACTIVITY);

    // Activity
    Activity* activity = dynamic_cast<Activity*>(literalUpperBoundParser.theEl->getOwnedElements().front());
    ASSERT_TRUE(activity->getNodes().size() == 1);
    ASSERT_TRUE(activity->getEdges().size() == 0);
    ASSERT_TRUE(activity->getNodes().front()->getElementType() == ElementType::OBJECT_NODE);

    // Object Node
    ObjectNode* objNode = dynamic_cast<ObjectNode*>(activity->getNodes().front());
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
    YAML::Node expressionUpperBoundNode = YAML::LoadFile(ymlPath + "objectNodeTests/expressionUpperBound.yml");

    // Test
    ASSERT_NO_THROW(expressionUpperBoundParser.parse(expressionUpperBoundNode));
    ASSERT_TRUE(expressionUpperBoundParser.theEl->getOwnedElements().size() == 1);
    ASSERT_TRUE(expressionUpperBoundParser.theEl->getOwnedElements().front()->getElementType() == ElementType::ACTIVITY);

    // Activity
    Activity* activity = dynamic_cast<Activity*>(expressionUpperBoundParser.theEl->getOwnedElements().front());
    ASSERT_TRUE(activity->getNodes().size() == 1);
    ASSERT_TRUE(activity->getEdges().size() == 0);
    ASSERT_TRUE(activity->getNodes().front()->getElementType() == ElementType::OBJECT_NODE);

    // Object Node
    ObjectNode* objNode = dynamic_cast<ObjectNode*>(activity->getNodes().front());
    ASSERT_TRUE(objNode->getType() != NULL);
    ASSERT_TRUE(objNode->getType()->isPrimitive());
    ASSERT_TRUE(dynamic_cast<PrimitiveType*>(objNode->getType())->getPrimitiveType() == PrimitiveType::Primitive::INT);
    ASSERT_TRUE(objNode->getUpperBound() != NULL);
    ASSERT_TRUE(objNode->getUpperBound()->getElementType() == ElementType::EXPRESSION);
    ASSERT_TRUE(dynamic_cast<Expression*>(objNode->getUpperBound())->getOperands().size() == 1);
    ASSERT_TRUE(dynamic_cast<Expression*>(objNode->getUpperBound())->getOperands().front()->getElementType() == ElementType::LITERAL_INT);
    ASSERT_TRUE(dynamic_cast<LiteralInt*>(dynamic_cast<Expression*>(objNode->getUpperBound())->getOperands().front())->getValue() == 5);
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
    a.getNodes().add(o);
    o.setActivity(&a);
    m.getOwnedElements().add(a);
    a.setOwner(&m);
    ModelParser emitLiteralUpperValue = ModelParser::createNewParser();
    string expectedEmit = R""""(model:
  id: fa8cc066-6191-4903-a766-ee91d216c929
  children:
    - activity:
        id: c7f09553-c13a-4d21-9e00-b9364c0aeaed
        children:
          - 9c16720e-a366-4eef-825a-d46b5232a1d5
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
    lb.setID("6799346f-9a33-4150-ba6e-f466ad414a8e");
    lb.setValue(false);
    expr.getOperands().add(lb);
    expr.setSymbol("!");
    expr.setType(&pt);
    o.setType(&pt);
    o.setUpperBound(&expr);
    a.getNodes().add(o);
    o.setActivity(&a);
    m.getOwnedElements().add(a);
    a.setOwner(&m);
    ModelParser emitExpressionUpperValue = ModelParser::createNewParser();
    string expectedEmit = R""""(model:
  id: fa8cc066-6191-4903-a766-ee91d216c929
  children:
    - activity:
        id: c7f09553-c13a-4d21-9e00-b9364c0aeaed
        children:
          - 9c16720e-a366-4eef-825a-d46b5232a1d5
        nodes:
          - objectNode:
              id: 9c16720e-a366-4eef-825a-d46b5232a1d5
              type: BOOL
              upperBound:
                expression:
                  type: BOOL
                  id: fa7a57e9-88cf-489c-8345-8351336aec05
                  children:
                    - 6799346f-9a33-4150-ba6e-f466ad414a8e
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