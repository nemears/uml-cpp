#include "gtest/gtest.h"
#include "test/yumlParsersTest.h"
#include "yuml-parsers/modelParser.h"
#include "uml/class.h"
#include "uml/operation.h"

class MultiplicityElementParserTest : public ::testing::Test {
    public:
        string ymlPath;
        void SetUp() override {
            ymlPath = YML_FILES_PATH;
        };
};

TEST_F(MultiplicityElementParserTest, ProperPropertyMultiplicityParsingTest) {
    ModelParser properPropertyMultiplicityParser = ModelParser::createNewParser();
    YAML::Node properPropertyMultiplicityNode = YAML::LoadFile(ymlPath + "multiplicityElementTests/properPropertyMultiplicty.yml");

    ASSERT_NO_THROW(properPropertyMultiplicityParser.parse(properPropertyMultiplicityNode));
    ASSERT_TRUE(dynamic_cast<Class*>(properPropertyMultiplicityParser.theEl->getOwnedElements().front())->getAttributes().front()->multiplicitySpecified());
}

TEST_F(MultiplicityElementParserTest, ProperParameterMultiplicityParsingTest) {
    ModelParser properParameterMultiplicityParser = ModelParser::createNewParser();
    YAML::Node properParameterMultiplicityNode = YAML::LoadFile(ymlPath + "multiplicityElementTests/properParameterMultiplicity.yml");
    ASSERT_NO_THROW(properParameterMultiplicityParser.parse(properParameterMultiplicityNode));
    ASSERT_TRUE(dynamic_cast<Class*>(properParameterMultiplicityParser.theEl->getOwnedElements().front())->getOperations().front()->getParameters().front()->multiplicitySpecified());
}