#include "gtest/gtest.h"
#include "test/yumlParsersTest.h"
#include "yuml-parsers/modelParser.h"
#include "uml/classifier.h"
#include "uml/property.h"
#include "uml/class.h"
#include "uml/generalization.h"

using namespace UML;

class ClassifierParserTest : public ::testing::Test {
    public:
        ModelParser* pp2;
        YAML::Node modelNode;
        string ymlPath;
        
    protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  ClassifierParserTest() {
    ymlPath = YML_FILES_PATH;
    pp2 = new ModelParser(new map<boost::uuids::uuid, Element*>, new map<boost::uuids::uuid, PostParser*>);
    modelNode = YAML::LoadFile(ymlPath + "classifierTests/classifier.yml");
    // TODO fix google_test within directory structure
  }

  ~ClassifierParserTest() override {
    
  }

  void SetUp() override {
    pp2->parse(modelNode);
  }

  void TearDown() override {
    delete pp2->elements;
    delete pp2->postProcessFlag;
    delete pp2;
  }

  // Class members declared here can be used by all tests in the test suite
  // for Element
};

TEST_F(ClassifierParserTest, ParseAttributesTest) {
  EXPECT_EQ(dynamic_cast<Classifier*>((*pp2->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")])->getAttributes().front()->getID() , boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d"));
  EXPECT_EQ(dynamic_cast<Classifier*>((*pp2->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")])->getAttributes().back()->getID(), boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033"));
  EXPECT_EQ(dynamic_cast<Classifier*>(pp2->theEl->getOwnedElements().back())->getID(), dynamic_cast<Classifier*>(pp2->theEl->getOwnedElements().back())->getAttributes().front()->getClassifier()->getID());
}

TEST_F(ClassifierParserTest, ParseAttributesNotSequenceTest) {
  // Setup
  ModelParser attributesNotSequenceParser(new map<boost::uuids::uuid, Element*>, new map<boost::uuids::uuid, PostParser*>);
  YAML::Node attributesNotSequenceNode = YAML::LoadFile(ymlPath + "classifierTests/invalidAttributes.yml");

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
  c.getAttributes().add(p);
  el.getOwnedElements().add(c);

  string expectedEmit = R""""(model:
  id: 7d18ee42-82c6-4f52-8ec4-fab67a75ff35
  children:
    - class:
        id: c0ab87cc-d00b-4afb-9558-538253b442b2
        children:
          - 16c345b4-5ae2-41ca-a0e7-a9c386ac941d
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

TEST_F(ClassifierParserTest, ParseBasicGeneralizationTest) {
  // Setup
  ModelParser basicGeneralizationParser = ModelParser::createNewParser();
  YAML::Node basicGeneralizationNode = YAML::LoadFile(ymlPath + "classifierTests/generalization.yml");
  
  // TEST
  ASSERT_NO_THROW(basicGeneralizationParser.parse(basicGeneralizationNode));
  Model* m = dynamic_cast<Model*>(basicGeneralizationParser.theEl);
  
  ASSERT_TRUE(m->getOwnedElements().size() == 2);
  ASSERT_TRUE(m->getOwnedElements().front()->getElementType() == ElementType::CLASS);
  ASSERT_TRUE(m->getOwnedElements().back()->getElementType() == ElementType::CLASS);

  Classifier* c1 = dynamic_cast<Classifier*>(m->getOwnedElements().front());
  Classifier* c2 = dynamic_cast<Classifier*>(m->getOwnedElements().back());
  
  ASSERT_TRUE(c1->getGeneralizations().size() == 0);
  ASSERT_TRUE(c2->getGeneralizations().size() == 1);
  ASSERT_TRUE(c1->getRelationships().size() == 1);
  ASSERT_TRUE(c2->getRelationships().size() == 1);

  Generalization* g = c2->getGeneralizations().front();
  ASSERT_TRUE(g->getGeneral() == c1);
  ASSERT_TRUE(g->getSpecific() == c2);
}

TEST_F(ClassifierParserTest, EmitSingleGeneralizationTest) {
  // Setup
  Model m;
  m.setID("7d18ee42-82c6-4f52-8ec4-fab67a75ff35");
  Class g;
  g.setID("16c345b4-5ae2-41ca-a0e7-a9c386ac941d");
  Class s;
  s.setID("190d1cb9-13dc-44e6-a064-126891ae0033");
  s.getGenerals().add(g);
  s.getGeneralizations().front()->setID("c0ab87cc-d00b-4afb-9558-538253b442b2");
  m.getOwnedElements().add(g);
  m.getOwnedElements().add(s);

  string expectedEmit = R""""(model:
  id: 7d18ee42-82c6-4f52-8ec4-fab67a75ff35
  children:
    - class:
        id: 16c345b4-5ae2-41ca-a0e7-a9c386ac941d
        relationships:
          - c0ab87cc-d00b-4afb-9558-538253b442b2
    - class:
        id: 190d1cb9-13dc-44e6-a064-126891ae0033
        relationships:
          - c0ab87cc-d00b-4afb-9558-538253b442b2
        generalizations:
          - generalization:
              id: c0ab87cc-d00b-4afb-9558-538253b442b2
              relatedElements:
                - 16c345b4-5ae2-41ca-a0e7-a9c386ac941d
                - 190d1cb9-13dc-44e6-a064-126891ae0033
              targets:
                - 16c345b4-5ae2-41ca-a0e7-a9c386ac941d
              sources:
                - 190d1cb9-13dc-44e6-a064-126891ae0033
              general: 16c345b4-5ae2-41ca-a0e7-a9c386ac941d
              specific: 190d1cb9-13dc-44e6-a064-126891ae0033)"""";

  ModelParser mp = ModelParser::createNewParser();
  string generatedEmit;
  YAML::Emitter emitter;
  ASSERT_NO_THROW(mp.emit(emitter, &m));
  generatedEmit = emitter.c_str();
  cout << generatedEmit << '\n';
  ASSERT_TRUE(emitter.good());
  ASSERT_EQ(expectedEmit, generatedEmit);
}