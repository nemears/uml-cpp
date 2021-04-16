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
}