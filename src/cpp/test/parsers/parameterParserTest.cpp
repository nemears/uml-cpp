#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/parameter.h"

using namespace std;
using namespace UML;

class ParameterParserTest : public ::testing::Test {
    public:
        string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(ParameterParserTest, properExceptions) {
    Element* el;
    ASSERT_THROW(el = Parsers::parse(YAML::LoadFile(ymlPath + "parameterTests/invalidDirection.yml")), Parsers::UmlParserException);
    ASSERT_THROW(el = Parsers::parse(YAML::LoadFile(ymlPath + "parameterTests/invalidDirection2.yml")), Parsers::UmlParserException);
}