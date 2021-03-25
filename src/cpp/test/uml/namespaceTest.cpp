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
    nmspc.getOwnedElements().add(n);
    n.setOwner(&nmspc);
    nmspc.getMembers().add(n);
    n.setNamespace(&nmspc);
    n.setID("190d1cb9-13dc-44e6-a064-126891ae0033");
    ASSERT_TRUE(nmspc.getOwnedElements().get(n.getID()) != NULL);
    ASSERT_TRUE(nmspc.getMembers().get(n.getID()) != NULL);
}

TEST_F(NamespaceTest, reindexNameTest) {
    Namespace nmspc;
    NamedElement n;
    nmspc.setName("nmspc");
    nmspc.getOwnedElements().add(n);
    n.setOwner(&nmspc);
    nmspc.getMembers().add(n);
    n.setNamespace(&nmspc);
    n.setName("test");
    ASSERT_TRUE(nmspc.getOwnedElements().get("test") != NULL);
    ASSERT_TRUE(nmspc.getMembers().get("test") != NULL);
}

TEST_F(NamespaceTest, AddMemeberFunctorTest) {
    Namespace n;
    NamedElement m;
    n.getMembers().add(m);
    ASSERT_TRUE(n.getMembers().size() == 1);
    ASSERT_TRUE(n.getMembers().front() == &m);
    ASSERT_TRUE(m.getNamespace() == &n);
}

TEST_F(NamespaceTest, setNamespaceTest) {
    Namespace n;
    NamedElement m;
    m.setNamespace(&n);
    ASSERT_TRUE(n.getMembers().size() == 1);
    ASSERT_TRUE(n.getMembers().front() == &m);
}