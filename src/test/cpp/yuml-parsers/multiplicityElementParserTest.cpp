#include "gtest/gtest.h"
#include "yuml-parsers/modelParser.h"
#include "uml/class.h"
#include "uml/operation.h"

class MultiplicityElementParserTest : public ::testing::Test {

};

TEST_F(MultiplicityElementParserTest, ProperPropertyMultiplicityParsingTest) {
    ModelParser properPropertyMultiplicityParser(new map<boost::uuids::uuid, Element*>);
    YAML::Node properPropertyMultiplicityNode = YAML::LoadFile("../../../../../src/test/yml/multiplicityElementTests/properPropertyMultiplicty.yml");

    ASSERT_NO_THROW(properPropertyMultiplicityParser.parse(properPropertyMultiplicityNode));
    ASSERT_TRUE(dynamic_cast<Class*>(properPropertyMultiplicityParser.theEl->ownedElements.front())->ownedAttributes.front()->multiplicitySpecified());
}

TEST_F(MultiplicityElementParserTest, ProperParameterMultiplicityParsingTest) {
    ModelParser properParameterMultiplicityParser(new map<boost::uuids::uuid, Element*>);
    YAML::Node properParameterMultiplicityNode = YAML::LoadFile("../../../../../src/test/yml/multiplicityElementTests/properParameterMultiplicity.yml");
    ASSERT_NO_THROW(properParameterMultiplicityParser.parse(properParameterMultiplicityNode));
    ASSERT_TRUE(dynamic_cast<Class*>(properParameterMultiplicityParser.theEl->ownedElements.front())->operations.front()->parameters.front()->multiplicitySpecified());
}