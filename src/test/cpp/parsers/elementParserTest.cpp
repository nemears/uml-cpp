#include "gtest/gtest.h"
#include "../../../cpp/parsers/headers/classParser.h"
#include "../../../cpp/parsers/headers/inputParser.h"

class ElementParserTest : public ::testing::Test {
    public:
        ClassParser* pp;
        InputParser* input;
    protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  ElementParserTest() {
    pp = new ClassParser(new map<boost::uuids::uuid, Element*>);
    input = new InputParser("../../../../../src/test/yml/element.yml"); // root file is the gmock_main which is like 7 down right now
    // TODO fix google_test within directory structure
  }

  ~ElementParserTest() override {
    delete pp;
    delete input;
  }

  void SetUp() override {
    input->readNextLine();
    pp->parse(input);
  }

  void TearDown() override {
    // nothing right now
  }

  // Class members declared here can be used by all tests in the test suite
  // for Element
};

TEST_F(ElementParserTest, ParseID_Test) {
    EXPECT_TRUE((*pp->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")] != NULL);
    EXPECT_TRUE((*pp->elements)[boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d")] != NULL);
}