#include "gtest/gtest.h"
#include "yuml-parsers/modelParser.h"
#include "uml/classifier.h"

class ClassifierParserTest : public ::testing::Test {
    public:
        ModelParser* pp2;
        YAML::Node modelNode;
        
    protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  ClassifierParserTest() {
    pp2 = new ModelParser(new map<boost::uuids::uuid, UML::Element*>);
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
  EXPECT_EQ(((UML::Classifier*)(*pp2->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")])->ownedAttributes.front()->uuid , boost::lexical_cast<boost::uuids::uuid>("16c345b4-5ae2-41ca-a0e7-a9c386ac941d"));
  EXPECT_EQ(((UML::Classifier*)(*pp2->elements)[boost::lexical_cast<boost::uuids::uuid>("7d18ee42-82c6-4f52-8ec4-fab67a75ff35")])->ownedAttributes.back()->uuid, boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033"));
}