#include "gtest/gtest.h"
#include "yuml-parsers/modelParser.h"
#include "uml/instanceSpecification.h"
#include "uml/primitiveType.h"
#include "uml/literalString.h"
#include "uml/instanceValue.h"
#include "uml/class.h"

using namespace UML;

class InstanceSpecificationParserTest : public ::testing::Test {
    public:
        ModelParser* ppInstanceSpecification, * ppInstanceValue;
        YAML::Node instanceSpecificationNode, instanceValueNode;
    protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  void SetUp() override {
    ppInstanceSpecification = new ModelParser(new map<boost::uuids::uuid, Element*>);
    instanceSpecificationNode = YAML::LoadFile("../../../../../src/test/yml/instanceSpecificationTests/primitiveSlot.yml");

    ppInstanceValue = new ModelParser(new map<boost::uuids::uuid, Element*>);
    instanceValueNode = YAML::LoadFile("../../../../../src/test/yml/instanceSpecificationTests/instanceSlot.yml");
  }

  void TearDown() override {
    delete ppInstanceSpecification->elements;
    delete ppInstanceSpecification;
  }

  // Class members declared here can be used by all tests in the test suite
  // for Element
};

TEST_F(InstanceSpecificationParserTest, PrimitiveSlotTest) {
    ASSERT_NO_THROW(ppInstanceSpecification->parse(instanceSpecificationNode));
    ASSERT_TRUE(((InstanceSpecification*)(*ppInstanceSpecification->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")])->getClassifier() != NULL);
    ASSERT_TRUE(((InstanceSpecification*)(*ppInstanceSpecification->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")])->getClassifier()->uuid == boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033"));
    ASSERT_TRUE(((InstanceSpecification*)(*ppInstanceSpecification->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")])->slots.empty() == false);
    ASSERT_TRUE(((InstanceSpecification*)(*ppInstanceSpecification->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")])->slots.front()->uuid == boost::lexical_cast<boost::uuids::uuid>("c0ab87cc-d00b-4afb-9558-538253b442b2"));
    ASSERT_TRUE(((InstanceSpecification*)(*ppInstanceSpecification->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")])->slots.front()->getDefiningFeature()->uuid == boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d"));
    ASSERT_TRUE(((InstanceSpecification*)(*ppInstanceSpecification->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")])->slots.front()->getDefiningFeature()->getType() != NULL);
    ASSERT_TRUE(((InstanceSpecification*)(*ppInstanceSpecification->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")])->slots.front()->getDefiningFeature()->getType()->isPrimitive());
    ASSERT_TRUE(((PrimitiveType*)((InstanceSpecification*)(*ppInstanceSpecification->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")])->slots.front()->getDefiningFeature()->getType())->getPrimitiveType() == PrimitiveType::Primitive::STRING);
    ASSERT_TRUE(((InstanceSpecification*)(*ppInstanceSpecification->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")])->slots.front()->values.empty() == false);
    ASSERT_TRUE(((LiteralString*)((InstanceSpecification*)(*ppInstanceSpecification->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")])->slots.front()->values.front())->getValue().compare("test") == 0);
    
}

TEST_F(InstanceSpecificationParserTest, InstanceSlotTest) {
  ASSERT_NO_THROW(ppInstanceValue->parse(instanceValueNode));
  ASSERT_TRUE(((InstanceSpecification*)(*ppInstanceValue->elements)[boost::lexical_cast<boost::uuids::uuid>("c0ab87cc-d00b-4afb-9558-538253b442b2")])->getClassifier() != NULL);
  ASSERT_TRUE(((InstanceSpecification*)(*ppInstanceValue->elements)[boost::lexical_cast<boost::uuids::uuid>("c0ab87cc-d00b-4afb-9558-538253b442b2")])->getClassifier()->uuid == boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033"));
  ASSERT_TRUE(((InstanceSpecification*)(*ppInstanceValue->elements)[boost::lexical_cast<boost::uuids::uuid>("c0ab87cc-d00b-4afb-9558-538253b442b2")])->slots.empty());
  ASSERT_TRUE(((InstanceSpecification*)(*ppInstanceValue->elements)[boost::lexical_cast<boost::uuids::uuid>("563f4740-e107-4d08-8618-2489f0fe1865")])->getClassifier() != NULL);
  ASSERT_TRUE(((InstanceSpecification*)(*ppInstanceValue->elements)[boost::lexical_cast<boost::uuids::uuid>("563f4740-e107-4d08-8618-2489f0fe1865")])->getClassifier()->uuid == boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d"));
  ASSERT_TRUE(((InstanceSpecification*)(*ppInstanceValue->elements)[boost::lexical_cast<boost::uuids::uuid>("563f4740-e107-4d08-8618-2489f0fe1865")])->slots.empty() == false);
  ASSERT_TRUE(((InstanceSpecification*)(*ppInstanceValue->elements)[boost::lexical_cast<boost::uuids::uuid>("563f4740-e107-4d08-8618-2489f0fe1865")])->slots.front()->uuid == boost::lexical_cast<boost::uuids::uuid>("d9ab2f06-4c2c-4330-9e1b-7eaee423a66a"));
  ASSERT_TRUE(((InstanceSpecification*)(*ppInstanceValue->elements)[boost::lexical_cast<boost::uuids::uuid>("563f4740-e107-4d08-8618-2489f0fe1865")])->slots.front()->getDefiningFeature()->uuid == boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35"));
  ASSERT_TRUE(((InstanceSpecification*)(*ppInstanceValue->elements)[boost::lexical_cast<boost::uuids::uuid>("563f4740-e107-4d08-8618-2489f0fe1865")])->slots.front()->getDefiningFeature()->getType() != NULL);
  ASSERT_TRUE(((InstanceSpecification*)(*ppInstanceValue->elements)[boost::lexical_cast<boost::uuids::uuid>("563f4740-e107-4d08-8618-2489f0fe1865")])->slots.front()->getDefiningFeature()->getType()->uuid == boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033"));
  ASSERT_TRUE(((InstanceValue*)((InstanceSpecification*)(*ppInstanceValue->elements)[boost::lexical_cast<boost::uuids::uuid>("563f4740-e107-4d08-8618-2489f0fe1865")])->slots.front()->values.front())->getInstance()->uuid == boost::lexical_cast<boost::uuids::uuid>("c0ab87cc-d00b-4afb-9558-538253b442b2"));
    
}

TEST_F(InstanceSpecificationParserTest, EmitInstanceWithClassifierTest) {
  // Setup
  Model m;
  m.setID("190d1cb9-13dc-44e6-a064-126891ae0033");
  Class c;
  c.setID("16c345b4-5ae2-41ca-a0e7-a9c386ac941d");
  InstanceSpecification i;
  i.setID("7d18ee42-82c6-4f52-8ec4-fab67a75ff35");
  i.setClassifier(&c);
  m.ownedElements.push_back(&c);
  m.ownedElements.push_back(&i);

  ModelParser emitInstanceWithClassTestParser(new map<boost::uuids::uuid, Element*>);
  string expectedEmit = R""""(model:
  id: 190d1cb9-13dc-44e6-a064-126891ae0033
  children:
    - class:
        id: 16c345b4-5ae2-41ca-a0e7-a9c386ac941d
    - instanceSpecification:
        id: 7d18ee42-82c6-4f52-8ec4-fab67a75ff35
        classifier: 16c345b4-5ae2-41ca-a0e7-a9c386ac941d)"""";

  string generatedEmit;
  YAML::Emitter emitter;
  ASSERT_NO_THROW(emitInstanceWithClassTestParser.emit(emitter, &m));
  generatedEmit = emitter.c_str();
  cout << generatedEmit << '\n';
  ASSERT_TRUE(emitter.good());
  ASSERT_EQ(expectedEmit, generatedEmit);
}