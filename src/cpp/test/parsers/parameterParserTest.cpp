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
    ASSERT_THROW(el = Parsers::parse(ymlPath + "parameterTests/invalidDirection.yml"), Parsers::UmlParserException);
    ASSERT_THROW(el = Parsers::parse(ymlPath + "parameterTests/invalidDirection2.yml"), Parsers::UmlParserException);
}

// TEST_F(ParameterParserTest, emitParameterWMultiplicityTest) {
//     OpaqueBehavior b;
//     Parameter p;
//     b.setID("b278ca8a-9d1d-45d1-970f-403bc60998b3");
//     p.setID("d2a0bcbd-a1aa-4953-9d95-b10a9a322fe3");
// }