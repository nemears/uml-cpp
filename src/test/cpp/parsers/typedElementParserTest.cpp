#include "gtest/gtest.h"
#include "../../../cpp/parsers/headers/modelParser.h"
#include "../../../cpp/parsers/headers/inputParser.h"
#include "../../../cpp/uml/headers/typedElement.h"

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
    EXPECT_EQ(((TypedElement*)(*pp->elements)[boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d")])->getType()->uuid, boost::lexical_cast<boost::uuids::uuid>("c0ab87cc-d00b-4afb-9558-538253b442b2"));
    EXPECT_EQ(((TypedElement*)(*pp->elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->getType()->uuid, boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35"));
}