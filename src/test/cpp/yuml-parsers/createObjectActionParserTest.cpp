#include "gtest/gtest.h"
#include "yuml-parsers/modelParser.h"
#include "uml/activity.h"
#include "uml/createObjectAction.h"
#include "uml/primitiveType.h"

using namespace UML;

class CreateObjectParserTest : public ::testing::Test {

};

TEST_F(CreateObjectParserTest, ParseLiteralClassifierTest) {
    // Setup
    ModelParser literalClassifierParser = ModelParser::createNewParser();
    YAML::Node literalClassifierNode = YAML::LoadFile("../../../../../src/test/yml/createObjectActionTests/primitiveClassifier.yml");

    // Test
    ASSERT_NO_THROW(literalClassifierParser.parse(literalClassifierNode));
    ASSERT_TRUE(literalClassifierParser.theEl->ownedElements.size() == 1);
    ASSERT_TRUE(literalClassifierParser.theEl->ownedElements.front()->getElementType() == ElementType::ACTIVITY);
    
    // Activity
    Activity* act = dynamic_cast<Activity*>(literalClassifierParser.theEl->ownedElements.front());
    ASSERT_TRUE(act->nodes.size() == 2);
    ASSERT_TRUE(act->nodes.front()->getElementType() == ElementType::CREATE_OBJECT_ACTION);

    // CreateObjectAction
    CreateObjectAction* coa = dynamic_cast<CreateObjectAction*>(act->nodes.front());
    ASSERT_TRUE(coa->getClassifier() != NULL);
    ASSERT_TRUE(coa->getClassifier()->isPrimitive());
    ASSERT_TRUE(dynamic_cast<PrimitiveType*>(coa->getClassifier())->getPrimitiveType() == PrimitiveType::Primitive::BOOL);
    ASSERT_TRUE(coa->outputs.size() == 1);
    ASSERT_TRUE(coa->outputs.front()->getType()->isPrimitive());
    ASSERT_TRUE(dynamic_cast<PrimitiveType*>(coa->outputs.front()->getType())->getPrimitiveType() == PrimitiveType::Primitive::BOOL);

    // Tear down
    ModelParser::deleteParser(&literalClassifierParser);
}