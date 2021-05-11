#include "gtest/gtest.h"
#include "uml/namedElement.h"
#include "uml/sequence.h"
#include "uml/namespace.h"

using namespace UML;

class NamedElementTest : public ::testing::Test {
    public:

        NamedElement namedEl;
        NamedElement* namedElPtr;

    protected:

        NamedElementTest() {
            namedElPtr = new NamedElement;
        }

        ~NamedElementTest() override {
            delete namedElPtr;
        }

        void SetUp() override {
            namedEl.setName("test");
            namedElPtr->setName("ptrTest");
        }
};

TEST_F(NamedElementTest, SetNameTest) {
    EXPECT_EQ(namedEl.getName(), "test");
    EXPECT_EQ(namedElPtr->getName(), "ptrTest");
}

TEST_F(NamedElementTest, GetNullNameTest) {
    NamedElement ne;
    ASSERT_NO_THROW(ne.getName());
    EXPECT_TRUE(ne.getName().compare("") == 0);
}

TEST_F(NamedElementTest, reIndexNameTest) {
  Element e1;
  NamedElement e2;
  e1.getOwnedElements().add(e2);
  e2.setOwner(&e1);
  e2.setName("test");
  ASSERT_TRUE(e1.getOwnedElements().get("test") != NULL);
}

TEST_F(NamedElementTest, overwriteNamespaceTest) {
  Namespace p1;
  Namespace p2;
  NamedElement c;
  p1.getOwnedMembers().add(c);
  c.setNamespace(&p2);
  ASSERT_TRUE(p2.getOwnedMembers().size() == 1);
  ASSERT_TRUE(p2.getOwnedMembers().front() == &c);
  ASSERT_TRUE(c.getNamespace() == &p2);
  ASSERT_TRUE(p1.getOwnedMembers().size() == 0);
}

TEST_F(NamedElementTest, overwriteNamespaceByOwnedMemebersAddTest) {
  Namespace p1;
  Namespace p2;
  NamedElement c;
  p1.getOwnedMembers().add(c);
  p2.getOwnedMembers().add(c);
  ASSERT_TRUE(p2.getOwnedMembers().size() == 1);
  ASSERT_TRUE(p2.getOwnedMembers().front() == &c);
  ASSERT_TRUE(c.getNamespace() == &p2);
  ASSERT_TRUE(p1.getOwnedMembers().size() == 0);
}

TEST_F(NamedElementTest, copyNamedElementTest) {
    NamedElement n;
    n.setName("test");
    Namespace p;
    Element c;
    n.setNamespace(&p);
    n.getOwnedElements().add(c);
    NamedElement n2 = n;
    ASSERT_TRUE(n2.getName().compare("test") == 0);
    ASSERT_TRUE(n2.getID() == n.getID());
    ASSERT_TRUE(n2.getOwner() == &p);
    ASSERT_TRUE(n2.getNamespace() == &p);
    ASSERT_TRUE(n2.getOwnedElements().size() == 1);
    ASSERT_TRUE(n2.getOwnedElements().front() == &c);
    ASSERT_TRUE(c.getOwner() == &n);
}

TEST_F(NamedElementTest, visibilityTest) {
    NamedElement n;
    ASSERT_TRUE(n.getVisibility() == VisibilityKind::PUBLIC);
    ASSERT_NO_THROW(n.setVisibility(VisibilityKind::PRIVATE));
    ASSERT_TRUE(n.getVisibility() == VisibilityKind::PRIVATE);
}