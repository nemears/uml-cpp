#include "gtest/gtest.h"
#include "../../../cpp/parsers/headers/classParser.h"
#include "../../../cpp/parsers/headers/inputParser.h"

class ElementParserTest : public ::testing::Test {
    public:
        ClassParser* pp, *invalidIDpp;
        InputParser* input, *invalidIdInput;
        boost::uuids::uuid id1, id2;
    protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  ElementParserTest() {
    pp = new ClassParser(new map<boost::uuids::uuid, Element*>);
    input = new InputParser("../../../../../src/test/yml/elementTests/element.yml"); // root file is the gmock_main which is like 7 down right now
    invalidIdInput = new InputParser("../../../../../src/test/yml/elementTests/invalidID.yml");
    invalidIDpp = new ClassParser(new map<boost::uuids::uuid, Element*>);
    // TODO fix google_test within directory structure
  }

  ~ElementParserTest() override {
    
  }

  void SetUp() override {
    // base functionality tests
    input->readNextLine();
    pp->parse(input);
    id1 = (*pp->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")]->ownedElements.back()->uuid;
    id2 = (*pp->elements)[boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d")]->uuid;

    // invalid ID tests
    invalidIdInput->readNextLine();
  }

  void TearDown() override {
    delete pp->elements;
    delete pp;
    delete input;
    delete invalidIDpp;
    delete invalidIDpp->elements;
    delete invalidIdInput;
  }

  // Class members declared here can be used by all tests in the test suite
  // for Element
};

TEST_F(ElementParserTest, ParseID_Test) {
    EXPECT_TRUE((*pp->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")] != NULL);
    EXPECT_TRUE((*pp->elements)[boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d")] != NULL);
    EXPECT_EQ((*pp->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")]->uuid, boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35"));
    EXPECT_EQ((*pp->elements)[boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d")]->uuid, boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d"));
}

TEST_F(ElementParserTest, ParseChildrenTest) {
    EXPECT_EQ((*pp->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")]->ownedElements.back()->uuid, id1);
}

TEST_F(ElementParserTest, ParseSingleCharID_Test) {
  EXPECT_NO_THROW(invalidIDpp->parse(invalidIdInput));
  EXPECT_TRUE(invalidIDpp->elements->empty() == false);
}