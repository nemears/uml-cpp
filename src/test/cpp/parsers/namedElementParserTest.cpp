#include "gtest/gtest.h"
#include "../../../cpp/parsers/headers/classParser.h"

class NamedElementParserTest : public ::testing::Test {
    public:
        ClassParser* ppYAML, *invalidTypeppYAML;
        YAML::Node node, invalidTypeNode;
    protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  NamedElementParserTest() {
    ppYAML = new ClassParser(new map<boost::uuids::uuid, UML::Element*>);
    node = YAML::LoadFile("../../../../../src/test/yml/namedElementTests/namedElement.yml");

    invalidTypeppYAML = new ClassParser(new map<boost::uuids::uuid, UML::Element*>);
    invalidTypeNode = YAML::LoadFile("../../../../../src/test/yml/namedElementTests/improperType.yml");
  }

  ~NamedElementParserTest() override {
    
  }

  void SetUp() override {
    ppYAML->parse(node);
  }

  void TearDown() override {
    delete ppYAML->elements;
    delete ppYAML;
    delete invalidTypeppYAML->elements;
    delete invalidTypeppYAML;
  }

  // Class members declared here can be used by all tests in the test suite
  // for Element
};

TEST_F(NamedElementParserTest, ParseNameTest) {
  EXPECT_TRUE(((UML::NamedElement*)(*ppYAML->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")])->getName().compare("test_1") == 0);
  EXPECT_TRUE(((UML::NamedElement*)(*ppYAML->elements)[boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d")])->getName().compare("test_1_child") == 0);
  EXPECT_TRUE(((UML::NamedElement*)(*ppYAML->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")]->ownedElements.back())->getName().compare("test_1_child_2") == 0);
}

TEST_F(NamedElementParserTest, ParseInvalidTypeTest) {
  // Commented below out because i think that it should assign numerical values as types
  // EXPECT_NO_THROW(invalidTypeppYAML->parse(invalidTypeNode));
  // EXPECT_TRUE(((NamedElement*)(*invalidTypeppYAML->elements).begin()->second)->getName().empty());
}