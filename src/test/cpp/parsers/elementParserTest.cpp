#include "gtest/gtest.h"
#include "../../../cpp/parsers/headers/classParser.h"
#include "../../../cpp/parsers/headers/inputParser.h"
#include "../../../cpp/uml/headers/namedElement.h"

class ElementParserTest : public ::testing::Test {
    public:
        ClassParser* pp, *invalidIDpp, *emptyChildrenpp, *emptyChildren2pp, 
                    *ppYAML, *invalidIDppYAML, *emptyChildrenppYAML, *emptyChildren2ppYAML;
        InputParser* input, *invalidIdInput, *emptyChildrenInput, *emptyChildren2Input;
        YAML::Node node, invalidIDNode, emptyChildrenNode, emptyChildren2Node;
        boost::uuids::uuid id1, id2;
    protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  ElementParserTest() {
    pp = new ClassParser(new map<boost::uuids::uuid, Element*>);
    input = new InputParser("../../../../../src/test/yml/elementTests/element.yml"); // root file is the gmock_main which is like 7 down right now
    invalidIdInput = new InputParser("../../../../../src/test/yml/elementTests/invalidID.yml");
    invalidIDpp = new ClassParser(new map<boost::uuids::uuid, Element*>);
    emptyChildrenpp = new ClassParser(new map<boost::uuids::uuid, Element*>);
    emptyChildrenInput = new InputParser("../../../../../src/test/yml/elementTests/emptyChildren.yml");
    emptyChildren2pp = new ClassParser(new map<boost::uuids::uuid, Element*>);
    emptyChildren2Input = new InputParser("../../../../../src/test/yml/elementTests/emptyChildren2.yml");
    // TODO fix google_test within directory structure

    ppYAML = new ClassParser(new map<boost::uuids::uuid, Element*>);
    node = YAML::LoadFile("../../../../../src/test/yml/elementTests/element.yml");

    invalidIDppYAML = new ClassParser(new map<boost::uuids::uuid, Element*>);
    invalidIDNode = YAML::LoadFile("../../../../../src/test/yml/elementTests/invalidID.yml");

    emptyChildrenppYAML = new ClassParser(new map<boost::uuids::uuid, Element*>);
    emptyChildrenNode = YAML::LoadFile("../../../../../src/test/yml/elementTests/emptyChildren.yml");

    emptyChildren2ppYAML = new ClassParser(new map<boost::uuids::uuid, Element*>);
    emptyChildren2Node = YAML::LoadFile("../../../../../src/test/yml/elementTests/emptyChildren2.yml");
  }

  ~ElementParserTest() override {
    
  }

  void SetUp() override {
    // base functionality tests
    input->readNextLine();
    pp->parse(input);
    id1 = (*pp->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")]->ownedElements.back()->uuid;
    id2 = (*pp->elements)[boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d")]->uuid;

    ppYAML->parse(node);

    // invalid ID tests
    invalidIdInput->readNextLine();

    // empty children tests
    emptyChildrenInput->readNextLine();
    emptyChildren2Input->readNextLine();
  }

  void TearDown() override {
    delete pp->elements;
    delete pp;
    delete input;
    delete invalidIDpp->elements;
    delete invalidIDpp;
    delete invalidIdInput;
    delete emptyChildrenpp->elements;
    delete emptyChildrenpp;
    delete emptyChildrenInput;
    delete ppYAML->elements;
    delete ppYAML;
    delete invalidIDppYAML->elements;
    delete invalidIDppYAML;
    delete emptyChildrenppYAML->elements;
    delete emptyChildrenppYAML;
    delete emptyChildren2ppYAML->elements;
    delete emptyChildren2ppYAML;
  }

  // Class members declared here can be used by all tests in the test suite
  // for Element
};

TEST_F(ElementParserTest, ParseID_Test) {
    EXPECT_TRUE((*pp->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")] != NULL);
    EXPECT_TRUE((*pp->elements)[boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d")] != NULL);
    EXPECT_EQ((*pp->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")]->uuid, boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35"));
    EXPECT_EQ((*pp->elements)[boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d")]->uuid, boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d"));

    ASSERT_TRUE((*ppYAML->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")] != NULL);
    ASSERT_TRUE((*ppYAML->elements)[boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d")] != NULL);
    EXPECT_EQ((*ppYAML->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")]->uuid, boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35"));
    EXPECT_EQ((*ppYAML->elements)[boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d")]->uuid, boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d"));
}

TEST_F(ElementParserTest, ParseChildrenTest) {
    EXPECT_EQ((*pp->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")]->ownedElements.back()->uuid, id1);
}

TEST_F(ElementParserTest, ParseSingleCharID_Test) {
  EXPECT_NO_THROW(invalidIDpp->parse(invalidIdInput));
  EXPECT_TRUE(invalidIDpp->elements->empty() == false);
}

TEST_F(ElementParserTest, ParseEmptyChildrenTest) {
  EXPECT_NO_THROW(emptyChildrenpp->parse(emptyChildrenInput));
  EXPECT_TRUE(emptyChildrenpp->elements->empty() == false);
  EXPECT_EQ(((NamedElement*)emptyChildrenpp->elements->begin()->second)->getName(), "pete");
  EXPECT_NO_THROW(emptyChildren2pp->parse(emptyChildren2Input));
  EXPECT_TRUE(((NamedElement*)emptyChildren2pp->elements->begin()->second)->getName().empty());
}