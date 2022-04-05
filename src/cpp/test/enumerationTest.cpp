#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"
#include "test/uml-cpp-paths.h"

using namespace UML;

UML_SET_INTEGRATION_TEST(EnumerationOwnedLiteral, EnumerationLiteral, Enumeration, &Enumeration::getOwnedLiterals);
UML_SINGLETON_INTEGRATION_TEST(EnumerationLiteralEnumeration, Enumeration, EnumerationLiteral, &EnumerationLiteral::getEnumeration, &EnumerationLiteral::setEnumeration);

class EnumerationTest : public ::testing::Test {
    public:
        std::string ymlPath;
    void SetUp() override {
        ymlPath = YML_FILES_PATH;
    };
};

TEST_F(EnumerationTest, addOwnedLiteralTest) {
    UmlManager m;
    Enumeration& e = *m.create<Enumeration>();
    EnumerationLiteral& l = *m.create<EnumerationLiteral>();
    ASSERT_NO_THROW(e.getOwnedLiterals().add(l));
    ASSERT_TRUE(e.getOwnedLiterals().size() == 1);
    ASSERT_TRUE(&e.getOwnedLiterals().front() == &l);
    ASSERT_TRUE(e.getOwnedMembers().size() == 1);
    ASSERT_TRUE(&e.getOwnedMembers().front() == &l);
    ASSERT_TRUE(e.getMembers().size() == 1);
    ASSERT_TRUE(&e.getMembers().front() == &l);
    ASSERT_TRUE(e.getOwnedElements().size() == 1);
    ASSERT_TRUE(*e.getOwnedElements().begin() == l);

    ASSERT_EQ(*l.getEnumeration(), e);
    ASSERT_EQ(*l.getNamespace(), e);
    ASSERT_EQ(*l.getOwner(), e);
}

TEST_F(EnumerationTest, setEnumerationTest) {
    UmlManager m;
    Enumeration& e = *m.create<Enumeration>();
    EnumerationLiteral& l = *m.create<EnumerationLiteral>();
    ASSERT_NO_THROW(l.setEnumeration(&e));
    ASSERT_TRUE(e.getOwnedLiterals().size() == 1);
    ASSERT_TRUE(&e.getOwnedLiterals().front() == &l);
    ASSERT_TRUE(e.getOwnedMembers().size() == 1);
    ASSERT_TRUE(&e.getOwnedMembers().front() == &l);
    ASSERT_TRUE(e.getMembers().size() == 1);
    ASSERT_TRUE(&e.getMembers().front() == &l);
    ASSERT_TRUE(e.getOwnedElements().size() == 1);
    ASSERT_TRUE(*e.getOwnedElements().begin() == l);

    ASSERT_EQ(*l.getEnumeration(), e);
    ASSERT_EQ(*l.getNamespace(), e);
    ASSERT_EQ(*l.getOwner(), e);
}

TEST_F(EnumerationTest, removeOwnedLiteralTest) {
    UmlManager m;
    Enumeration& e = *m.create<Enumeration>();
    EnumerationLiteral& l = *m.create<EnumerationLiteral>();
    e.getOwnedLiterals().add(l);
    ASSERT_NO_THROW(e.getOwnedLiterals().remove(l));
    ASSERT_TRUE(e.getOwnedLiterals().size() == 0);
    ASSERT_TRUE(e.getOwnedMembers().size() == 0);
    ASSERT_TRUE(e.getMembers().size() == 0);
    ASSERT_TRUE(e.getOwnedElements().size() == 0);
    
    ASSERT_FALSE(l.getEnumeration());
    ASSERT_FALSE(l.getNamespace());
    ASSERT_FALSE(l.getOwner());
}

TEST_F(EnumerationTest, setNullEnumeration) {
    UmlManager m;
    Enumeration& e = *m.create<Enumeration>();
    EnumerationLiteral& l = *m.create<EnumerationLiteral>();
    e.getOwnedLiterals().add(l);
    ASSERT_NO_THROW(l.setEnumeration(0));
    ASSERT_TRUE(e.getOwnedLiterals().size() == 0);
    ASSERT_TRUE(e.getOwnedMembers().size() == 0);
    ASSERT_TRUE(e.getMembers().size() == 0);
    ASSERT_TRUE(e.getOwnedElements().size() == 0);
    
    ASSERT_FALSE(l.getEnumeration());
    ASSERT_FALSE(l.getNamespace());
    ASSERT_FALSE(l.getOwner());
}

TEST_F(EnumerationTest, basicEnumerationTest) {
    Element* el;
    UmlManager m;
    ASSERT_NO_THROW(el = m.parse(ymlPath + "enumerationTests/basicEnumeration.yml").ptr());
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
    ASSERT_TRUE(l1->getOwner() == e);

    ASSERT_TRUE(l2->getEnumeration() == e);
    ASSERT_TRUE(l2->getNamespace() == e);
    ASSERT_TRUE(l2->getOwner() == e);
}

TEST_F(EnumerationTest, emitEnumerationWLiterals) {
    UmlManager m;
    Enumeration& e = *m.create<Enumeration>();
    EnumerationLiteral& l1 = *m.create<EnumerationLiteral>();
    EnumerationLiteral& l2 = *m.create<EnumerationLiteral>();
    e.setID("a6ds7Q7pgI80WPT5vd2LbJn4dN2g");
    e.setName("enum");
    l1.setID("ki59FSBz84bR3hyOjfeHEg9LLtCA");
    l1.setName("one");
    l2.setID("IFMeIYNqJzfzBIOMdbuxl&rBBLwR");
    l2.setName("two");
    e.getOwnedLiterals().add(l1);
    e.getOwnedLiterals().add(l2);
    std::string expectedEmit = R""""(enumeration:
  id: a6ds7Q7pgI80WPT5vd2LbJn4dN2g
  name: enum
  ownedLiteral:
    - enumerationLiteral:
        id: ki59FSBz84bR3hyOjfeHEg9LLtCA
        name: one
    - enumerationLiteral:
        id: IFMeIYNqJzfzBIOMdbuxl&rBBLwR
        name: two)"""";
    std::string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = Parsers::emit(e));
    std::cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(EnumerationTest, mountEnumerationTest) {
    UmlManager m;
    Enumeration& enumeration = *m.create<Enumeration>();
    EnumerationLiteral& enumerationLiteral = *m.create<EnumerationLiteral>();
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
    ASSERT_TRUE(enumerationLiteral.getEnumeration());
    ASSERT_EQ(*enumerationLiteral.getEnumeration(), enumeration2);
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
    ASSERT_TRUE(enumerationLiteral2.getEnumeration());
    ASSERT_EQ(*enumerationLiteral2.getEnumeration(), enumeration3);
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_NAMESPACE(enumerationLiteral2, enumeration3));

    m.release(enumerationLiteral2, enumeration3);
    ASSERT_FALSE(m.loaded(enumerationID));
    ASSERT_FALSE(m.loaded(enumerationLiteralID));
}