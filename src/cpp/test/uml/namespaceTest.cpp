#include "gtest/gtest.h"
#include "uml/namespace.h"
#include "uml/package.h"

using namespace UML;

class NamespaceTest : public ::testing::Test {};

// Really this test isn't necessary just did it for posterity and getting used to gtest
TEST_F(NamespaceTest, SetNameTest) {
    UmlManager m;
    Package n = m.create<Package>();
    n.setName("test");
    Package owningPackage = m.create<Package>();
    owningPackage.getPackagedElements().add(n);
    ASSERT_EQ(n.getName(), "test");
    ASSERT_EQ(owningPackage.getPackagedElements().get("test"), n);
}

TEST_F(NamespaceTest, reindexID_Test) {
    UmlManager m;
    Package nmspc = m.create<Package>();
    Package n = m.create<Package>();
    nmspc.setName("nmspc");
    n.setNamespace(&nmspc);
    n.setID("190d1cb9_13dc_44e6_a064_1268");
    ASSERT_EQ(nmspc.getOwnedElements().get(n.getID()), n);
    ASSERT_EQ(nmspc.getMembers().get(n.getID()), n);
    ASSERT_EQ(nmspc.getOwnedMembers().get(n.getID()), n);
}

// TEST_F(NamespaceTest, reindexNameTest) {
//     Namespace nmspc;
//     NamedElement n;
//     nmspc.setName("nmspc");
//     n.setNamespace(&nmspc);
//     ASSERT_NO_THROW(n.setName("test"));
//     ASSERT_TRUE(nmspc.getOwnedElements().get("test") != NULL);
//     ASSERT_TRUE(nmspc.getMembers().get("test") != NULL);
//     ASSERT_TRUE(nmspc.getOwnedMembers().get("test") != 0);
// }

TEST_F(NamespaceTest, AddOwnedMemeberFunctorTest) {
    UmlManager mm;
    Package n = mm.create<Package>();
    Package m = mm.create<Package>();
    n.getOwnedMembers().add(m);
    ASSERT_EQ(n.getOwnedMembers().size(), 1);
    ASSERT_EQ(n.getOwnedMembers().front(), m);
    ASSERT_TRUE(m.hasNamespace());
    ASSERT_TRUE(m.getNamespace());
    ASSERT_EQ(m.getNamespaceRef(), n);
    ASSERT_EQ(n.getMembers().size(), 1);
    ASSERT_EQ(n.getMembers().front(), m);
    ASSERT_EQ(m.getMemberNamespace().size(), 1);
    ASSERT_EQ(m.getMemberNamespace().front(), n);
}

TEST_F(NamespaceTest, setNamespaceTest) {
    UmlManager mm;
    Package n = mm.create<Package>();
    Package m = mm.create<Package>();
    m.setNamespace(&n);
    ASSERT_EQ(n.getOwnedMembers().size(), 1);
    ASSERT_EQ(n.getOwnedMembers().front(), m);
    ASSERT_EQ(n.getMembers().size(), 1);
    ASSERT_EQ(n.getMembers().front(), m);
}

TEST_F(NamespaceTest, copyNamespaceTest) {
    UmlManager mm;
    Package n = mm.create<Package>();
    n.setName("test");
    Package m = mm.create<Package>();
    n.getOwnedMembers().add(m);
    Package n2 = n;
    ASSERT_EQ(n2, n);
    ASSERT_EQ(n2.getName(), n.getName());
    ASSERT_EQ(n2.getOwnedMembers().size(), 1);
    ASSERT_EQ(n2.getOwnedMembers().front(), m);
    ASSERT_EQ(n2.getMembers().size(), 1);
    ASSERT_EQ(n2.getMembers().front(), m);
}

TEST_F(NamespaceTest, SharedMemberTest) {
    UmlManager mm;
    Package n1 = mm.create<Package>();
    Package n2 = mm.create<Package>();
    Package m = mm.create<Package>();
    n1.getOwnedMembers().add(m);
    n2.getMembers().add(m);
    ASSERT_EQ(n1.getOwnedMembers().size(), 1);
    ASSERT_EQ(n1.getOwnedMembers().front(), m);
    ASSERT_EQ(n1.getMembers().size(), 1);
    ASSERT_EQ(n1.getMembers().front(), m);
    ASSERT_EQ(n1.getOwnedElements().size(), 1);
    ASSERT_EQ(n1.getOwnedElements().front(), m);
    ASSERT_EQ(n2.getOwnedMembers().size() , 0);
    ASSERT_EQ(n2.getMembers().size(), 1);
    ASSERT_EQ(n2.getMembers().front(), m);
    ASSERT_EQ(n2.getOwnedElements().size(), 0);
    ASSERT_TRUE(m.hasNamespace());
    ASSERT_TRUE(m.getNamespace());
    ASSERT_EQ(m.getNamespaceRef(), n1);
    ASSERT_EQ(m.getMemberNamespace().size(), 2);
    ASSERT_EQ(m.getMemberNamespace().front(), n1);
    ASSERT_EQ(m.getMemberNamespace().back(), n2);
}

TEST_F(NamespaceTest, removeMemeberFunctorTest) {
    UmlManager mm;
    Package n = mm.create<Package>();
    Package m = mm.create<Package>();
    n.getOwnedMembers().add(m);
    ASSERT_NO_THROW(n.getMembers().remove(m));
    ASSERT_EQ(n.getOwnedMembers().size(), 0);
    ASSERT_EQ(n.getMembers().size(), 0);
    ASSERT_EQ(n.getOwnedElements().size(), 0);
    ASSERT_FALSE(m.getNamespace());
    ASSERT_EQ(m.getMemberNamespace().size(), 0);
}