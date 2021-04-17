#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/opaqueBehavior.h"

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
    ASSERT_NO_THROW(el = Parsers::parse(YAML::LoadFile(ymlPath + "opaqueBehaviorTests/multipleSimpleBodies.yml")));
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
    ASSERT_NO_THROW(el = Parsers::parse(YAML::LoadFile(ymlPath + "opaqueBehaviorTests/param.yml")));
    ASSERT_TRUE(el->getElementType() == ElementType::OPAQUE_BEHAVIOR);
    OpaqueBehavior* bhv = dynamic_cast<OpaqueBehavior*>(el);
    ASSERT_TRUE(bhv->getParameters().size() == 1);
    Parameter* param = bhv->getParameters().front();
    ASSERT_TRUE(param->getName().compare("test") == 0);
    ASSERT_TRUE(param->getDirection() == ParameterDirectionKind::IN);
}