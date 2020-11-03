#include "gtest/gtest.h"
#include "../../../cpp/uml/headers/element.h"

class ElementTest : public ::testing::Test {
    public:
    Element el;
    boost::uuids::uuid uuid;
    protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  ElementTest() {
     
  }

  ~ElementTest() override {
     // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     uuid = boost::uuids::random_generator()();
     el.setID(boost::lexical_cast<std::string>(uuid));
  }

  void TearDown() override {
   
  }

  // Class members declared here can be used by all tests in the test suite
  // for Foo.
};

TEST_F(ElementTest, OverrideID_Test) {
    EXPECT_EQ(el.getID(), uuid);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

