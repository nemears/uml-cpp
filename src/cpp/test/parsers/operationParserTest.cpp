#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/operation.h"

using namespace std;
using namespace UML;

class OperationParserTest : public ::testing::Test {
    public:
        string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(OperationParserTest, properExceptions) {
    Element* el;
    ASSERT_THROW(el = Parsers::parse(ymlPath + "operationTests/invalidBehavior.yml"), Parsers::UmlParserException);
    ASSERT_THROW(el = Parsers::parse(ymlPath + "operationTests/invalidMethodList.yml"), Parsers::UmlParserException);
}