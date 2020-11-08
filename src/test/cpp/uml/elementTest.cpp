#include "gtest/gtest.h"
#include "../../../cpp/uml/headers/element.h"
#include "../../../cpp/uml/headers/class.h"

class ElementTest : public ::testing::Test {
    public:
    Element* el1;
    Element* el2;
    Element* el3;
    boost::uuids::uuid uuid;

    protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  ElementTest() {
     el1 = new Element;
     el2 = new Element;
     el3 = new Element;
  }

  ~ElementTest() override {
     delete el1;
     delete el2;
     delete el3;
  }

  void SetUp() override {
    // Override uuid on Element (not reccomended use of lib)
     uuid = boost::uuids::random_generator()();
     el1->setID(boost::lexical_cast<std::string>(uuid));

     // add element to owned element list
     el2->ownedElements.push_front(el3);
  }

  void TearDown() override {
   
  }

  // Class members declared here can be used by all tests in the test suite
  // for Foo.
};

TEST_F(ElementTest, OverrideID_Test) {
    EXPECT_EQ(el1->uuid, uuid);
}

TEST_F(ElementTest, GetOwnedElementsTest) {
  EXPECT_FALSE(el2->ownedElements.empty());
  EXPECT_EQ(el2->ownedElements.front(), el3);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

