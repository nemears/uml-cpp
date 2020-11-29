#include "gtest/gtest.h"
#include "../../../cpp/parsers/headers/modelParser.h"
#include "../../../cpp/parsers/headers/inputParser.h"

class TypedElementParserTest : public ::testing::Test {
    public:
        ModelParser* pp;
        InputParser* input;
    protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  TypedElementParserTest() {
    pp = new ModelParser(new map<boost::uuids::uuid, Element*>);
    input = new InputParser("../../../../../src/test/yml/typedElement.yml"); // root file is the gmock_main which is like 7 down right now
    // TODO fix google_test within directory structure
  }

  ~TypedElementParserTest() override {
    
  }

  void SetUp() override {
    input->readNextLine();
    pp->parse(input);
  }

  void TearDown() override {
    delete pp->elements;
    delete pp;
    delete input;
  }

  // Class members declared here can be used by all tests in the test suite
  // for Element
};

TEST_F(TypedElementParserTest, ParseTypeTest) {
    EXPECT_TRUE(true);
}