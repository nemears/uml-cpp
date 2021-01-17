#include "gtest/gtest.h"
#include "../../../cpp/parsers/headers/modelParser.h"
#include "../../../cpp/uml/headers/typedElement.h"
#include "../../../cpp/uml/headers/primitiveType.h"
#include "../../../cpp/uml/headers/literalString.h"
#include "../../../cpp/uml/headers/literalInt.h"
#include "../../../cpp/uml/headers/literalReal.h"
#include "../../../cpp/uml/headers/literalBool.h"

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
    instanceSpecificationNode = YAML::LoadFile("../../../../../src/test/yml/instanceSpecificationTests/instanceSpecification.yml");
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
}