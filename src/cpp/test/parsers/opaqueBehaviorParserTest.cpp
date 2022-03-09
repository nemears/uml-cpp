#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/uml-stable.h"

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
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "opaqueBehaviorTests/multipleSimpleBodies.yml").ptr());
    ASSERT_TRUE(el->getElementType() == ElementType::OPAQUE_BEHAVIOR);
    OpaqueBehavior* bhv = dynamic_cast<OpaqueBehavior*>(el);
    ASSERT_TRUE(bhv->getName().compare("test") == 0);
    ASSERT_TRUE(bhv->getID() == ID::fromString("i0wopIpBjBHdekQ57DbWeHfWmQp3"));
    ASSERT_TRUE(bhv->getBodies().size() == 3);
    ASSERT_TRUE(bhv->getBodies().get(0).getValue().compare("one") == 0);
    ASSERT_TRUE(bhv->getBodies().get(1).getValue().compare("1") == 0);
    ASSERT_TRUE(bhv->getBodies().get(2).getValue().compare("true") == 0);
}

TEST_F(OpaqueBehaviorParserTest, parseParameter) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "opaqueBehaviorTests/param.yml").ptr());
    ASSERT_TRUE(el->getElementType() == ElementType::OPAQUE_BEHAVIOR);
    OpaqueBehavior* bhv = dynamic_cast<OpaqueBehavior*>(el);
    ASSERT_TRUE(bhv->getOwnedParameters().size() == 1);
    Parameter* param = &bhv->getOwnedParameters().front();
    ASSERT_TRUE(param->getName().compare("test") == 0);
    ASSERT_TRUE(param->getDirection() == ParameterDirectionKind::IN);
}

TEST_F(OpaqueBehaviorParserTest, properParameters) {
    Element* el;
    UmlManager m;
    ASSERT_THROW(el = m.parse(ymlPath + "opaqueBehaviorTests/improperParameters.yml").ptr(), Parsers::UmlParserException);
    ASSERT_THROW(el = m.parse(ymlPath + "opaqueBehaviorTests/bodyNotLiteralString.yml").ptr(), Parsers::UmlParserException);
    ASSERT_THROW(el = m.parse(ymlPath + "opaqueBehaviorTests/bodiesEntryIsSequence.yml").ptr(), Parsers::UmlParserException);
    ASSERT_THROW(el = m.parse(ymlPath + "opaqueBehaviorTests/bodiesNotSequence.yml").ptr(), Parsers::UmlParserException);
}

TEST_F(OpaqueBehaviorParserTest, emitBasicOpaqueBehavior) {
    UmlManager m;
    OpaqueBehavior& b = *m.create<OpaqueBehavior>();
    Property& p = *m.create<Property>();
    Operation& o = *m.create<Operation>();
    b.setID("0FTAvf5nrXsknnc60ziElK2TDb7D");
    b.setName("Opaque");
    p.setID("YVV4HY0dc4OS0zPOc1HPLfyLmx39");
    p.setName("prop");
    p.setVisibility(VisibilityKind::PRIVATE);
    o.setID("P73WqZXNrYSV0fEtHqKAJTv3RDTD");
    o.setName("op");
    o.setVisibility(VisibilityKind::PROTECTED);
    b.getOwnedAttributes().add(p);
    b.getOwnedOperations().add(o);
    string expectedEmit = R""""(opaqueBehavior:
  id: 0FTAvf5nrXsknnc60ziElK2TDb7D
  name: Opaque
  ownedAttributes:
    - property:
        id: YVV4HY0dc4OS0zPOc1HPLfyLmx39
        name: prop
        visibility: PRIVATE
  ownedOperations:
    - operation:
        id: P73WqZXNrYSV0fEtHqKAJTv3RDTD
        name: op
        visibility: PROTECTED)"""";
    string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(b));
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}