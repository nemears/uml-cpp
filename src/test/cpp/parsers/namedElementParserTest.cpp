#include "gtest/gtest.h"
#include "../../../cpp/parsers/headers/classParser.h"
#include "../../../cpp/parsers/headers/inputParser.h"

class NamedElementParserTest : public ::testing::Test {
    public:
        ClassParser* pp, *invalidTypepp,
                    *ppYAML, *invalidTypeppYAML;
        InputParser* input, *invalidTypeInput;
        YAML::Node node, invalidTypeNode;
    protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  NamedElementParserTest() {
    pp = new ClassParser(new map<boost::uuids::uuid, Element*>);
    input = new InputParser("../../../../../src/test/yml/namedElementTests/namedElement.yml"); // root file is the gmock_main which is like 7 down right now
    // TODO fix google_test within directory structure

    //Invalid type tests
    invalidTypepp = new ClassParser(new map<boost::uuids::uuid, Element*>);
    invalidTypeInput = new InputParser("../../../../../src/test/yml/namedElementTests/improperType.yml");

    ppYAML = new ClassParser(new map<boost::uuids::uuid, Element*>);
    node = YAML::LoadFile("../../../../../src/test/yml/namedElementTests/namedElement.yml");

    invalidTypeppYAML = new ClassParser(new map<boost::uuids::uuid, Element*>);
    invalidTypeNode = YAML::LoadFile("../../../../../src/test/yml/namedElementTests/improperType.yml");
  }

  ~NamedElementParserTest() override {
    
  }

  void SetUp() override {
    input->readNextLine();
    pp->parse(input);

    // Invalid type tests
    invalidTypeInput->readNextLine();

    ppYAML->parse(node);
  }

  void TearDown() override {
    delete pp->elements;
    delete pp;
    delete input;
    delete invalidTypepp->elements;
    delete invalidTypepp;
    delete invalidTypeInput;
    delete ppYAML->elements;
    delete ppYAML;
    delete invalidTypeppYAML->elements;
    delete invalidTypeppYAML;
  }

  // Class members declared here can be used by all tests in the test suite
  // for Element
};

TEST_F(NamedElementParserTest, ParseNameTest) {
    EXPECT_TRUE(((NamedElement*)(*pp->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")])->getName().compare("test_1") == 0);
    EXPECT_TRUE(((NamedElement*)(*pp->elements)[boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d")])->getName().compare("test_1_child") == 0);
    EXPECT_TRUE(((NamedElement*)(*pp->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")]->ownedElements.back())->getName().compare("test_1_child_2") == 0);
    
    EXPECT_TRUE(((NamedElement*)(*ppYAML->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")])->getName().compare("test_1") == 0);
    EXPECT_TRUE(((NamedElement*)(*ppYAML->elements)[boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d")])->getName().compare("test_1_child") == 0);
    EXPECT_TRUE(((NamedElement*)(*ppYAML->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")]->ownedElements.back())->getName().compare("test_1_child_2") == 0);
}

TEST_F(NamedElementParserTest, ParseInvalidTypeTest) {
  EXPECT_NO_THROW(invalidTypepp->parse(invalidTypeInput));
  EXPECT_TRUE(((NamedElement*)(*invalidTypepp->elements).begin()->second)->getName().empty());

  // Commented below out because i think that it should assign numerical values as types
  // EXPECT_NO_THROW(invalidTypeppYAML->parse(invalidTypeNode));
  // EXPECT_TRUE(((NamedElement*)(*invalidTypeppYAML->elements).begin()->second)->getName().empty());
}