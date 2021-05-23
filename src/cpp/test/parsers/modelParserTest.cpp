#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/model.h"

using namespace std;
using namespace UML;

class ModelParserTest : public ::testing::Test {
    public:
        string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(ModelParserTest, parsedAndCreatedElementTest) {
    Element* el;
    ASSERT_NO_THROW(el = Parsers::parse(ymlPath + "modelTests/modelW_Packages.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::MODEL);
    Parsers::deleteParsedElement(el);
}