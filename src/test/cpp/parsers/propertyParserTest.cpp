#include "gtest/gtest.h"
#include "../../../cpp/parsers/headers/modelParser.h"
#include "../../../cpp/uml/headers/typedElement.h"
#include "../../../cpp/uml/headers/primitiveType.h"
#include "../../../cpp/uml/headers/literalString.h"
#include "../../../cpp/uml/headers/literalInt.h"
#include "../../../cpp/uml/headers/literalReal.h"
#include "../../../cpp/uml/headers/literalBool.h"

using namespace UML;

class PropertyParserTest : public ::testing::Test {
    public:
        ModelParser* ppDefaultValue;
        YAML::Node defaultValueNode;
    protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  void SetUp() override {
    ppDefaultValue = new ModelParser(new map<boost::uuids::uuid, Element*>);
    defaultValueNode = YAML::LoadFile("../../../../../src/test/yml/propertyTests/defaultValue.yml");
  }

  void TearDown() override {
    delete ppDefaultValue->elements;
    delete ppDefaultValue;
  }

  // Class members declared here can be used by all tests in the test suite
  // for Element
};

TEST_F(PropertyParserTest, defaultValueTest) {
    EXPECT_NO_THROW(ppDefaultValue->parse(defaultValueNode));

    // test string
    ASSERT_TRUE(((Property*)(*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d")])->getType()->isPrimitive());
    ASSERT_TRUE(((PrimitiveType*)((Property*)(*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d")])->getType())->getPrimitiveType() == PrimitiveType::Primitive::STRING);
    ASSERT_TRUE(((Property*)(*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d")])->getDefaultValue()->getType()->isPrimitive());
    ASSERT_TRUE(((PrimitiveType*)((Property*)(*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d")])->getDefaultValue()->getType())->getPrimitiveType() == PrimitiveType::Primitive::STRING);
    ASSERT_TRUE(((LiteralString*)((Property*)(*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d")])->getDefaultValue())->getValue().compare("testValue") == 0);

    // test int
    ASSERT_TRUE(((Property*)(*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")])->getType()->isPrimitive());
    ASSERT_TRUE(((PrimitiveType*)((Property*)(*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")])->getType())->getPrimitiveType() == PrimitiveType::Primitive::INT);
    ASSERT_TRUE(((Property*)(*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")])->getDefaultValue()->getType()->isPrimitive());
    ASSERT_TRUE(((PrimitiveType*)((Property*)(*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")])->getDefaultValue()->getType())->getPrimitiveType() == PrimitiveType::Primitive::INT);
    ASSERT_TRUE(((LiteralInt*)((Property*)(*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")])->getDefaultValue())->getValue() == -444);

    // test real
    ASSERT_TRUE(((Property*)(*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("c0ab87cc-d00b-4afb-9558-538253b442b2")])->getType()->isPrimitive());
    ASSERT_TRUE(((PrimitiveType*)((Property*)(*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("c0ab87cc-d00b-4afb-9558-538253b442b2")])->getType())->getPrimitiveType() == PrimitiveType::Primitive::REAL);
    ASSERT_TRUE(((Property*)(*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("c0ab87cc-d00b-4afb-9558-538253b442b2")])->getDefaultValue()->getType()->isPrimitive());
    ASSERT_TRUE(((PrimitiveType*)((Property*)(*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("c0ab87cc-d00b-4afb-9558-538253b442b2")])->getDefaultValue()->getType())->getPrimitiveType() == PrimitiveType::Primitive::REAL);
    ASSERT_TRUE(((LiteralReal*)((Property*)(*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("c0ab87cc-d00b-4afb-9558-538253b442b2")])->getDefaultValue())->getValue() == 555.888);

    // test int
    ASSERT_TRUE(((Property*)(*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->getType()->isPrimitive());
    ASSERT_TRUE(((PrimitiveType*)((Property*)(*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->getType())->getPrimitiveType() == PrimitiveType::Primitive::BOOL);
    ASSERT_TRUE(((Property*)(*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->getDefaultValue()->getType()->isPrimitive());
    ASSERT_TRUE(((PrimitiveType*)((Property*)(*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->getDefaultValue()->getType())->getPrimitiveType() == PrimitiveType::Primitive::BOOL);
    ASSERT_TRUE(((LiteralBool*)((Property*)(*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->getDefaultValue())->getValue() == false);
}