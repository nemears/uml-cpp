#include "gtest/gtest.h"
#include "test/yumlParsersTest.h"
#include "yuml-parsers/modelParser.h"
#include "uml/typedElement.h"
#include "uml/primitiveType.h"
#include "uml/class.h"

using namespace UML;

class TypedElementParserTest : public ::testing::Test {
    public:
        ModelParser *ppYAML, * ppPrimitive;
        YAML::Node node, primitiveNode, invalidPrimitiveNode;
        string ymlPath;
    protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  TypedElementParserTest() {
    ymlPath = YML_FILES_PATH;
    ppYAML = new ModelParser(new map<boost::uuids::uuid, Element*>, new map<boost::uuids::uuid, PostParser*>);
    node = YAML::LoadFile(ymlPath + "typedElementTests/typedElement.yml");
  }

  ~TypedElementParserTest() override {
    
  }

  void SetUp() override {
    ppYAML->parse(node);
  }

  void TearDown() override {
    delete ppYAML->elements;
    delete ppYAML->postProcessFlag;
    delete ppYAML;
  }

  // Class members declared here can be used by all tests in the test suite
  // for Element
};

TEST_F(TypedElementParserTest, ParseTypeTest) {
  EXPECT_EQ(dynamic_cast<TypedElement*>((*ppYAML->elements)[boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d")])->getType()->getID(), boost::lexical_cast<boost::uuids::uuid>("c0ab87cc-d00b-4afb-9558-538253b442b2"));
  EXPECT_EQ(dynamic_cast<TypedElement*>((*ppYAML->elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->getType()->getID(), boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35"));
}

TEST_F(TypedElementParserTest, ParsePrimitiveTest) {

  ppPrimitive = new ModelParser(new map<boost::uuids::uuid, UML::Element*>, new map<boost::uuids::uuid, PostParser*>);

  primitiveNode = YAML::LoadFile(ymlPath + "typedElementTests/primitives.yml");
  EXPECT_NO_THROW(ppPrimitive->parse(primitiveNode));
  EXPECT_EQ(((PrimitiveType*)dynamic_cast<TypedElement*>((*ppPrimitive->elements)[boost::lexical_cast<boost::uuids::uuid>("c0ab87cc-d00b-4afb-9558-538253b442b2")])->getType())->getPrimitiveType(), UML::PrimitiveType::Primitive::STRING);
  EXPECT_EQ(((PrimitiveType*)dynamic_cast<TypedElement*>((*ppPrimitive->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")])->getType())->getPrimitiveType(), UML::PrimitiveType::Primitive::INT);
  EXPECT_EQ(((PrimitiveType*)dynamic_cast<TypedElement*>((*ppPrimitive->elements)[boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d")])->getType())->getPrimitiveType(), UML::PrimitiveType::Primitive::REAL);
  EXPECT_EQ(((PrimitiveType*)dynamic_cast<TypedElement*>((*ppPrimitive->elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->getType())->getPrimitiveType(), UML::PrimitiveType::Primitive::BOOL);
  delete ppPrimitive->elements;
  delete ppPrimitive;
}

TEST_F(TypedElementParserTest, ParseInvalidPrimitive) {
  invalidPrimitiveNode = YAML::LoadFile(ymlPath + "typedElementTests/invalidPrimitive.yml");

  ModelParser invalidPrimitiveParser = ModelParser::createNewParser();
  ASSERT_THROW(invalidPrimitiveParser.parse(invalidPrimitiveNode), ElementParser::InvalidIdentifierException);
}

TEST_F(TypedElementParserTest, EmitLiteralTypeTest) {
  Model m;
  m.setID("c0ab87cc-d00b-4afb-9558-538253b442b2");
  Class c;
  c.setID("7d18ee42-82c6-4f52-8ec4-fab67a75ff35");
  Property p;
  p.setID("16c345b4-5ae2-41ca-a0e7-a9c386ac941d");
  PrimitiveType pt;
  pt.setPrimitiveType(PrimitiveType::Primitive::STRING);
  p.setType(&pt);
  c.ownedAttributes.push_back(&p);
  m.ownedElements.push_back(&c);

  string expectedEmit = R""""(model:
  id: c0ab87cc-d00b-4afb-9558-538253b442b2
  children:
    - class:
        id: 7d18ee42-82c6-4f52-8ec4-fab67a75ff35
        attributes:
          - property:
              type: STRING
              id: 16c345b4-5ae2-41ca-a0e7-a9c386ac941d)"""";

  ModelParser mp = ModelParser::createNewParser();
  string generatedEmit;
  YAML::Emitter emitter;
  ASSERT_NO_THROW(mp.emit(emitter, &m));
  generatedEmit = emitter.c_str();
  cout << generatedEmit << '\n';
  ASSERT_TRUE(emitter.good());
  ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(TypedElementParserTest, EmitAttributesOfClassiferTypeTest) {
  Model m;
  m.setID("c0ab87cc-d00b-4afb-9558-538253b442b2");
  Class c;
  c.setID("7d18ee42-82c6-4f52-8ec4-fab67a75ff35");
  Property p;
  p.setID("16c345b4-5ae2-41ca-a0e7-a9c386ac941d");
  Class c2;
  c2.setID("190d1cb9-13dc-44e6-a064-126891ae0033");
  p.setType(&c2);
  c.ownedAttributes.push_back(&p);
  m.ownedElements.push_back(&c);
  m.ownedElements.push_back(&c2);

  string expectedEmit = R""""(model:
  id: c0ab87cc-d00b-4afb-9558-538253b442b2
  children:
    - class:
        id: 7d18ee42-82c6-4f52-8ec4-fab67a75ff35
        attributes:
          - property:
              type: 190d1cb9-13dc-44e6-a064-126891ae0033
              id: 16c345b4-5ae2-41ca-a0e7-a9c386ac941d
    - class:
        id: 190d1cb9-13dc-44e6-a064-126891ae0033)"""";

  ModelParser mp = ModelParser::createNewParser();
  string generatedEmit;
  YAML::Emitter emitter;
  ASSERT_NO_THROW(mp.emit(emitter, &m));
  generatedEmit = emitter.c_str();
  cout << generatedEmit << '\n';
  ASSERT_TRUE(emitter.good());
  ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(TypedElementParserTest, BackwardsParsingTest) {
  // Setup
  ModelParser backwardsParsingParser = ModelParser::createNewParser();
  YAML::Node backwardsParsingNode = YAML::LoadFile(ymlPath + "typedElementTests/backwardsParsing.yml");

  // Test
  ASSERT_NO_THROW(backwardsParsingParser.parse(backwardsParsingNode));
  ASSERT_TRUE(backwardsParsingParser.theEl->ownedElements.size() == 2);
  ASSERT_TRUE(backwardsParsingParser.theEl->ownedElements.front()->getID() == boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033"));
  ASSERT_TRUE(backwardsParsingParser.theEl->ownedElements.front()->getElementType() == ElementType::CLASS);
  ASSERT_TRUE(dynamic_cast<Class*>(backwardsParsingParser.theEl->ownedElements.front())->ownedAttributes.size() == 1);
  ASSERT_TRUE(dynamic_cast<Class*>(backwardsParsingParser.theEl->ownedElements.front())->ownedAttributes.front()->getType() != NULL);
  ASSERT_TRUE(dynamic_cast<Class*>(backwardsParsingParser.theEl->ownedElements.front())->ownedAttributes.front()->getType()->getID() == backwardsParsingParser.theEl->ownedElements.back()->getID());
}