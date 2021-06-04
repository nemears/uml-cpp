#include "gtest/gtest.h"
#include "uml/namespace.h"

using namespace UML;

class NamespaceTest : public ::testing::Test {
    public:

        Namespace namespc;
        Namespace* namespcPtr;

    protected:

        NamespaceTest() {
            namespcPtr = new Namespace;
        }

        ~NamespaceTest() override {
            delete namespcPtr;
        }

        void SetUp() override {
            namespc.setName("test");
            namespcPtr->setName("ptrTest");
        }
};

// Really this test isn't necessary just did it for posterity and getting used to gtest
TEST_F(NamespaceTest, SetNameTest) {
    EXPECT_EQ(namespc.getName(), "test");
    EXPECT_EQ(namespcPtr->getName(), "ptrTest");
}

TEST_F(NamespaceTest, reindexID_Test) {
    Namespace nmspc;
    NamedElement n;
    nmspc.setName("nmspc");
    n.setNamespace(&nmspc);
    ASSERT_NO_THROW(n.setID("190d1cb9_13dc_44e6_a064_1268"));
    ASSERT_TRUE(nmspc.getOwnedElements().get(n.getID()) != NULL);
    ASSERT_TRUE(nmspc.getMembers().get(n.getID()) != NULL);
    ASSERT_TRUE(nmspc.getOwnedMembers().get(n.getID()) != 0);
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
    Namespace n;
    NamedElement m;
    n.getOwnedMembers().add(m);
    ASSERT_TRUE(n.getOwnedMembers().size() == 1);
    ASSERT_TRUE(n.getOwnedMembers().front() == &m);
    ASSERT_TRUE(m.getNamespace() == &n);
    ASSERT_TRUE(n.getMembers().size() == 1);
    ASSERT_TRUE(n.getMembers().front() == &m);
    ASSERT_TRUE(m.getMemberNamespace().size() == 1);
    ASSERT_TRUE(m.getMemberNamespace().front() == &n);
}

TEST_F(NamespaceTest, setNamespaceTest) {
    Namespace n;
    NamedElement m;
    m.setNamespace(&n);
    ASSERT_TRUE(n.getOwnedMembers().size() == 1);
    ASSERT_TRUE(n.getOwnedMembers().front() == &m);
    ASSERT_TRUE(n.getMembers().size() == 1);
    ASSERT_TRUE(n.getMembers().front() == &m);
}

TEST_F(NamespaceTest, copyNamespaceTest) {
    Namespace n;
    n.setName("test");
    NamedElement m;
    n.getOwnedMembers().add(m);
    Namespace n2 = n;
    ASSERT_TRUE(n2.getID() == n.getID());
    ASSERT_TRUE(n2.getName().compare(n.getName()) == 0);
    ASSERT_TRUE(n2.getOwnedMembers().size() == 1);
    ASSERT_TRUE(n2.getOwnedMembers().front() == &m);
    ASSERT_TRUE(n2.getMembers().size() == 1);
    ASSERT_TRUE(n2.getMembers().front() == &m);
}

TEST_F(NamespaceTest, SharedMemberTest) {
    Namespace n1;
    Namespace n2;
    NamedElement m;
    n1.getOwnedMembers().add(m);
    n2.getMembers().add(m);
    ASSERT_TRUE(n1.getOwnedMembers().size() == 1);
    ASSERT_TRUE(n1.getOwnedMembers().front() == &m);
    ASSERT_TRUE(n1.getMembers().size() == 1);
    ASSERT_TRUE(n1.getMembers().front() == &m);
    ASSERT_TRUE(n1.getOwnedElements().size() == 1);
    ASSERT_TRUE(n1.getOwnedElements().front() == &m);
    ASSERT_TRUE(n2.getOwnedMembers().size() == 0);
    ASSERT_TRUE(n2.getMembers().size() == 1);
    ASSERT_TRUE(n2.getMembers().front() == &m);
    ASSERT_TRUE(n2.getOwnedElements().size() == 0);
    ASSERT_TRUE(m.getNamespace() == &n1);
    ASSERT_TRUE(m.getMemberNamespace().size() == 2);
    ASSERT_TRUE(m.getMemberNamespace().front() == &n1);
    ASSERT_TRUE(m.getMemberNamespace().back() == &n2);
}

TEST_F(NamespaceTest, removeMemeberFunctorTest) {
    Namespace n;
    NamedElement m;
    n.getOwnedMembers().add(m);
    ASSERT_NO_THROW(n.getMembers().remove(m));
    ASSERT_TRUE(n.getOwnedMembers().size() == 0);
    ASSERT_TRUE(n.getMembers().size() == 0);
    ASSERT_TRUE(n.getOwnedElements().size() == 0);
    ASSERT_TRUE(m.getNamespace() == 0);
    ASSERT_TRUE(m.getMemberNamespace().size() == 0);
}