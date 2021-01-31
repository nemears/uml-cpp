#include "gtest/gtest.h"
#include "uml/typedElement.h"
#include "uml/class.h"

using namespace UML;

class TypedElementTest : public ::testing::Test {
    public:
    TypedElement typedEl;
    Class type;
    TypedElement* typedElPtr;

    protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  TypedElementTest() {
     typedElPtr = new TypedElement;
  }

  ~TypedElementTest() override {
     delete typedElPtr;
  }

  void SetUp() override {
     // add element to owned element list
     typedEl.setType(&type);
     typedElPtr->setType(&type);
  }

  void TearDown() override {
    // nothing right now
  }

  // Class members declared here can be used by all tests in the test suite
  // for Element
};

TEST_F(TypedElementTest, GetTypeTest) {
  EXPECT_EQ(typedEl.getType(), &type);
  EXPECT_EQ(typedElPtr->getType(), &type);
}

TEST_F(TypedElementTest, GetTypeReturnsNullTest) {
  TypedElement t;
  ASSERT_TRUE(t.getType() == NULL);
}