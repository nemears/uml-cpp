#include "gtest/gtest.h"
#include "../../../cpp/parsers/headers/modelParser.h"
#include "../../../cpp/uml/headers/instanceSpecification.h"
#include "../../../cpp/uml/headers/primitiveType.h"
#include "../../../cpp/uml/headers/literalString.h"

using namespace UML;

class InstanceSpecificationParserTest : public ::testing::Test {
    public:
        ModelParser* ppInstanceSpecification;
        YAML::Node instanceSpecificationNode;
    protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  void SetUp() override {
    ppInstanceSpecification = new ModelParser(new map<boost::uuids::uuid, Element*>);
    instanceSpecificationNode = YAML::LoadFile("../../../../../src/test/yml/instanceSpecificationTests/primitiveSlot.yml");
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