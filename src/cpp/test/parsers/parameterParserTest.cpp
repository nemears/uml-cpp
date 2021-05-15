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

TEST_F(ParameterParserTest, emitParameterWMultiplicityTest) {
    OpaqueBehavior b;
    Parameter p;
    b.setID("b278ca8a-9d1d-45d1-970f-403bc60998b3");
    p.setID("d2a0bcbd-a1aa-4953-9d95-b10a9a322fe3");
    LiteralInt low;
    LiteralInt high;
    low.setID("9c744c8c-ed4e-4c71-9c97-5d3e6115bc24");
    low.setValue(0);
    high.setID("8823326b-4882-460c-a4b2-3a1052ef6eb3");
    high.setValue(5);
    p.setLowerValue(&low);
    p.setUpperValue(&high);
    b.getParameters().add(p);
    string expectedEmit = R""""(opaqueBehavior:
  id: b278ca8a-9d1d-45d1-970f-403bc60998b3
  parameters:
    - parameter:
        id: d2a0bcbd-a1aa-4953-9d95-b10a9a322fe3
        lower:
          literalInt:
            id: 9c744c8c-ed4e-4c71-9c97-5d3e6115bc24
            value: 0
        upper:
          literalInt:
            id: 8823326b-4882-460c-a4b2-3a1052ef6eb3
            value: 5)"""";
    string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(b));
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}