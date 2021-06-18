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
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "enumerationTests/basicEnumeration.yml"));
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

TEST_F(EnumerationParserTest, emitEnumerationWLiterals) {
    Enumeration e;
    EnumerationLiteral l1;
    EnumerationLiteral l2;
    e.setID("b278ca8a-9d1d-45d1-970f-403bc60998b3");
    e.setName("enum");
    l1.setID("9c744c8c-ed4e-4c71-9c97-5d3e6115bc24");
    l1.setName("one");
    l2.setID("d2a0bcbd-a1aa-4953-9d95-b10a9a322fe3");
    l2.setName("two");
    e.getOwnedLiteral().add(l1);
    e.getOwnedLiteral().add(l2);
    string expectedEmit = R""""(enumeration:
  id: b278ca8a-9d1d-45d1-970f-403bc60998b3
  name: enum
  ownedLiteral:
    - enumerationLiteral:
        id: 9c744c8c-ed4e-4c71-9c97-5d3e6115bc24
        name: one
    - enumerationLiteral:
        id: d2a0bcbd-a1aa-4953-9d95-b10a9a322fe3
        name: two)"""";
    string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(e));
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}