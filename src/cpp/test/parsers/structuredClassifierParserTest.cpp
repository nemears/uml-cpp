#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/class.h"
#include "uml/property.h"

using namespace std;
using namespace UML;

class StructuredClassifierParserTest : public ::testing::Test {
    public:
        string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(StructuredClassifierParserTest, parseOwnedAttributeTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "structuredClassifierTests/ownedAttributeTest.yml"));
    ASSERT_TRUE(el->getElementType() == ElementType::CLASS);
    Class c = *dynamic_cast<Class*>(el);
    ASSERT_TRUE(c.getOwnedAttributes().size() == 1);
    Property* p = &c.getOwnedAttributes().front();
    ASSERT_TRUE(c.getAttributes().size() == 1);
    ASSERT_TRUE(&c.getAttributes().front() == p);
    ASSERT_TRUE(c.getRole().size() == 1);
    ASSERT_TRUE(&c.getRole().front() == p);
    ASSERT_TRUE(c.getFeatures().size() == 1);
    ASSERT_TRUE(&c.getFeatures().front() == p);
    ASSERT_TRUE(c.getOwnedMembers().size() == 1);
    ASSERT_TRUE(&c.getOwnedMembers().front() == p);
    ASSERT_TRUE(c.getMembers().size() == 1);
    ASSERT_TRUE(&c.getMembers().front() == p);

    ASSERT_TRUE(p->getName().compare("test") == 0);
}

TEST_F(StructuredClassifierParserTest, partTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "structuredClassifierTests/partTest.yml"));
    Class c = *dynamic_cast<Class*>(el);
    ASSERT_TRUE(c.getParts().size() == 1);
    ASSERT_TRUE(c.getOwnedAttributes().size() == 1);
    Property* p = &c.getOwnedAttributes().front();
    ASSERT_TRUE(&c.getParts().front() == p);
    ASSERT_TRUE(c.getAttributes().size() == 1);
    ASSERT_TRUE(&c.getAttributes().front() == p);
    ASSERT_TRUE(c.getRole().size() == 1);
    ASSERT_TRUE(&c.getRole().front() == p);
    ASSERT_TRUE(c.getFeatures().size() == 1);
    ASSERT_TRUE(&c.getFeatures().front() == p);
    ASSERT_TRUE(c.getOwnedMembers().size() == 1);
    ASSERT_TRUE(&c.getOwnedMembers().front() == p);
    ASSERT_TRUE(c.getMembers().size() == 1);
    ASSERT_TRUE(&c.getMembers().front() == p);

    ASSERT_TRUE(p->getAggregation() == AggregationKind::COMPOSITE);
}