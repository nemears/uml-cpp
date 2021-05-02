#include "gtest/gtest.h"
#include "uml/enumeration.h"
#include "uml/package.h"

using namespace UML;

class EnumerationTest : public ::testing::Test {};

TEST_F(EnumerationTest, addOwnedLiteralTest) {
    Enumeration e;
    EnumerationLiteral l;
    ASSERT_NO_THROW(e.getOwnedLiteral().add(l));
    ASSERT_TRUE(e.getOwnedLiteral().size() == 1);
    ASSERT_TRUE(e.getOwnedLiteral().front() == &l);
    ASSERT_TRUE(e.getOwnedMembers().size() == 1);
    ASSERT_TRUE(e.getOwnedMembers().front() == &l);
    ASSERT_TRUE(e.getMembers().size() == 1);
    ASSERT_TRUE(e.getMembers().front() == &l);
    ASSERT_TRUE(e.getOwnedElements().size() == 1);
    ASSERT_TRUE(e.getOwnedElements().front() == &l);

    ASSERT_TRUE(l.getEnumeration() == &e);
    ASSERT_TRUE(l.getNamespace() == &e);
    ASSERT_TRUE(l.getMemberNamespace().size() == 1);
    ASSERT_TRUE(l.getMemberNamespace().front() == &e);
    ASSERT_TRUE(l.getOwner() == &e);
}

TEST_F(EnumerationTest, setEnumerationTest) {
    Enumeration e;
    EnumerationLiteral l;
    ASSERT_NO_THROW(l.setEnumeration(&e));
    ASSERT_TRUE(e.getOwnedLiteral().size() == 1);
    ASSERT_TRUE(e.getOwnedLiteral().front() == &l);
    ASSERT_TRUE(e.getOwnedMembers().size() == 1);
    ASSERT_TRUE(e.getOwnedMembers().front() == &l);
    ASSERT_TRUE(e.getMembers().size() == 1);
    ASSERT_TRUE(e.getMembers().front() == &l);
    ASSERT_TRUE(e.getOwnedElements().size() == 1);
    ASSERT_TRUE(e.getOwnedElements().front() == &l);

    ASSERT_TRUE(l.getEnumeration() == &e);
    ASSERT_TRUE(l.getNamespace() == &e);
    ASSERT_TRUE(l.getMemberNamespace().size() == 1);
    ASSERT_TRUE(l.getMemberNamespace().front() == &e);
    ASSERT_TRUE(l.getOwner() == &e);
}

TEST_F(EnumerationTest, removeOwnedLiteralTest) {
    Enumeration e;
    EnumerationLiteral l;
    e.getOwnedLiteral().add(l);
    ASSERT_NO_THROW(e.getOwnedLiteral().remove(l));
    ASSERT_TRUE(e.getOwnedLiteral().size() == 0);
    ASSERT_TRUE(e.getOwnedMembers().size() == 0);
    ASSERT_TRUE(e.getMembers().size() == 0);
    ASSERT_TRUE(e.getOwnedElements().size() == 0);
    
    ASSERT_TRUE(l.getEnumeration() == 0);
    ASSERT_TRUE(l.getNamespace() == 0);
    ASSERT_TRUE(l.getMemberNamespace().size() == 0);
    ASSERT_TRUE(l.getOwner() == 0);
}

TEST_F(EnumerationTest, setNullEnumeration) {
    Enumeration e;
    EnumerationLiteral l;
    e.getOwnedLiteral().add(l);
    ASSERT_NO_THROW(l.setEnumeration(0));
    ASSERT_TRUE(e.getOwnedLiteral().size() == 0);
    ASSERT_TRUE(e.getOwnedMembers().size() == 0);
    ASSERT_TRUE(e.getMembers().size() == 0);
    ASSERT_TRUE(e.getOwnedElements().size() == 0);
    
    ASSERT_TRUE(l.getEnumeration() == 0);
    ASSERT_TRUE(l.getNamespace() == 0);
    ASSERT_TRUE(l.getMemberNamespace().size() == 0);
    ASSERT_TRUE(l.getOwner() == 0);
}

TEST_F(EnumerationTest, copyEnumerationTest) {
    Enumeration e;
    EnumerationLiteral l;
    Package p;
    e.setName("test");
    p.getPackagedElements().add(e);
    e.getOwnedLiteral().add(l);
    Enumeration e2 = e;
    ASSERT_TRUE(e2.getName().compare("test") == 0);
    ASSERT_TRUE(e2.getOwningPackage() == &p);
    ASSERT_TRUE(e2.getNamespace() == &p);
    ASSERT_TRUE(e2.getMemberNamespace().size() == 1);
    ASSERT_TRUE(e2.getMemberNamespace().front() == &p);
    ASSERT_TRUE(e2.getOwner() == &p);
    ASSERT_TRUE(e2.getOwnedLiteral().size() == 1);
    ASSERT_TRUE(e2.getOwnedLiteral().front() == &l);
    ASSERT_TRUE(e2.getOwnedMembers().size() == 1);
    ASSERT_TRUE(e2.getOwnedMembers().front() == &l);
    ASSERT_TRUE(e2.getMembers().size() == 1);
    ASSERT_TRUE(e2.getMembers().front() == &l);
    ASSERT_TRUE(e2.getOwnedElements().size() == 1);
    ASSERT_TRUE(e2.getOwnedElements().front() == &l);

}