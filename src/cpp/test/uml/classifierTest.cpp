#include "gtest/gtest.h"
#include "uml/classifier.h"

using namespace UML;

class ClassifierTest : public ::testing::Test {
    public:
    Classifier classifier1;
    Property prop;
    Classifier* classifierPtr;
    Property prop2;
    Property* propPtr;

    protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  ClassifierTest() {
     classifierPtr = new Classifier;
     propPtr = new Property;
  }

  ~ClassifierTest() override {
     delete classifierPtr;
  }

  void SetUp() override {
     // add element to owned element list
     classifier1.getAttributes().add(prop);
     classifier1.getAttributes().add(*propPtr);
     classifierPtr->getAttributes().add(prop2);
  }

  void TearDown() override {
    // nothing right now
  }

  // Class members declared here can be used by all tests in the test suite
  // for Element
};

TEST_F(ClassifierTest, GetOwnedAttributesTest) {
  EXPECT_FALSE(classifier1.getAttributes().empty());
  EXPECT_EQ(classifier1.getAttributes().front(), &prop);
  EXPECT_EQ(classifier1.getAttributes().back(), propPtr);
  EXPECT_EQ(classifierPtr->getAttributes().front(), &prop2);
}

// TEST_F(ClassifierTest, AddOwnedAttributesTest) {
//   Classifier c;
//   Property p;
//   ASSERT_NO_THROW(c.addAttribute(p));
//   ASSERT_EQ(c.ownedAttributes.front()->getID(), p.getID());
// }

// TEST_F(ClassifierTest, RemoveOwnedAttributeTest) {
//   Classifier c;
//   Property p;
//   c.addAttribute(p);
//   ASSERT_NO_THROW(c.removeAttribute(p));
//   ASSERT_TRUE(c.ownedAttributes.empty());
// }