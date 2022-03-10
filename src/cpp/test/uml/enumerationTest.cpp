#include "gtest/gtest.h"
#include "uml/enumeration.h"
#include "uml/package.h"
#include "uml/enumerationLiteral.h"
#include "uml/uml-stable.h"

using namespace UML;

class EnumerationTest : public ::testing::Test {};

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