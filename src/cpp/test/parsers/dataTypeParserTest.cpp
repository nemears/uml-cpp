#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/dataType.h"

using namespace std;
using namespace UML;

class DataTypeParserTest : public ::testing::Test {
    public:
        string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(DataTypeParserTest, basicDataTypeTest) {
    Element* el;
    ASSERT_NO_THROW(el = Parsers::parse(ymlPath + "dataTypeTests/basicDataType.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::DATA_TYPE);
    DataType d = *dynamic_cast<DataType*>(el);
    ASSERT_TRUE(d.getName().compare("int") == 0);
    ASSERT_TRUE(d.getOwnedAttribute().size() == 1);
    Property* p = d.getOwnedAttribute().front();
    ASSERT_TRUE(d.getAttributes().size() == 1);
    ASSERT_TRUE(d.getAttributes().front() == p);
    ASSERT_TRUE(d.getOwnedOperation().size() == 1);
    Operation* o = d.getOwnedOperation().front();
    ASSERT_TRUE(d.getFeatures().size() == 2);
    ASSERT_TRUE(d.getFeatures().front() == p);
    ASSERT_TRUE(d.getFeatures().back() == o);
    ASSERT_TRUE(d.getOwnedMembers().size() == 2);
    ASSERT_TRUE(d.getOwnedMembers().front() == p);
    ASSERT_TRUE(d.getOwnedMembers().back() == o);
    ASSERT_TRUE(d.getMembers().size() == 2);
    ASSERT_TRUE(d.getMembers().front() == p);
    ASSERT_TRUE(d.getMembers().back() == o);
    ASSERT_TRUE(d.getOwnedElements().size() == 2);
    ASSERT_TRUE(d.getOwnedElements().front() == p);
    ASSERT_TRUE(d.getOwnedElements().back() == o);
}