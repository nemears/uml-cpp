#include "gtest/gtest.h"
#include "uml/enumeration.h"
#include "uml/package.h"
#include "uml/enumerationLiteral.h"

using namespace UML;

class EnumerationTest : public ::testing::Test {};

TEST_F(EnumerationTest, addOwnedLiteralTest) {
    UmlManager m;
    Enumeration& e = m.create<Enumeration>();
    EnumerationLiteral& l = m.create<EnumerationLiteral>();
    ASSERT_NO_THROW(e.getOwnedLiterals().add(l));
    ASSERT_TRUE(e.getOwnedLiterals().size() == 1);
    ASSERT_TRUE(&e.getOwnedLiterals().front() == &l);
    ASSERT_TRUE(e.getOwnedMembers().size() == 1);
    ASSERT_TRUE(&e.getOwnedMembers().front() == &l);
    ASSERT_TRUE(e.getMembers().size() == 1);
    ASSERT_TRUE(&e.getMembers().front() == &l);
    ASSERT_TRUE(e.getOwnedElements().size() == 1);
    ASSERT_TRUE(*e.getOwnedElements().begin() == l);

    ASSERT_TRUE(l.getEnumeration() == &e);
    ASSERT_TRUE(l.getNamespace() == &e);
    ASSERT_TRUE(l.getMemberNamespace().size() == 1);
    ASSERT_TRUE(&l.getMemberNamespace().front() == &e);
    ASSERT_TRUE(l.getOwner() == &e);
}

TEST_F(EnumerationTest, setEnumerationTest) {
    UmlManager m;
    Enumeration& e = m.create<Enumeration>();
    EnumerationLiteral& l = m.create<EnumerationLiteral>();
    ASSERT_NO_THROW(l.setEnumeration(&e));
    ASSERT_TRUE(e.getOwnedLiterals().size() == 1);
    ASSERT_TRUE(&e.getOwnedLiterals().front() == &l);
    ASSERT_TRUE(e.getOwnedMembers().size() == 1);
    ASSERT_TRUE(&e.getOwnedMembers().front() == &l);
    ASSERT_TRUE(e.getMembers().size() == 1);
    ASSERT_TRUE(&e.getMembers().front() == &l);
    ASSERT_TRUE(e.getOwnedElements().size() == 1);
    ASSERT_TRUE(*e.getOwnedElements().begin() == l);

    ASSERT_TRUE(l.getEnumeration() == &e);
    ASSERT_TRUE(l.getNamespace() == &e);
    ASSERT_TRUE(l.getMemberNamespace().size() == 1);
    ASSERT_TRUE(&l.getMemberNamespace().front() == &e);
    ASSERT_TRUE(l.getOwner() == &e);
}

TEST_F(EnumerationTest, removeOwnedLiteralTest) {
    UmlManager m;
    Enumeration& e = m.create<Enumeration>();
    EnumerationLiteral& l = m.create<EnumerationLiteral>();
    e.getOwnedLiterals().add(l);
    ASSERT_NO_THROW(e.getOwnedLiterals().remove(l));
    ASSERT_TRUE(e.getOwnedLiterals().size() == 0);
    ASSERT_TRUE(e.getOwnedMembers().size() == 0);
    ASSERT_TRUE(e.getMembers().size() == 0);
    ASSERT_TRUE(e.getOwnedElements().size() == 0);
    
    ASSERT_TRUE(l.getEnumeration() == 0);
    ASSERT_TRUE(l.getNamespace() == 0);
    ASSERT_TRUE(l.getMemberNamespace().size() == 0);
    ASSERT_TRUE(l.getOwner() == 0);
}

TEST_F(EnumerationTest, setNullEnumeration) {
    UmlManager m;
    Enumeration& e = m.create<Enumeration>();
    EnumerationLiteral& l = m.create<EnumerationLiteral>();
    e.getOwnedLiterals().add(l);
    ASSERT_NO_THROW(l.setEnumeration(0));
    ASSERT_TRUE(e.getOwnedLiterals().size() == 0);
    ASSERT_TRUE(e.getOwnedMembers().size() == 0);
    ASSERT_TRUE(e.getMembers().size() == 0);
    ASSERT_TRUE(e.getOwnedElements().size() == 0);
    
    ASSERT_TRUE(l.getEnumeration() == 0);
    ASSERT_TRUE(l.getNamespace() == 0);
    ASSERT_TRUE(l.getMemberNamespace().size() == 0);
    ASSERT_TRUE(l.getOwner() == 0);
}

TEST_F(EnumerationTest, copyEnumerationTest) {
    UmlManager m;
    Enumeration& e = m.create<Enumeration>();
    EnumerationLiteral& l = m.create<EnumerationLiteral>();
    Package& p = m.create<Package>();
    e.setName("test");
    p.getPackagedElements().add(e);
    e.getOwnedLiterals().add(l);
    Enumeration e2 = e;
    ASSERT_TRUE(e2.getName().compare("test") == 0);
    ASSERT_TRUE(e2.getOwningPackage() == &p);
    ASSERT_TRUE(e2.getNamespace() == &p);
    ASSERT_TRUE(e2.getMemberNamespace().size() == 1);
    ASSERT_TRUE(&e2.getMemberNamespace().front() == &p);
    ASSERT_TRUE(e2.getOwner() == &p);
    ASSERT_TRUE(e2.getOwnedLiterals().size() == 1);
    ASSERT_TRUE(&e2.getOwnedLiterals().front() == &l);
    ASSERT_TRUE(e2.getOwnedMembers().size() == 1);
    ASSERT_TRUE(&e2.getOwnedMembers().front() == &l);
    ASSERT_TRUE(e2.getMembers().size() == 1);
    ASSERT_TRUE(&e2.getMembers().front() == &l);
    ASSERT_TRUE(e2.getOwnedElements().size() == 1);
    ASSERT_TRUE(*e2.getOwnedElements().begin() == l);

}