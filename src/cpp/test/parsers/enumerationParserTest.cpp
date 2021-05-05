#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/enumeration.h"

using namespace std;
using namespace UML;

class EnumerationParserTest : public ::testing::Test {
    public:
        string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(EnumerationParserTest, basicEnumerationTest) {
    Element* el;
    ASSERT_NO_THROW(el = Parsers::parse(YAML::LoadFile(ymlPath + "enumerationTests/basicEnumeration.yml")));
    ASSERT_TRUE(el->getElementType() == ElementType::ENUMERATION);
    Enumeration* e = dynamic_cast<Enumeration*>(el);
    ASSERT_TRUE(e->getOwnedLiteral().size() == 2);
    EnumerationLiteral* l1 = e->getOwnedLiteral().front();
    EnumerationLiteral* l2 = e->getOwnedLiteral().back();
    ASSERT_TRUE(l1->getName().compare("one") == 0);
    ASSERT_TRUE(l2->getName().compare("two") == 0);
    ASSERT_TRUE(e->getOwnedMembers().size() == 2);
    ASSERT_TRUE(e->getOwnedMembers().front() == l1);
    ASSERT_TRUE(e->getOwnedMembers().back() == l2);
    ASSERT_TRUE(e->getMembers().size() == 2);
    ASSERT_TRUE(e->getMembers().front() == l1);
    ASSERT_TRUE(e->getMembers().back() == l2);
    ASSERT_TRUE(e->getOwnedElements().size() == 2);
    ASSERT_TRUE(e->getOwnedElements().front() == l1);
    ASSERT_TRUE(e->getOwnedElements().back() == l2);

    ASSERT_TRUE(l1->getEnumeration() == e);
    ASSERT_TRUE(l1->getNamespace() == e);
    ASSERT_TRUE(l1->getMemberNamespace().size() == 1);
    ASSERT_TRUE(l1->getMemberNamespace().front() == e);
    ASSERT_TRUE(l1->getOwner() == e);

    ASSERT_TRUE(l2->getEnumeration() == e);
    ASSERT_TRUE(l2->getNamespace() == e);
    ASSERT_TRUE(l2->getMemberNamespace().size() == 1);
    ASSERT_TRUE(l2->getMemberNamespace().front() == e);
    ASSERT_TRUE(l2->getOwner() == e);
}