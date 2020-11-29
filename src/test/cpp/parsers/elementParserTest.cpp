#include "gtest/gtest.h"
#include "../../../cpp/parsers/headers/classParser.h"
#include "../../../cpp/parsers/headers/inputParser.h"
#include "pthread.h"

class ElementParserTest : public ::testing::Test {
    public:
        ClassParser* pp;
        InputParser* input;
        boost::uuids::uuid id1, id2;
    protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  ElementParserTest() {
    pp = new ClassParser(new map<boost::uuids::uuid, Element*>);
    input = new InputParser("../../../../../src/test/yml/element.yml"); // root file is the gmock_main which is like 7 down right now
    // TODO fix google_test within directory structure
  }

  ~ElementParserTest() override {
    
  }

  void SetUp() override {
    input->readNextLine();
    pp->parse(input);
    printf(boost::lexical_cast<std::string>((*pp->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")]->ownedElements.back()->uuid).c_str());
    id1 = (*pp->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")]->ownedElements.back()->uuid;
    id2 = (*pp->elements)[boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d")]->uuid;
    cout << "\nParent id:" << id1 << '\n';
    cout << "child id:" << id2 << '\n';
  }

  void TearDown() override {
    delete pp->elements;
    delete pp;
    delete input;
  }

  // Class members declared here can be used by all tests in the test suite
  // for Element
};

TEST_F(ElementParserTest, ParseID_Test) {
    EXPECT_TRUE((*pp->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")] != NULL);
    EXPECT_TRUE((*pp->elements)[boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d")] != NULL);
}

TEST_F(ElementParserTest, ParseChildrenTest) {
    ASSERT_TRUE(true);
}