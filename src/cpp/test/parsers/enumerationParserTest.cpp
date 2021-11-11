#include "gtest/gtest.h"
#include "uml/parsers/parser.h"
#include "test/yumlParsersTest.h"
#include "uml/enumeration.h"
#include "uml/enumerationLiteral.h"
#include "test/umlTestUtil.h"

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
    ASSERT_TRUE(e->getOwnedLiterals().size() == 2);
    EnumerationLiteral* l1 = &e->getOwnedLiterals().front();
    EnumerationLiteral* l2 = &e->getOwnedLiterals().back();
    ASSERT_TRUE(l1->getName().compare("one") == 0);
    ASSERT_TRUE(l2->getName().compare("two") == 0);
    ASSERT_TRUE(e->getOwnedMembers().size() == 2);
    ASSERT_TRUE(&e->getOwnedMembers().front() == l1);
    ASSERT_TRUE(&e->getOwnedMembers().back() == l2);
    ASSERT_TRUE(e->getMembers().size() == 2);
    ASSERT_TRUE(&e->getMembers().front() == l1);
    ASSERT_TRUE(&e->getMembers().back() == l2);
    ASSERT_TRUE(e->getOwnedElements().size() == 2);
    ASSERT_EQ(*e->getOwnedElements().begin(), *l1);
    ASSERT_EQ(*(e->getOwnedElements().begin()++), *l2);

    ASSERT_TRUE(l1->getEnumeration() == e);
    ASSERT_TRUE(l1->getNamespace() == e);
    ASSERT_TRUE(l1->getMemberNamespace().size() == 1);
    ASSERT_TRUE(&l1->getMemberNamespace().front() == e);
    ASSERT_TRUE(l1->getOwner() == e);

    ASSERT_TRUE(l2->getEnumeration() == e);
    ASSERT_TRUE(l2->getNamespace() == e);
    ASSERT_TRUE(l2->getMemberNamespace().size() == 1);
    ASSERT_TRUE(&l2->getMemberNamespace().front() == e);
    ASSERT_TRUE(l2->getOwner() == e);
}

TEST_F(EnumerationParserTest, emitEnumerationWLiterals) {
    UmlManager m;
    Enumeration& e = m.create<Enumeration>();
    EnumerationLiteral& l1 = m.create<EnumerationLiteral>();
    EnumerationLiteral& l2 = m.create<EnumerationLiteral>();
    e.setID("a6ds7Q7pgI80WPT5vd2LbJn4dN2g");
    e.setName("enum");
    l1.setID("ki59FSBz84bR3hyOjfeHEg9LLtCA");
    l1.setName("one");
    l2.setID("IFMeIYNqJzfzBIOMdbuxl&rBBLwR");
    l2.setName("two");
    e.getOwnedLiterals().add(l1);
    e.getOwnedLiterals().add(l2);
    string expectedEmit = R""""(enumeration:
  id: a6ds7Q7pgI80WPT5vd2LbJn4dN2g
  name: enum
  ownedLiteral:
    - enumerationLiteral:
        id: ki59FSBz84bR3hyOjfeHEg9LLtCA
        name: one
    - enumerationLiteral:
        id: IFMeIYNqJzfzBIOMdbuxl&rBBLwR
        name: two)"""";
    string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(e));
    cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(EnumerationParserTest, mountEnumerationTest) {
    UmlManager m;
    Enumeration& enumeration = m.create<Enumeration>();
    EnumerationLiteral& enumerationLiteral = m.create<EnumerationLiteral>();
    enumeration.getOwnedLiterals().add(enumerationLiteral);
    m.setRoot(&enumeration);
    m.mount(ymlPath + "enumerationTests");
    
    ID enumerationID = enumeration.getID();
    ID enumerationLiteralID = enumerationLiteral.getID();
    m.release(enumeration);
    ASSERT_FALSE(m.loaded(enumerationID));
    Enumeration& enumeration2 = m.aquire(enumerationID)->as<Enumeration>();
    ASSERT_EQ(enumeration2.getOwnedLiterals().size(), 1);
    ASSERT_EQ(enumeration2.getOwnedLiterals().front(), enumerationLiteral);
    ASSERT_TRUE(enumerationLiteral.hasEnumeration());
    ASSERT_EQ(enumerationLiteral.getEnumerationRef(), enumeration2);
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_NAMESPACE(enumerationLiteral, enumeration2));

    m.release(enumerationLiteral, enumeration2);
    ASSERT_FALSE(m.loaded(enumerationID));
    ASSERT_FALSE(m.loaded(enumerationLiteralID));
    Enumeration& enumeration3 = m.aquire(enumerationID)->as<Enumeration>();
    ASSERT_FALSE(m.loaded(enumerationLiteralID));
    ASSERT_EQ(enumeration3.getOwnedLiterals().size(), 1);
    ASSERT_EQ(enumeration3.getOwnedMembers().size(), 1);
    EnumerationLiteral& enumerationLiteral2 = m.aquire(enumerationLiteralID)->as<EnumerationLiteral>();
    ASSERT_EQ(enumeration3.getOwnedLiterals().front(), enumerationLiteral2);
    ASSERT_TRUE(enumerationLiteral2.hasEnumeration());
    ASSERT_EQ(enumerationLiteral2.getEnumerationRef(), enumeration3);
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_NAMESPACE(enumerationLiteral2, enumeration3));

    m.release(enumerationLiteral2, enumeration3);
    ASSERT_FALSE(m.loaded(enumerationID));
    ASSERT_FALSE(m.loaded(enumerationLiteralID));
}