#include "gtest/gtest.h"
#include "yuml-parsers/classParser.h"
#include "yuml-parsers/modelParser.h"
#include "uml/namedElement.h"
#include "uml/model.h"
#include "uml/class.h"

class ElementParserTest : public ::testing::Test {
    public:
        ClassParser* ppYAML, *invalidIDppYAML, *emptyChildrenppYAML, *emptyChildren2ppYAML;
        YAML::Node node, invalidIDNode, emptyChildrenNode, emptyChildren2Node;
        boost::uuids::uuid id1, id2;
    protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  ElementParserTest() {
    // TODO fix google_test within directory structure
    ppYAML = new ClassParser(new map<boost::uuids::uuid, UML::Element*>);
    node = YAML::LoadFile("../../../../../src/test/yml/elementTests/element.yml");

    invalidIDppYAML = new ClassParser(new map<boost::uuids::uuid, UML::Element*>);
    invalidIDNode = YAML::LoadFile("../../../../../src/test/yml/elementTests/invalidID.yml");

    emptyChildrenppYAML = new ClassParser(new map<boost::uuids::uuid, UML::Element*>);
    emptyChildrenNode = YAML::LoadFile("../../../../../src/test/yml/elementTests/emptyChildren.yml");

    emptyChildren2ppYAML = new ClassParser(new map<boost::uuids::uuid, UML::Element*>);
    emptyChildren2Node = YAML::LoadFile("../../../../../src/test/yml/elementTests/emptyChildren2.yml");
  }

  ~ElementParserTest() override {
    
  }

  void SetUp() override {
    ppYAML->parse(node);
  }

  void TearDown() override {
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
  ASSERT_TRUE((*ppYAML->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")] != NULL);
  ASSERT_TRUE((*ppYAML->elements)[boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d")] != NULL);
  EXPECT_EQ((*ppYAML->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")]->uuid, boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35"));
  EXPECT_EQ((*ppYAML->elements)[boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d")]->uuid, boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d"));
}

TEST_F(ElementParserTest, ParseChildrenTest) {
  EXPECT_EQ((*ppYAML->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")]->ownedElements.back()->uuid, 
        (*ppYAML->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")]->ownedElements.back()->uuid);
}

TEST_F(ElementParserTest, ParseSingleCharID_Test) {
  EXPECT_NO_THROW(invalidIDppYAML->parse(invalidIDNode));
  EXPECT_TRUE(invalidIDppYAML->elements->empty() == false);
}

TEST_F(ElementParserTest, ParseEmptyChildrenTest) {
  EXPECT_THROW(emptyChildrenppYAML->parse(emptyChildrenNode), ElementParser::InvalidNodeTypeException);
  EXPECT_TRUE(emptyChildrenppYAML->elements->empty() == false);
  EXPECT_EQ(((UML::NamedElement*)emptyChildrenppYAML->elements->begin()->second)->getName(), "pete");
  EXPECT_THROW(emptyChildren2ppYAML->parse(emptyChildren2Node), ElementParser::InvalidNodeTypeException);
  EXPECT_TRUE(((UML::NamedElement*)emptyChildren2ppYAML->elements->begin()->second)->getName().empty());
}

TEST_F(ElementParserTest, ThrowInvalidIdentifierExceptionTest) {
  // Setup
  ModelParser invalidIdentifierTestParser(new map<boost::uuids::uuid, Element*>);
  YAML::Node invalidIdentifierTestNode = YAML::LoadFile("../../../../../src/test/yml/elementTests/invalidIdentifierError.yml");

  // Test
  ASSERT_THROW(invalidIdentifierTestParser.parse(invalidIdentifierTestNode), ElementParser::InvalidIdentifierException);
}

TEST_F(ElementParserTest, ThrowAbstractEmitExceptionTest) {
  // Setup
  Model m;
  Element e;
  m.ownedElements.push_back(&e);
  ModelParser abstractEmitExceptionParser(new map<boost::uuids::uuid, Element*>);

  // Test
  ASSERT_THROW(abstractEmitExceptionParser.emitDocument(&m), ElementParser::AbstractTypeEmitException);
}

TEST_F(ElementParserTest, EmitBasicIDTest) {
  Model el;
  el.setID("7d18ee42-82c6-4f52-8ec4-fab67a75ff35");

  string expectedEmit = R""""(model:
  id: 7d18ee42-82c6-4f52-8ec4-fab67a75ff35)"""";
  
  ModelParser mp(new map<boost::uuids::uuid, Element*>);
  string generatedEmit;
  YAML::Emitter emitter;
  ASSERT_NO_THROW(mp.emit(emitter, &el));
  generatedEmit = emitter.c_str();
  cout << generatedEmit << '\n';
  ASSERT_TRUE(emitter.good());
  ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(ElementParserTest, EmitBasicChildrenOfTypeClass) {
  Model el;
  el.setID("7d18ee42-82c6-4f52-8ec4-fab67a75ff35");
  Class c;
  c.setID("c0ab87cc-d00b-4afb-9558-538253b442b2");
  el.ownedElements.push_back(&c);

  string expectedEmit = R""""(model:
  id: 7d18ee42-82c6-4f52-8ec4-fab67a75ff35
  children:
    - class:
        id: c0ab87cc-d00b-4afb-9558-538253b442b2)"""";

  ModelParser mp(new map<boost::uuids::uuid, Element*>);
  string generatedEmit;
  YAML::Emitter emitter;
  ASSERT_NO_THROW(mp.emit(emitter, &el));
  generatedEmit = emitter.c_str();
  cout << generatedEmit << '\n';
  ASSERT_TRUE(emitter.good());
  ASSERT_EQ(expectedEmit, generatedEmit);
}