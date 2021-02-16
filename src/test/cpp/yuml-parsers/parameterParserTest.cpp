#include "gtest/gtest.h"
#include "yuml-parsers/modelParser.h"
#include "uml/parameter.h"

using namespace UML;

class ParameterParserTest : public ::testing::Test {

};

TEST_F(ParameterParserTest, TestParseDirection) {
    // Setup
    ModelParser directionParser = ModelParser::createNewParser();
    YAML::Node directionNode = YAML::LoadFile("../../../../../src/test/yml/parameterTests/parameterDirection.yml");

    // Test
    ASSERT_NO_THROW(directionParser.parse(directionNode));
    ASSERT_TRUE(dynamic_cast<Parameter*>((*directionParser.elements)[boost::lexical_cast<boost::uuids::uuid>("190d1cb9-13dc-44e6-a064-126891ae0033")])->getDirection() == ParameterDirectionKind::IN);
}

TEST_F(ParameterParserTest, TestEmitDirection) {
    // Setup
    Model m;
    m.setID("190d1cb9-13dc-44e6-a064-126891ae0033");
    Parameter p;
    p.setID("16c345b4-5ae2-41ca-a0e7-a9c386ac941d");
    p.setDirection(ParameterDirectionKind::RETURN);
    m.ownedElements.push_back(&p);

    string expectedEmit = R""""(model:
  id: 190d1cb9-13dc-44e6-a064-126891ae0033
  children:
    - parameter:
        id: 16c345b4-5ae2-41ca-a0e7-a9c386ac941d
        direction: RETURN)"""";

    ModelParser mp = ModelParser::createNewParser();
    string generatedEmit;
    YAML::Emitter emitter;
    ASSERT_NO_THROW(mp.emit(emitter, &m));
    generatedEmit = emitter.c_str();
    cout << generatedEmit << '\n';
    ASSERT_TRUE(emitter.good());
    ASSERT_EQ(expectedEmit, generatedEmit);
}