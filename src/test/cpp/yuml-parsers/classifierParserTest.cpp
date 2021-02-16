#include "gtest/gtest.h"
#include "yuml-parsers/modelParser.h"
#include "uml/classifier.h"
#include "uml/class.h"

using namespace UML;

class ClassifierParserTest : public ::testing::Test {
    public:
        ModelParser* pp2;
        YAML::Node modelNode;
        
    protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  ClassifierParserTest() {
    pp2 = new ModelParser(new map<boost::uuids::uuid, Element*>, new map<boost::uuids::uuid, PostParser*>);
    modelNode = YAML::LoadFile("../../../../../src/test/yml/classifierTests/classifier.yml");
    // TODO fix google_test within directory structure
  }

  ~ClassifierParserTest() override {
    
  }

  void SetUp() override {
    pp2->parse(modelNode);

  }

  void TearDown() override {
    delete pp2->elements;
    delete pp2;
  }

  // Class members declared here can be used by all tests in the test suite
  // for Element
};

TEST_F(ClassifierParserTest, ParseAttributesTest) {
  EXPECT_EQ(dynamic_cast<Classifier*>((*pp2->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")])->ownedAttributes.front()->uuid , boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d"));
  EXPECT_EQ(dynamic_cast<Classifier*>((*pp2->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")])->ownedAttributes.back()->uuid, boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033"));
}

TEST_F(ClassifierParserTest, ParseAttributesNotSequenceTest) {
  // Setup
  ModelParser attributesNotSequenceParser(new map<boost::uuids::uuid, Element*>, new map<boost::uuids::uuid, PostParser*>);
  YAML::Node attributesNotSequenceNode = YAML::LoadFile("../../../../../src/test/yml/classifierTests/invalidAttributes.yml");

  // Test
  ASSERT_THROW(attributesNotSequenceParser.parse(attributesNotSequenceNode), ElementParser::InvalidNodeTypeException);
}

TEST_F(ClassifierParserTest, EmitSingleBasicAttributeTest) {
  Model el;
  el.setID("7d18ee42-82c6-4f52-8ec4-fab67a75ff35");
  Class c;
  c.setID("c0ab87cc-d00b-4afb-9558-538253b442b2");
  Property p;
  p.setID("16c345b4-5ae2-41ca-a0e7-a9c386ac941d");
  c.ownedAttributes.push_back(&p);
  el.ownedElements.push_back(&c);

  string expectedEmit = R""""(model:
  id: 7d18ee42-82c6-4f52-8ec4-fab67a75ff35
  children:
    - class:
        id: c0ab87cc-d00b-4afb-9558-538253b442b2
        attributes:
          - property:
              id: 16c345b4-5ae2-41ca-a0e7-a9c386ac941d)"""";

  ModelParser mp = ModelParser::createNewParser();
  string generatedEmit;
  YAML::Emitter emitter;
  ASSERT_NO_THROW(mp.emit(emitter, &el));
  generatedEmit = emitter.c_str();
  cout << generatedEmit << '\n';
  ASSERT_TRUE(emitter.good());
  ASSERT_EQ(expectedEmit, generatedEmit);
}