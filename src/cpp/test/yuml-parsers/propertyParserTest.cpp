#include "gtest/gtest.h"
#include "test/yumlParsersTest.h"
#include "yuml-parsers/modelParser.h"
#include "uml/typedElement.h"
#include "uml/primitiveType.h"
#include "uml/literalString.h"
#include "uml/literalInt.h"
#include "uml/literalReal.h"
#include "uml/literalBool.h"
#include "uml/instanceValue.h"
#include "uml/class.h"

using namespace UML;

class PropertyParserTest : public ::testing::Test {
    public:
        ModelParser* ppDefaultValue;
        YAML::Node defaultValueNode;
        string ymlPath;
    protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  void SetUp() override {
    ymlPath = YML_FILES_PATH;
    ppDefaultValue = new ModelParser(new map<boost::uuids::uuid, Element*>, new map<boost::uuids::uuid, PostParser*>);
    defaultValueNode = YAML::LoadFile(ymlPath + "propertyTests/defaultValue.yml");
  }

  void TearDown() override {
    delete ppDefaultValue->elements;
    delete ppDefaultValue->postProcessFlag;
    delete ppDefaultValue;
  }

  // Class members declared here can be used by all tests in the test suite
  // for Element
};

TEST_F(PropertyParserTest, defaultLiteralValueTest) {
    EXPECT_NO_THROW(ppDefaultValue->parse(defaultValueNode));

    // test string
    ASSERT_TRUE(dynamic_cast<Property*>((*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d")])->getType()->isPrimitive());
    ASSERT_TRUE(((PrimitiveType*)dynamic_cast<Property*>((*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d")])->getType())->getPrimitiveType() == PrimitiveType::Primitive::STRING);
    ASSERT_TRUE(dynamic_cast<Property*>((*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d")])->getDefaultValue()->getType()->isPrimitive());
    ASSERT_TRUE(((PrimitiveType*)dynamic_cast<Property*>((*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d")])->getDefaultValue()->getType())->getPrimitiveType() == PrimitiveType::Primitive::STRING);
    ASSERT_TRUE(((LiteralString*)dynamic_cast<Property*>((*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d")])->getDefaultValue())->getValue().compare("testValue") == 0);

    // test int
    ASSERT_TRUE(dynamic_cast<Property*>((*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")])->getType()->isPrimitive());
    ASSERT_TRUE(((PrimitiveType*)dynamic_cast<Property*>((*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")])->getType())->getPrimitiveType() == PrimitiveType::Primitive::INT);
    ASSERT_TRUE(dynamic_cast<Property*>((*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")])->getDefaultValue()->getType()->isPrimitive());
    ASSERT_TRUE(((PrimitiveType*)dynamic_cast<Property*>((*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")])->getDefaultValue()->getType())->getPrimitiveType() == PrimitiveType::Primitive::INT);
    ASSERT_TRUE(((LiteralInt*)dynamic_cast<Property*>((*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")])->getDefaultValue())->getValue() == -444);

    // test real
    ASSERT_TRUE(dynamic_cast<Property*>((*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("c0ab87cc-d00b-4afb-9558-538253b442b2")])->getType()->isPrimitive());
    ASSERT_TRUE(((PrimitiveType*)dynamic_cast<Property*>((*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("c0ab87cc-d00b-4afb-9558-538253b442b2")])->getType())->getPrimitiveType() == PrimitiveType::Primitive::REAL);
    ASSERT_TRUE(dynamic_cast<Property*>((*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("c0ab87cc-d00b-4afb-9558-538253b442b2")])->getDefaultValue()->getType()->isPrimitive());
    ASSERT_TRUE(((PrimitiveType*)dynamic_cast<Property*>((*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("c0ab87cc-d00b-4afb-9558-538253b442b2")])->getDefaultValue()->getType())->getPrimitiveType() == PrimitiveType::Primitive::REAL);
    ASSERT_TRUE(((LiteralReal*)dynamic_cast<Property*>((*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("c0ab87cc-d00b-4afb-9558-538253b442b2")])->getDefaultValue())->getValue() == 555.888);

    // test bool
    ASSERT_TRUE(dynamic_cast<Property*>((*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->getType()->isPrimitive());
    ASSERT_TRUE(((PrimitiveType*)dynamic_cast<Property*>((*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->getType())->getPrimitiveType() == PrimitiveType::Primitive::BOOL);
    ASSERT_TRUE(dynamic_cast<Property*>((*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->getDefaultValue()->getType()->isPrimitive());
    ASSERT_TRUE(((PrimitiveType*)dynamic_cast<Property*>((*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->getDefaultValue()->getType())->getPrimitiveType() == PrimitiveType::Primitive::BOOL);
    ASSERT_TRUE(((LiteralBool*)dynamic_cast<Property*>((*ppDefaultValue->elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->getDefaultValue())->getValue() == false);
}

TEST_F(PropertyParserTest, InstanceValueDefaultValueTest) {
  // Setup
  ModelParser instanceValueTestParser = ModelParser::createNewParser();
  YAML::Node instanceValueTestNode = YAML::LoadFile(ymlPath + "propertyTests/defaultInstanceValue.yml");
  ASSERT_NO_THROW(instanceValueTestParser.parse(instanceValueTestNode));

  ASSERT_TRUE((*instanceValueTestParser.elements)[boost::lexical_cast<boost::uuids::uuid>("c0ab87cc-d00b-4afb-9558-538253b442b2")] != NULL);
  ASSERT_TRUE((*instanceValueTestParser.elements)[boost::lexical_cast<boost::uuids::uuid>("c0ab87cc-d00b-4afb-9558-538253b442b2")]->getElementType() == ElementType::PROPERTY);
  ASSERT_TRUE(dynamic_cast<Property*>((*instanceValueTestParser.elements)[boost::lexical_cast<boost::uuids::uuid>("c0ab87cc-d00b-4afb-9558-538253b442b2")])->getType() != NULL);
  ASSERT_TRUE(dynamic_cast<Property*>((*instanceValueTestParser.elements)[boost::lexical_cast<boost::uuids::uuid>("c0ab87cc-d00b-4afb-9558-538253b442b2")])->getType()->getID() == boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d"));
  ASSERT_TRUE((*instanceValueTestParser.elements)[dynamic_cast<Property*>((*instanceValueTestParser.elements)[boost::lexical_cast<boost::uuids::uuid>("c0ab87cc-d00b-4afb-9558-538253b442b2")])->getType()->getID()]->getElementType() == ElementType::CLASS);
  ASSERT_TRUE(dynamic_cast<Property*>((*instanceValueTestParser.elements)[boost::lexical_cast<boost::uuids::uuid>("c0ab87cc-d00b-4afb-9558-538253b442b2")])->getDefaultValue() != NULL);
  ASSERT_TRUE(dynamic_cast<Property*>((*instanceValueTestParser.elements)[boost::lexical_cast<boost::uuids::uuid>("c0ab87cc-d00b-4afb-9558-538253b442b2")])->getDefaultValue()->getElementType() == ElementType::INSTANCE_VALUE);
  ASSERT_TRUE(((InstanceValue*)dynamic_cast<Property*>((*instanceValueTestParser.elements)[boost::lexical_cast<boost::uuids::uuid>("c0ab87cc-d00b-4afb-9558-538253b442b2")])->getDefaultValue())->getInstance()!= NULL);
  ASSERT_TRUE(((InstanceValue*)dynamic_cast<Property*>((*instanceValueTestParser.elements)[boost::lexical_cast<boost::uuids::uuid>("c0ab87cc-d00b-4afb-9558-538253b442b2")])->getDefaultValue())->getInstance()->getID() == boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033"));
  ASSERT_TRUE((*instanceValueTestParser.elements)[((InstanceValue*)dynamic_cast<Property*>((*instanceValueTestParser.elements)[boost::lexical_cast<boost::uuids::uuid>("c0ab87cc-d00b-4afb-9558-538253b442b2")])->getDefaultValue())->getInstance()->getID()]->getElementType() == ElementType::INSTANCE_SPECIFICATION);
  ASSERT_TRUE(((InstanceSpecification*)((InstanceValue*)dynamic_cast<Property*>((*instanceValueTestParser.elements)[boost::lexical_cast<boost::uuids::uuid>("c0ab87cc-d00b-4afb-9558-538253b442b2")])->getDefaultValue())->getInstance())->getClassifier()->getID() == boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d"));
  ASSERT_TRUE(((InstanceSpecification*)((InstanceValue*)dynamic_cast<Property*>((*instanceValueTestParser.elements)[boost::lexical_cast<boost::uuids::uuid>("c0ab87cc-d00b-4afb-9558-538253b442b2")])->getDefaultValue())->getInstance())->getClassifier()->getID() == dynamic_cast<Property*>((*instanceValueTestParser.elements)[boost::lexical_cast<boost::uuids::uuid>("c0ab87cc-d00b-4afb-9558-538253b442b2")])->getType()->getID());
}

TEST_F(PropertyParserTest, EmitLiteralDefaultValue) {
  // Setup
  Model m;
  m.setID("16c345b4-5ae2-41ca-a0e7-a9c386ac941d");
  Class c;
  c.setID("190d1cb9-13dc-44e6-a064-126891ae0033");
  Property p;
  p.setID("7d18ee42-82c6-4f52-8ec4-fab67a75ff35");
  PrimitiveType pt;
  pt.setPrimitiveType(PrimitiveType::Primitive::STRING);
  LiteralString ls;
  ls.setValue("test");
  p.setType(&pt);
  p.setDefaultValue(&ls);
  c.ownedAttributes.push_back(&p);
  m.getOwnedElements().add(c);

  ModelParser emitLiteralDefaultValueParser = ModelParser::createNewParser();
  string expectedEmit = R""""(model:
  id: 16c345b4-5ae2-41ca-a0e7-a9c386ac941d
  children:
    - class:
        id: 190d1cb9-13dc-44e6-a064-126891ae0033
        attributes:
          - property:
              type: STRING
              id: 7d18ee42-82c6-4f52-8ec4-fab67a75ff35
              defaultValue: test)"""";

  string generatedEmit;
  YAML::Emitter emitter;
  ASSERT_NO_THROW(emitLiteralDefaultValueParser.emit(emitter, &m));
  generatedEmit = emitter.c_str();
  cout << generatedEmit << '\n';
  ASSERT_TRUE(emitter.good());
  ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(PropertyParserTest, EmitInstanceValueTest) {
  // Setup
  Model m;
  m.setID("16c345b4-5ae2-41ca-a0e7-a9c386ac941d");
  Class c;
  c.setID("190d1cb9-13dc-44e6-a064-126891ae0033");
  Class c2;
  c2.setID("c0ab87cc-d00b-4afb-9558-538253b442b2");
  Property p;
  p.setID("7d18ee42-82c6-4f52-8ec4-fab67a75ff35");
  InstanceSpecification i;
  i.setID("563f4740-e107-4d08-8618-2489f0fe1865");
  i.setClassifier(&c2);
  p.setType(&c2);
  InstanceValue iv;
  iv.setInstance(&i);
  p.setDefaultValue(&iv);
  c.ownedAttributes.push_back(&p);
  m.getOwnedElements().add(c2);
  m.getOwnedElements().add(i);
  m.getOwnedElements().add(c);

  ModelParser emitInstanceDefaultValueParser = ModelParser::createNewParser();
  string expectedEmit = R""""(model:
  id: 16c345b4-5ae2-41ca-a0e7-a9c386ac941d
  children:
    - class:
        id: c0ab87cc-d00b-4afb-9558-538253b442b2
    - instanceSpecification:
        id: 563f4740-e107-4d08-8618-2489f0fe1865
        classifier: c0ab87cc-d00b-4afb-9558-538253b442b2
    - class:
        id: 190d1cb9-13dc-44e6-a064-126891ae0033
        attributes:
          - property:
              type: c0ab87cc-d00b-4afb-9558-538253b442b2
              id: 7d18ee42-82c6-4f52-8ec4-fab67a75ff35
              defaultValue: 563f4740-e107-4d08-8618-2489f0fe1865)"""";

  string generatedEmit;
  YAML::Emitter emitter;
  ASSERT_NO_THROW(emitInstanceDefaultValueParser.emit(emitter, &m));
  generatedEmit = emitter.c_str();
  cout << generatedEmit << '\n';
  ASSERT_TRUE(emitter.good());
  ASSERT_EQ(expectedEmit, generatedEmit);
}