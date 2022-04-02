#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/uml-cpp-paths.h"
#include "uml/uml-stable.h"

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
    UmlManager m;
    ASSERT_THROW(el = m.parse(ymlPath + "parameterTests/invalidDirection.yml").ptr(), Parsers::UmlParserException);
    ASSERT_THROW(el = m.parse(ymlPath + "parameterTests/invalidDirection2.yml").ptr(), Parsers::UmlParserException);
}

TEST_F(ParameterParserTest, emitParameterWMultiplicityTest) {
    UmlManager m;
    OpaqueBehavior& b = *m.create<OpaqueBehavior>();
    Parameter& p = *m.create<Parameter>();
    b.setID("SeJ_0hSPaIa4EYap3sXgRQm4LuSn");
    p.setID("QzBhVkqNSRAZMlkHU6cQ3d_Wm8FT");
    LiteralInt& low = *m.create<LiteralInt>();
    LiteralInt& high = *m.create<LiteralInt>();
    low.setID("K2_2cpL966uyziy9e2pGb60cCR5X");
    low.setValue(0);
    high.setID("sGGXJFNinAvKyKVSgc&JAlMWsbw5");
    high.setValue(5);
    p.setLowerValue(&low);
    p.setUpperValue(&high);
    b.getOwnedParameters().add(p);
    string expectedEmit = R""""(opaqueBehavior:
  id: SeJ_0hSPaIa4EYap3sXgRQm4LuSn
  parameters:
    - parameter:
        id: QzBhVkqNSRAZMlkHU6cQ3d_Wm8FT
        lowerValue:
          literalInt:
            id: K2_2cpL966uyziy9e2pGb60cCR5X
            value: 0
        upperValue:
          literalInt:
            id: sGGXJFNinAvKyKVSgc&JAlMWsbw5
            value: 5)"""";
    string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(b));
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}