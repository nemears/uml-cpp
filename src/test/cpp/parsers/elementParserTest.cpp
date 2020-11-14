#include "gtest/gtest.h"
#include "../../../cpp/parsers/headers/elementParser.h"

class ElementParserTest : public ::testing::Test {
    public:
    // Element* el1;
    // Element* el2;
    // Element* el3;
    // boost::uuids::uuid uuid;

    protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  ElementParserTest() {
    //  el1 = new Element;
    //  el2 = new Element;
    //  el3 = new Element;
  }

  ~ElementParserTest() override {
    //  delete el1;
    //  delete el2;
    //  delete el3;
  }

  void SetUp() override {
    // Override uuid on Element (not reccomended use of lib)
    //  uuid = boost::uuids::random_generator()();
    //  el1->setID(boost::lexical_cast<std::string>(uuid));

    //  // add element to owned element list
    //  el2->ownedElements.push_front(el3);
  }

  void TearDown() override {
    // nothing right now
  }

  // Class members declared here can be used by all tests in the test suite
  // for Element
};

TEST_F(ElementParserTest, ParseID_Test) {
    // EXPECT_EQ(el1->uuid, uuid);
}