#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/opaqueBehavior.h"

using namespace std;
using namespace UML;

class OpaqueBehaviorParserTest : public ::testing::Test {
    public:
        string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(OpaqueBehaviorParserTest, parseMultipleSimpleBodies) {
    Element* el;
    ASSERT_NO_THROW(el = Parsers::parse(ymlPath + "opaqueBehaviorTests/multipleSimpleBodies.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::OPAQUE_BEHAVIOR);
    OpaqueBehavior* bhv = dynamic_cast<OpaqueBehavior*>(el);
    ASSERT_TRUE(bhv->getName().compare("test") == 0);
    ASSERT_TRUE(bhv->getID() == boost::lexical_cast<boost::uuids::uuid>("7d4b6b0b-f6c2-4670-868c-87709cede18e"));
    ASSERT_TRUE(bhv->getBodies().size() == 3);
    ASSERT_TRUE(bhv->getBodies().get(0)->getValue().compare("one") == 0);
    ASSERT_TRUE(bhv->getBodies().get(1)->getValue().compare("1") == 0);
    ASSERT_TRUE(bhv->getBodies().get(2)->getValue().compare("true") == 0);
}

TEST_F(OpaqueBehaviorParserTest, parseParameter) {
    Element* el;
    ASSERT_NO_THROW(el = Parsers::parse(ymlPath + "opaqueBehaviorTests/param.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::OPAQUE_BEHAVIOR);
    OpaqueBehavior* bhv = dynamic_cast<OpaqueBehavior*>(el);
    ASSERT_TRUE(bhv->getParameters().size() == 1);
    Parameter* param = bhv->getParameters().front();
    ASSERT_TRUE(param->getName().compare("test") == 0);
    ASSERT_TRUE(param->getDirection() == ParameterDirectionKind::IN);
}

TEST_F(OpaqueBehaviorParserTest, properParameters) {
    Element* el;
    ASSERT_THROW(el = Parsers::parse(ymlPath + "opaqueBehaviorTests/improperParameters.yml"), Parsers::UmlParserException);
    ASSERT_THROW(el = Parsers::parse(ymlPath + "opaqueBehaviorTests/bodyNotLiteralString.yml"), Parsers::UmlParserException);
    ASSERT_THROW(el = Parsers::parse(ymlPath + "opaqueBehaviorTests/bodiesEntryIsSequence.yml"), Parsers::UmlParserException);
    ASSERT_THROW(el = Parsers::parse(ymlPath + "opaqueBehaviorTests/bodiesNotSequence.yml"), Parsers::UmlParserException);
}

TEST_F(OpaqueBehaviorParserTest, emitBasicOpaqueBehavior) {
    OpaqueBehavior b;
    Property p;
    Operation o;
    b.setID("b278ca8a-9d1d-45d1-970f-403bc60998b3");
    b.setName("Opaque");
    p.setID("9c744c8c-ed4e-4c71-9c97-5d3e6115bc24");
    p.setName("prop");
    p.setVisibility(VisibilityKind::PRIVATE);
    o.setID("d2a0bcbd-a1aa-4953-9d95-b10a9a322fe3");
    o.setName("op");
    o.setVisibility(VisibilityKind::PROTECTED);
    b.getOwnedAttributes().add(p);
    b.getOperations().add(o);
    string expectedEmit = R""""(opaqueBehavior:
  id: b278ca8a-9d1d-45d1-970f-403bc60998b3
  name: Opaque
  ownedAttributes:
    - property:
        id: 9c744c8c-ed4e-4c71-9c97-5d3e6115bc24
        name: prop
        visibility: PRIVATE
  operations:
    - operation:
        id: d2a0bcbd-a1aa-4953-9d95-b10a9a322fe3
        name: op
        visibility: PROTECTED)"""";
    string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(b));
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}