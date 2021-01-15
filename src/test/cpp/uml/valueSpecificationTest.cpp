#include "gtest/gtest.h"
#include "../../../cpp/uml/headers/valueSpecification.h"
#include "../../../cpp/uml/headers/property.h"
#include "../../../cpp/uml/headers/primitiveType.h"
#include "../../../cpp/uml/headers/literalString.h"
#include "../../../cpp/uml/headers/literalInt.h"
#include "../../../cpp/uml/headers/literalReal.h"
#include "../../../cpp/uml/headers/literalBool.h"

using namespace UML;

class ValueSpecificationTest : public ::testing::Test {
    public:

  ValueSpecificationTest() {

  }

  ~ValueSpecificationTest() override {

  }

  void SetUp() override {
     
  }

  void TearDown() override {
    // nothing right now
  }
};

TEST_F(ValueSpecificationTest, LiteralStringTest) {
    string val = "test";
    Property p;
    PrimitiveType prim;
    prim.setPrimitiveType(PrimitiveType::Primitive::STRING);
    p.setType(&prim);
    LiteralString ls;
    ASSERT_NO_THROW(ls.setValue(val));
    EXPECT_EQ(ls.getValue(), val);
    EXPECT_EQ(ls.getValue(), "test");
}

TEST_F(ValueSpecificationTest, LiteralIntTest) {
    int val = -1;
    Property p;
    PrimitiveType prim;
    prim.setPrimitiveType(PrimitiveType::Primitive::INT);
    p.setType(&prim);
    LiteralInt ls;
    ASSERT_NO_THROW(ls.setValue(val));
    EXPECT_EQ(ls.getValue(), val);
    EXPECT_EQ(ls.getValue(), -1);
}

TEST_F(ValueSpecificationTest, LiteralRealTest) {
    double val = -3.14159;
    Property p;
    PrimitiveType prim;
    prim.setPrimitiveType(PrimitiveType::Primitive::REAL);
    p.setType(&prim);
    LiteralReal ls;
    ASSERT_NO_THROW(ls.setValue(val));
    EXPECT_EQ(ls.getValue(), val);
    EXPECT_EQ(ls.getValue(), -3.14159);
}

TEST_F(ValueSpecificationTest, LiteralBoolTest) {
    bool val = 0; // > 0 is true
    Property p;
    PrimitiveType prim;
    prim.setPrimitiveType(PrimitiveType::Primitive::BOOL);
    p.setType(&prim);
    LiteralBool ls;
    ASSERT_NO_THROW(ls.setValue(val));
    EXPECT_EQ(ls.getValue(), val);
    EXPECT_EQ(ls.getValue(), false);
}