#include "gtest/gtest.h"
#include "parsers/modelParser.h"
#include "uml/typedElement.h"
#include "uml/primitiveType.h"

class TypedElementParserTest : public ::testing::Test {
    public:
        ModelParser *ppYAML, * ppPrimitive;
        YAML::Node node, primitiveNode, invalidPrimitiveNode;
    protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  TypedElementParserTest() {
    ppYAML = new ModelParser(new map<boost::uuids::uuid, UML::Element*>);
    node = YAML::LoadFile("../../../../../src/test/yml/typedElementTests/typedElement.yml");
  }

  ~TypedElementParserTest() override {
    
  }

  void SetUp() override {
    ppYAML->parse(node);
  }

  void TearDown() override {
    delete ppYAML->elements;
    delete ppYAML;
  }

  // Class members declared here can be used by all tests in the test suite
  // for Element
};

TEST_F(TypedElementParserTest, ParseTypeTest) {
  EXPECT_EQ(((UML::TypedElement*)(*ppYAML->elements)[boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d")])->getType()->uuid, boost::lexical_cast<boost::uuids::uuid>("c0ab87cc-d00b-4afb-9558-538253b442b2"));
  EXPECT_EQ(((UML::TypedElement*)(*ppYAML->elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->getType()->uuid, boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35"));
}

TEST_F(TypedElementParserTest, ParsePrimitiveTest) {

  ppPrimitive = new ModelParser(new map<boost::uuids::uuid, UML::Element*>);

  primitiveNode = YAML::LoadFile("../../../../../src/test/yml/typedElementTests/primitives.yml");
  EXPECT_NO_THROW(ppPrimitive->parse(primitiveNode));
  EXPECT_EQ(((UML::PrimitiveType*)((UML::TypedElement*)(*ppPrimitive->elements)[boost::lexical_cast<boost::uuids::uuid>("c0ab87cc-d00b-4afb-9558-538253b442b2")])->getType())->getPrimitiveType(), UML::PrimitiveType::Primitive::STRING);
  EXPECT_EQ(((UML::PrimitiveType*)((UML::TypedElement*)(*ppPrimitive->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")])->getType())->getPrimitiveType(), UML::PrimitiveType::Primitive::INT);
  EXPECT_EQ(((UML::PrimitiveType*)((UML::TypedElement*)(*ppPrimitive->elements)[boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d")])->getType())->getPrimitiveType(), UML::PrimitiveType::Primitive::REAL);
  EXPECT_EQ(((UML::PrimitiveType*)((UML::TypedElement*)(*ppPrimitive->elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->getType())->getPrimitiveType(), UML::PrimitiveType::Primitive::BOOL);
  delete ppPrimitive->elements;
  delete ppPrimitive;
}

TEST_F(TypedElementParserTest, ParseInvalidPrimitive) {
  invalidPrimitiveNode = YAML::LoadFile("../../../../../src/test/yml/typedElementTests/invalidPrimitive.yml");

  ModelParser invalidPrimitiveParser = ModelParser(new map<boost::uuids::uuid, UML::Element*>);
  ASSERT_NO_THROW(invalidPrimitiveParser.parse(invalidPrimitiveNode));
  UML::TypedElement* val = (UML::TypedElement*)(*invalidPrimitiveParser.elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")];
  EXPECT_TRUE(((UML::TypedElement*)(*invalidPrimitiveParser.elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->getType() == NULL);
}