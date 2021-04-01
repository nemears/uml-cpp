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

TEST_F(ClassifierTest, addAttributeFunctorTest) {
  Classifier c;
  Property p;
  c.getAttributes().add(p);
  ASSERT_TRUE(c.getAttributes().size() == 1);
  ASSERT_TRUE(c.getAttributes().front() == &p);
  ASSERT_TRUE(p.getClassifier() == &c);
  ASSERT_TRUE(c.getMembers().count(p.getID()));
  ASSERT_TRUE(c.getOwnedElements().count(p.getID()));
  ASSERT_TRUE(p.getNamespace() == &c);
  ASSERT_TRUE(p.getOwner() == &c);
}

TEST_F(ClassifierTest, setClassifierTest) {
  Property p;
  Classifier c;
  p.setClassifier(&c);
  ASSERT_TRUE(c.getAttributes().size() == 1);
  ASSERT_TRUE(c.getAttributes().front() == &p);
  ASSERT_TRUE(p.getClassifier() == &c);
}

TEST_F(ClassifierTest, removeAttributeFunctorTest) {
  Property p;
  Classifier c;
  c.getAttributes().add(p);
  ASSERT_NO_THROW(c.getAttributes().remove(p));
  ASSERT_TRUE(c.getAttributes().size() == 0);
  ASSERT_TRUE(c.getMembers().size() == 0);
  ASSERT_TRUE(c.getOwnedElements().size() == 0);
  ASSERT_TRUE(!p.getClassifier());
  ASSERT_TRUE(!p.getNamespace());
  ASSERT_TRUE(!p.getOwner());
}