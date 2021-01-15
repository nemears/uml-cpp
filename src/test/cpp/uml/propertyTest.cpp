#include "gtest/gtest.h"
#include "../../../cpp/uml/headers/valueSpecification.h"
#include "../../../cpp/uml/headers/property.h"
#include "../../../cpp/uml/headers/primitiveType.h"
#include "../../../cpp/uml/headers/literalString.h"

using namespace UML;

class PropertyTest : public ::testing::Test {
    public:

  PropertyTest() {

  }

  ~PropertyTest() override {

  }

  void SetUp() override {
     
  }

  void TearDown() override {
    // nothing right now
  }
};

TEST_F(PropertyTest, setDefaultValueOfProperTypeTestString) {
    Property p;
    PrimitiveType stringP;
    stringP.setPrimitiveType(PrimitiveType::Primitive::STRING);
    LiteralString ls;
    ls.setValue("test");
    ASSERT_NO_THROW(p.setDefaultValue(&ls));
}

TEST_F(PropertyTest, setDefaultValueOfImproperTypeTestString) {
    Property p;
    PrimitiveType stringP;

    // assign it improper type if expecting string
    stringP.setPrimitiveType(PrimitiveType::Primitive::INT);
    LiteralString ls;
    ls.setValue("test");
    EXPECT_THROW(p.setDefaultValue(&ls), Property::InvalidValueException);
}