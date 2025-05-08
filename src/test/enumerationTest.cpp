#include "gtest/gtest.h"
#include "uml/uml-stable.h"
#include "test/umlTestUtil.h"

using namespace UML;

UML_SET_INTEGRATION_TEST(EnumerationOwnedLiteral, EnumerationLiteral, Enumeration, getOwnedLiterals)
UML_SINGLETON_INTEGRATION_TEST(EnumerationLiteralEnumeration, Enumeration, EnumerationLiteral, getEnumerationSingleton)

class EnumerationTest : public ::testing::Test {
    public:
        std::string ymlPath;
    void SetUp() override {
        ymlPath = "../examples/";
    };
};

TEST_F(EnumerationTest, addOwnedLiteralTest) {
    UmlManager m;
    auto& e = *m.create<Enumeration>();
    auto& l = *m.create<EnumerationLiteral>();
    ASSERT_NO_THROW(e.getOwnedLiterals().add(l));
    ASSERT_TRUE(e.getOwnedLiterals().size() == 1);
    ASSERT_TRUE(e.getOwnedLiterals().front() == &l);
    ASSERT_TRUE(e.getOwnedMembers().size() == 1);
    ASSERT_TRUE(e.getOwnedMembers().front() == &l);
    ASSERT_TRUE(e.getMembers().size() == 1);
    ASSERT_TRUE(e.getMembers().front() == &l);
    ASSERT_TRUE(e.getOwnedElements().size() == 1);
    ASSERT_TRUE(*e.getOwnedElements().begin() == l);

    ASSERT_EQ(*l.getEnumeration(), e);
    ASSERT_EQ(*l.getNamespace(), e);
    ASSERT_EQ(*l.getOwner(), e);
}

TEST_F(EnumerationTest, setEnumerationTest) {
    UmlManager m;
    auto& e = *m.create<Enumeration>();
    auto& l = *m.create<EnumerationLiteral>();
    ASSERT_NO_THROW(l.setEnumeration(&e));
    ASSERT_TRUE(e.getOwnedLiterals().size() == 1);
    ASSERT_TRUE(e.getOwnedLiterals().front() == &l);
    ASSERT_TRUE(e.getOwnedMembers().size() == 1);
    ASSERT_TRUE(e.getOwnedMembers().front() == &l);
    ASSERT_TRUE(e.getMembers().size() == 1);
    ASSERT_TRUE(e.getMembers().front() == &l);
    ASSERT_TRUE(e.getOwnedElements().size() == 1);
    ASSERT_TRUE(*e.getOwnedElements().begin() == l);

    ASSERT_EQ(*l.getEnumeration(), e);
    ASSERT_EQ(*l.getNamespace(), e);
    ASSERT_EQ(*l.getOwner(), e);
}

TEST_F(EnumerationTest, removeOwnedLiteralTest) {
    UmlManager m;
    auto& e = *m.create<Enumeration>();
    auto& l = *m.create<EnumerationLiteral>();
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
    auto& e = *m.create<Enumeration>();
    auto& l = *m.create<EnumerationLiteral>();
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
    UmlManager m;
    ASSERT_NO_THROW(m.open(ymlPath + "enumerationTests/basicEnumeration.yml"));
    ASSERT_TRUE(m.getRoot()->getElementType() == UmlManager::ElementType<Enumeration>::result);
    auto* e = &m.getRoot()->as<Enumeration>();
    ASSERT_TRUE(e->getOwnedLiterals().size() == 2);
    auto l1 = e->getOwnedLiterals().front();
    auto l2 = e->getOwnedLiterals().back();
    ASSERT_TRUE(l1->getName().compare("one") == 0);
    ASSERT_TRUE(l2->getName().compare("two") == 0);
    ASSERT_TRUE(e->getOwnedMembers().contains(l1));
    ASSERT_TRUE(e->getOwnedMembers().contains(l2));
    ASSERT_TRUE(e->getOwnedMembers().size() == 2);
    ASSERT_TRUE(e->getMembers().size() == 2);
    ASSERT_TRUE(e->getMembers().contains(l1));
    ASSERT_TRUE(e->getMembers().contains(l2));
    ASSERT_TRUE(e->getOwnedElements().size() == 2);
    ASSERT_TRUE(e->getOwnedElements().contains(l1));
    ASSERT_TRUE(e->getOwnedElements().contains(l2));

    ASSERT_TRUE(l1->getEnumeration() == e);
    ASSERT_TRUE(l1->getNamespace() == e);
    ASSERT_TRUE(l1->getOwner() == e);

    ASSERT_TRUE(l2->getEnumeration() == e);
    ASSERT_TRUE(l2->getNamespace() == e);
    ASSERT_TRUE(l2->getOwner() == e);
}

TEST_F(EnumerationTest, emitEnumerationWLiterals) {
    UmlManager m;
    auto& e = *m.create<Enumeration>();
    auto& l1 = *m.create<EnumerationLiteral>();
    auto& l2 = *m.create<EnumerationLiteral>();
    e.setID(EGM::ID::fromString("a6ds7Q7pgI80WPT5vd2LbJn4dN2g"));
    e.setName("enum");
    l1.setID(EGM::ID::fromString("ki59FSBz84bR3hyOjfeHEg9LLtCA"));
    l1.setName("one");
    l2.setID(EGM::ID::fromString("IFMeIYNqJzfzBIOMdbuxl-rBBLwR"));
    l2.setName("two");
    e.getOwnedLiterals().add(l1);
    e.getOwnedLiterals().add(l2);
    std::string expectedEmit = R""""(Enumeration:
  id: a6ds7Q7pgI80WPT5vd2LbJn4dN2g
  name: enum
  ownedLiterals:
    - EnumerationLiteral:
        id: ki59FSBz84bR3hyOjfeHEg9LLtCA
        name: one
    - EnumerationLiteral:
        id: IFMeIYNqJzfzBIOMdbuxl-rBBLwR
        name: two)"""";
    std::string generatedEmit;
    ASSERT_NO_THROW(generatedEmit = m.dump(e));
    std::cout << generatedEmit << '\n';
    ASSERT_EQ(expectedEmit, generatedEmit);
}

TEST_F(EnumerationTest, mountEnumerationTest) {
    UmlManager m;
    auto& enumeration = *m.create<Enumeration>();
    auto& enumerationLiteral = *m.create<EnumerationLiteral>();
    enumeration.getOwnedLiterals().add(enumerationLiteral);
    m.setRoot(&enumeration);
    m.mount(ymlPath + "enumerationTests");
    
    EGM::ID enumerationID = enumeration.getID();
    EGM::ID enumerationLiteralID = enumerationLiteral.getID();
    m.release(enumeration);
    ASSERT_FALSE(m.loaded(enumerationID));
    auto& enumeration2 = m.get(enumerationID)->as<Enumeration>();
    ASSERT_EQ(enumeration2.getOwnedLiterals().size(), 1);
    ASSERT_EQ(enumeration2.getOwnedLiterals().front(), &enumerationLiteral);
    ASSERT_TRUE(enumerationLiteral.getEnumeration());
    ASSERT_EQ(*enumerationLiteral.getEnumeration(), enumeration2);
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_NAMESPACE(enumerationLiteral, enumeration2));

    m.release(enumerationLiteral, enumeration2);
    ASSERT_FALSE(m.loaded(enumerationID));
    ASSERT_FALSE(m.loaded(enumerationLiteralID));
    auto& enumeration3 = m.get(enumerationID)->as<Enumeration>();
    // ASSERT_FALSE(m.loaded(enumerationLiteralID));
    ASSERT_EQ(enumeration3.getOwnedLiterals().size(), 1);
    ASSERT_EQ(enumeration3.getOwnedMembers().size(), 1);
    auto& enumerationLiteral2 = m.get(enumerationLiteralID)->as<EnumerationLiteral>();
    ASSERT_EQ(enumeration3.getOwnedLiterals().front(), &enumerationLiteral2);
    ASSERT_TRUE(enumerationLiteral2.getEnumeration());
    ASSERT_EQ(*enumerationLiteral2.getEnumeration(), enumeration3);
    ASSERT_NO_FATAL_FAILURE(ASSERT_RESTORED_NAMESPACE(enumerationLiteral2, enumeration3));

    m.release(enumerationLiteral2, enumeration3);
    ASSERT_FALSE(m.loaded(enumerationID));
    ASSERT_FALSE(m.loaded(enumerationLiteralID));
}
