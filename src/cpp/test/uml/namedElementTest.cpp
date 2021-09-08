#include "gtest/gtest.h"
#include "uml/namedElement.h"
#include "uml/sequence.h"
#include "uml/namespace.h"
#include "uml/package.h"
#include "uml/umlManager.h"
#include "test/umlTestUtil.h"

using namespace UML;

class NamedElementTest : public ::testing::Test {};

TEST_F(NamedElementTest, SetNameTest) {
    Package namedEl;
    namedEl.setName("test");
    EXPECT_EQ(namedEl.getName(), "test");
}

TEST_F(NamedElementTest, GetNullNameTest) {
    NamedElement ne;
    ASSERT_NO_THROW(ne.getName());
    EXPECT_TRUE(ne.getName().compare("") == 0);
}

// TEST_F(NamedElementTest, reIndexNameTest) {
//   Package e1;
//   Package e2;
//   e1.getPackagedElements().add(e2);
//   e2.setOwningPackage(&e1);
//   e2.setName("test");
//   ASSERT_TRUE(e1.getOwnedElements().get("test") != NULL);
// }

TEST_F(NamedElementTest, overwriteNamespaceTestW_Manager) {
  UmlManager m;
  Namespace& p1 = m.create<Package>();
  Namespace& p2 = m.create<Package>();
  NamedElement& c = m.create<Package>();
  p1.getOwnedMembers().add(c);
  c.setNamespace(&p2);
  ASSERT_TRUE(p2.getOwnedMembers().size() == 1);
  ASSERT_TRUE(&p2.getOwnedMembers().front() == &c);
  ASSERT_TRUE(c.getNamespace() == &p2);
  ASSERT_TRUE(p1.getOwnedMembers().size() == 0);
}

TEST_F(NamedElementTest, overwriteNamespaceTest) {
  Namespace p1;
  Namespace p2;
  NamedElement c;
  p1.getOwnedMembers().add(c);
  c.setNamespace(&p2);
  ASSERT_TRUE(p2.getOwnedMembers().size() == 1);
  ASSERT_TRUE(&p2.getOwnedMembers().front() == &c);
  ASSERT_TRUE(c.getNamespace() == &p2);
  ASSERT_TRUE(p1.getOwnedMembers().size() == 0);
}

TEST_F(NamedElementTest, overwriteNamespaceByOwnedMemebersAddTestW_Manager) {
  UmlManager m;
  Namespace& p1 = m.create<Package>();
  Namespace& p2 = m.create<Package>();
  NamedElement& c = m.create<Package>();
  p1.getOwnedMembers().add(c);
  p2.getOwnedMembers().add(c);
  ASSERT_TRUE(p2.getOwnedMembers().size() == 1);
  ASSERT_TRUE(&p2.getOwnedMembers().front() == &c);
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
  ASSERT_TRUE(&p2.getOwnedMembers().front() == &c);
  ASSERT_TRUE(c.getNamespace() == &p2);
  ASSERT_TRUE(p1.getOwnedMembers().size() == 0);
}

TEST_F(NamedElementTest, copyNamedElementTestW_Manager) {
  UmlManager m;
    Package& n = m.create<Package>();
    n.setName("test");
    Package& p = m.create<Package>();
    Package& c = m.create<Package>();
    n.setNamespace(&p);
    n.getPackagedElements().add(c);
    NamedElement n2 = n;
    ASSERT_TRUE(n2.getName().compare("test") == 0);
    ASSERT_TRUE(n2.getID() == n.getID());
    ASSERT_TRUE(n2.getOwner() == &p);
    ASSERT_TRUE(n2.getNamespace() == &p);
    ASSERT_TRUE(n2.getOwnedElements().size() == 1);
    ASSERT_TRUE(&n2.getOwnedElements().front() == &c);
    ASSERT_TRUE(c.getOwner() == &n);
}

TEST_F(NamedElementTest, copyNamedElementTest) {
    Package n;
    n.setName("test");
    Package p;
    Package c;
    n.setNamespace(&p);
    n.getPackagedElements().add(c);
    NamedElement n2 = n;
    ASSERT_TRUE(n2.getName().compare("test") == 0);
    ASSERT_TRUE(n2.getID() == n.getID());
    ASSERT_TRUE(n2.getOwner() == &p);
    ASSERT_TRUE(n2.getNamespace() == &p);
    ASSERT_TRUE(n2.getOwnedElements().size() == 1);
    ASSERT_TRUE(&n2.getOwnedElements().front() == &c);
    ASSERT_TRUE(c.getOwner() == &n);
}

TEST_F(NamedElementTest, visibilityTest) {
    NamedElement n;
    ASSERT_TRUE(n.getVisibility() == VisibilityKind::PUBLIC);
    ASSERT_NO_THROW(n.setVisibility(VisibilityKind::PRIVATE));
    ASSERT_TRUE(n.getVisibility() == VisibilityKind::PRIVATE);
}

TEST_F(NamedElementTest, copyAndEditTest) {
  UmlManager m;
  Package& p = m.create<Package>();
  Package& c = m.create<Package>();
  p.getPackagedElements().add(c);
  c.setName("test");
  c.setVisibility(VisibilityKind::PRIVATE);
  Package copy = c;
  ASSERT_NO_FATAL_FAILURE(ASSERT_COPY_SEQUENCE_CORRECTLY(c, copy, &NamedElement::getMemberNamespace));
  ASSERT_NO_FATAL_FAILURE(ASSERT_COPY_SINGLETON_CORRECTLY(c, copy, &NamedElement::getNamespace));
  ASSERT_EQ(c.getVisibility(), copy.getVisibility());
  p.getPackagedElements().remove(copy);
  c.setVisibility(VisibilityKind::PROTECTED);
  ASSERT_NO_FATAL_FAILURE(ASSERT_COPY_SEQUENCE_CORRECTLY(c, copy, &NamedElement::getMemberNamespace));
  ASSERT_NO_FATAL_FAILURE(ASSERT_COPY_SINGLETON_CORRECTLY(c, copy, &NamedElement::getNamespace));
  ASSERT_EQ(c.getVisibility(), copy.getVisibility());
  copy.setOwningPackage(&p);
  copy.setVisibility(VisibilityKind::PUBLIC);
  ASSERT_NO_FATAL_FAILURE(ASSERT_COPY_SEQUENCE_CORRECTLY(c, copy, &NamedElement::getMemberNamespace));
  ASSERT_NO_FATAL_FAILURE(ASSERT_COPY_SINGLETON_CORRECTLY(c, copy, &NamedElement::getNamespace));
  ASSERT_EQ(c.getVisibility(), copy.getVisibility());
  p.setOwningPackage(0);
  ASSERT_NO_FATAL_FAILURE(ASSERT_COPY_SEQUENCE_CORRECTLY(c, copy, &NamedElement::getMemberNamespace));
  ASSERT_NO_FATAL_FAILURE(ASSERT_COPY_SINGLETON_CORRECTLY(c, copy, &NamedElement::getNamespace));
}

TEST_F(NamedElementTest, singletonMethodsTest) {
    UmlManager m;
    Package p = m.create<Package>();
    Package c = m.create<Package>();
    ASSERT_FALSE(c.hasNamespace());
    ASSERT_FALSE(c.getNamespace());
    ASSERT_THROW(c.getNamespaceRef(), NullReferenceException);
    c.setOwningPackage(p);
    ASSERT_TRUE(c.hasNamespace());
    ASSERT_EQ(*c.getNamespace(), p);
    ASSERT_EQ(c.getNamespaceRef(), p);
    c.setOwningPackage(0);
    ASSERT_FALSE(c.hasNamespace());
    ASSERT_FALSE(c.getNamespace());
    ASSERT_THROW(c.getNamespaceRef(), NullReferenceException);
}