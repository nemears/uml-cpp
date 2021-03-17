#include "gtest/gtest.h"
#include "uml/valueSpecification.h"
#include "uml/property.h"
#include "uml/primitiveType.h"
#include "uml/literalString.h"
#include "uml/class.h"

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
    p.setType(&stringP);
    LiteralString ls;
    ls.setValue("test");
    ASSERT_NO_THROW(p.setDefaultValue(&ls));
}

TEST_F(PropertyTest, setDefaultValueOfImproperTypeTestString) {
    Property p;
    PrimitiveType stringP;

    // assign it improper type if expecting string
    stringP.setPrimitiveType(PrimitiveType::Primitive::INT);

    p.setType(&stringP);
    
    LiteralString ls;
    ls.setValue("test");
    EXPECT_THROW(p.setDefaultValue(&ls), Property::InvalidValueException);
}

TEST_F(PropertyTest, reindexID_forClassiferTest) {
  Class c;
  Property p;
  c.getAttributes().add(p);
  p.setClassifier(&c);
  p.setID("190d1cb9-13dc-44e6-a064-126891ae0033");
  ASSERT_TRUE(c.getAttributes().get(p.getID()) != NULL);
}

TEST_F(PropertyTest, reindexNameForClassifierTest) {
  Class c;
  Property p;
  c.getAttributes().add(p);
  p.setClassifier(&c);
  p.setName("test");
  ASSERT_TRUE(c.getAttributes().get("test") != NULL);
}