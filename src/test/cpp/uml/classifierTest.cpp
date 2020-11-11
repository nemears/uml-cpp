#include "gtest/gtest.h"
#include "../../../cpp/uml/headers/classifier.h"

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
     classifier1.ownedAttributes.push_front(&prop);
     classifier1.ownedAttributes.push_back(propPtr);
     classifierPtr->ownedAttributes.push_back(&prop2);
  }

  void TearDown() override {
    // nothing right now
  }

  // Class members declared here can be used by all tests in the test suite
  // for Element
};

TEST_F(ClassifierTest, GetOwnedAttributesTest) {
  EXPECT_FALSE(classifier1.ownedAttributes.empty());
  EXPECT_EQ(classifier1.ownedAttributes.front(), &prop);
  EXPECT_EQ(classifier1.ownedAttributes.back(), propPtr);
  EXPECT_EQ(classifierPtr->ownedAttributes.front(), &prop2);
}