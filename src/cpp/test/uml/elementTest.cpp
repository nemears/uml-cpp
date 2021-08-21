#include "gtest/gtest.h"
#include "uml/element.h"
#include "uml/umlManager.h"
#include "uml/namedElement.h"
#include "uml/sequence.h"
#include "uml/relationship.h"
#include "uml/package.h"
#include "uml/class.h"
#include "uml/instanceSpecification.h"
#include "uml/stereotype.h"
#include "uml/packageMerge.h"
#include "test/umlTestUtil.h"

using namespace UML;

class ElementTest : public ::testing::Test {};

TEST_F(ElementTest, OverrideID_Test) {
  Package el1;
  el1.setID("7d18ee4282c64f528ec4fab67a75");
  ID id = ID::fromString("7d18ee4282c64f528ec4fab67a75");
  EXPECT_EQ(el1.getID(), id);
}

TEST_F(ElementTest, GetOwnedElementsTest) {
  Package el2;
  Package el3;
  el2.getPackagedElements().add(el3);
  EXPECT_FALSE(el2.getOwnedElements().empty());
  EXPECT_EQ(&el2.getOwnedElements().get(0), &el3);
}

TEST_F(ElementTest, GetOwnedElementsTestW_Manager) {
  UmlManager m;
  Package& el2 = m.create<Package>();
  Package& el3 = m.create<Package>();
  el2.getPackagedElements().add(el3);
  EXPECT_FALSE(el2.getOwnedElements().empty());
  EXPECT_EQ(&el2.getOwnedElements().get(0), &el3);
}

TEST_F(ElementTest, InvalidID_Test) {
  Package el3;
  EXPECT_THROW(el3.setID("not a uuid4"), InvalidID_Exception);
  EXPECT_NO_THROW(el3.setID("7d18ee4282c64f528ec4fab67a75"));
}

TEST_F(ElementTest, getNullOwnerTest) {
  Element e;
  ASSERT_TRUE(e.getOwner() == NULL);
}

TEST_F(ElementTest, setAndGetOwnerTest) {
  Package e;
  Package c;
  c.setOwningPackage(&e);
  ASSERT_TRUE(c.getOwner() == &e);
  ASSERT_TRUE(c.getOwner()->getID() == e.getID());
}

TEST_F(ElementTest, getOwnedElementsBasicTest) {
  Package e;
  Package c;
  ASSERT_NO_THROW(e.getPackagedElements().add(c));
  ASSERT_NO_THROW(e.getOwnedElements().get(c.getID()));
}

TEST_F(ElementTest, getOwnedElementByNameTest) {
  Package e;
  Package n;
  n.setName("name");
  Package b;
  ASSERT_NO_THROW(e.getPackagedElements().add(b));
  ASSERT_NO_THROW(e.getPackagedElements().add(n));
  // ASSERT_TRUE(e.getOwnedElements().get("name") == &n);
  ASSERT_NO_THROW(&e.getOwnedElements().get(n.getID()) == &n);
  ASSERT_NO_THROW(&e.getOwnedElements().get(b.getID()) == &b);
}

TEST_F(ElementTest, reindexID_TestW_Manager) {
  UmlManager m;
  Package& e1 = m.create<Package>();
  Package& e2 = m.create<Package>();
  e1.getPackagedElements().add(e2);
  e2.setID("190d1cb913dc44e6a064126891ae");
  ASSERT_NO_THROW(e1.getOwnedElements().get(e2.getID()));
}

TEST_F(ElementTest, reIndexID_Test) {
  Package e1;
  Package e2;
  e1.getPackagedElements().add(e2);
  e2.setID("190d1cb913dc44e6a064126891ae");
  ASSERT_NO_THROW(e1.getOwnedElements().get(e2.getID()));
}

TEST_F(ElementTest, basicRelationshipTestW_Manager) {
  UmlManager m;
  Package& e = m.create<Package>();
  Package& a = m.create<Package>();
  PackageMerge& r = m.create<PackageMerge>();
  e.getPackageMerge().add(r);
  r.setMergedPackage(&a);
  ASSERT_TRUE(r.getRelatedElements().size() == 2);
  ASSERT_TRUE(&r.getRelatedElements().front() == &e);
  ASSERT_TRUE(&r.getRelatedElements().back() == &a);
  ASSERT_TRUE(e.getRelationships().size() == 1);
  ASSERT_TRUE(&e.getRelationships().front() == &r);
  ASSERT_TRUE(a.getRelationships().size() == 1);
  ASSERT_TRUE(&a.getRelationships().front() == &r);
}

TEST_F(ElementTest, basicRelationshipTest) {
  Package e;
  Package a;
  PackageMerge r;
  e.getPackageMerge().add(r);
  r.setMergedPackage(&a);
  ASSERT_TRUE(r.getRelatedElements().size() == 2);
  ASSERT_TRUE(&r.getRelatedElements().front() == &e);
  ASSERT_TRUE(&r.getRelatedElements().back() == &a);
  ASSERT_TRUE(e.getRelationships().size() == 1);
  ASSERT_TRUE(&e.getRelationships().front() == &r);
  ASSERT_TRUE(a.getRelationships().size() == 1);
  ASSERT_TRUE(&a.getRelationships().front() == &r);
}

TEST_F(ElementTest, reindexRelationshipID_test) {
  Package e;
  Package a;
  PackageMerge r;
  e.getPackageMerge().add(r);
  r.setMergedPackage(&a);
  r.setID("190d1cb913dc44e6a064126891ae");
  e.setID("7d18ee4282c64f528ec4fab67a75");
  ASSERT_NO_THROW(r.getRelatedElements().get(e.getID()));
  ASSERT_NO_THROW(e.getRelationships().get(r.getID()));
}

TEST_F(ElementTest, setOwnerFunctorTest) {
  Package e;
  Package c;
  e.getPackagedElements().add(c);
  ASSERT_TRUE(c.getOwner() == &e);
  ASSERT_TRUE(e.getOwnedElements().size() == 1);
}

TEST_F(ElementTest, setOwnerTest) {
  Package e;
  Package c;
  c.setOwningPackage(&e);
  ASSERT_TRUE(e.getOwnedElements().count(c.getID()));
  ASSERT_TRUE(e.getOwnedElements().size() == 1);
}

TEST_F(ElementTest, overwriteOwnerTestW_Manager) {
  UmlManager m;
  Package& p1 = m.create<Package>();
  Package& p2 = m.create<Package>();
  Package& c = m.create<Package>();
  p1.getPackagedElements().add(c);
  c.setOwningPackage(&p2);
  ASSERT_TRUE(p2.getOwnedElements().size() == 1);
  ASSERT_TRUE(&p2.getOwnedElements().front() == &c);
  ASSERT_TRUE(c.getOwner() == &p2);
  ASSERT_TRUE(p1.getOwnedElements().size() == 0);
}

TEST_F(ElementTest, overwriteOwnerTest) {
  Package p1;
  Package p2;
  Package c;
  p1.getPackagedElements().add(c);
  c.setOwningPackage(&p2);
  ASSERT_TRUE(p2.getOwnedElements().size() == 1);
  ASSERT_TRUE(&p2.getOwnedElements().front() == &c);
  ASSERT_TRUE(c.getOwner() == &p2);
  ASSERT_TRUE(p1.getOwnedElements().size() == 0);
}

TEST_F(ElementTest, overwriteOwnerByOwnedElementsAddTestW_Manager) {
  UmlManager m;
  Package& p1 = m.create<Package>();
  Package& p2 = m.create<Package>();
  Package& c = m.create<Package>();
  p1.getPackagedElements().add(c);
  p2.getPackagedElements().add(c);
  ASSERT_TRUE(p2.getOwnedElements().size() == 1);
  ASSERT_TRUE(&p2.getOwnedElements().front() == &c);
  ASSERT_TRUE(c.getOwner() == &p2);
  ASSERT_TRUE(p1.getOwnedElements().size() == 0);
}

TEST_F(ElementTest, overwriteOwnerByOwnedElementsAddTest) {
  Package p1;
  Package p2;
  Package c;
  p1.getPackagedElements().add(c);
  p2.getPackagedElements().add(c);
  ASSERT_TRUE(p2.getOwnedElements().size() == 1);
  ASSERT_TRUE(&p2.getOwnedElements().front() == &c);
  ASSERT_TRUE(c.getOwner() == &p2);
  ASSERT_TRUE(p1.getOwnedElements().size() == 0);
}

TEST_F(ElementTest, CopyTestW_Manager) {
  UmlManager m;
  Package& e1 = m.create<Package>();
  Package& p1 = m.create<Package>();
  Package& c1 = m.create<Package>();
  e1.setOwningPackage(&p1);
  e1.getPackagedElements().add(c1);

  // copying like this should dereference the element
  // from manager but keep link to manager to access model
  // TODO: clone method for deep model copy
  Element e2 = e1;
  ASSERT_TRUE(e2.getOwnedElements().size() == 1);
  ASSERT_TRUE(&e2.getOwnedElements().front() == &c1);
  ASSERT_TRUE(e2.getID() == e1.getID());
  ASSERT_TRUE(e2.getOwner() == &p1);
}

TEST_F(ElementTest, CopyTest) {
  Package e1;
  Package c1;
  Package p1;
  e1.setOwningPackage(&p1);
  e1.getPackagedElements().add(c1);
  Element e2 = e1;
  ASSERT_TRUE(e2.getOwnedElements().size() == 1);
  ASSERT_TRUE(&e2.getOwnedElements().front() == &c1);
  ASSERT_TRUE(e2.getID() == e1.getID());
  ASSERT_TRUE(e2.getOwner() == &p1);
}

TEST_F(ElementTest, doINeedAnAddRelationshipFunctorTest) { // answer is yes
  Package e;
  PackageMerge r;
  e.getPackageMerge().add(r);
  ASSERT_TRUE(r.getRelatedElements().size() == 1);
  ASSERT_TRUE(&r.getRelatedElements().front() == &e);
}

// TEST_F(ElementTest, fullCopyTest) {
//   Package p;
//   Package c;
//   p.getPackagedElements().add(c);
//   Package p2 = p;
//   ASSERT_TRUE(p2.getPackagedElements().size() == 1);
//   ASSERT_TRUE(p2.getPackagedElements().front()->getID() == c.getID());
//   ASSERT_TRUE(p2.getPackagedElements().front() != &c);
// }

TEST_F(ElementTest, readOnlySequenceTest) {
  Package p;
  Package c1;
  Package c2;
  ASSERT_NO_THROW(p.getPackagedElements().add(c1));
  ASSERT_THROW(p.getOwnedElements().add(c2), ReadOnlySequenceException);
  ASSERT_THROW(p.getOwnedElements().remove(c1), ReadOnlySequenceException);
}

TEST_F(ElementTest, readOnlyRelationships) {
  Package p;
  Package m;
  PackageMerge r;
  p.getPackageMerge().add(r);
  r.setMergedPackage(&m);
  PackageMerge r2;
  ASSERT_THROW(p.getRelationships().add(r2), ReadOnlySequenceException);
  ASSERT_THROW(p.getRelationships().remove(r), ReadOnlySequenceException);
}

TEST_F(ElementTest, readOnlyRelatedElementsTest) {
  Package p;
  Package m;
  PackageMerge r;
  Package h;
  p.getPackageMerge().add(r);
  r.setMergedPackage(&m);
  ASSERT_THROW(r.getRelatedElements().remove(p), ReadOnlySequenceException);
  ASSERT_THROW(r.getRelatedElements().add(h), ReadOnlySequenceException);
}

// TEST_F(ElementTest, setAndGetOwnerTest2) {
//   UmlManager m;
//   Element& owner = m.create<Element>();
//   Element& ownee = m.create<Element>();
//   ownee.setOwner2(&owner);
//   ASSERT_TRUE(ownee.getOwner2() == &owner);
// }

// TEST_F(ElementTest, addToOwnedElementsTest) {
//   UmlManager m;
//   Element& owner = m.create<Element>();
//   Element& ownee = m.create<Element>();
//   ASSERT_NO_THROW(owner.getOwnedElements2().add(ownee));
//   ASSERT_TRUE(owner.getOwnedElements2().size() == 1);
//   ASSERT_TRUE(owner.getOwnedElements2().front() == &ownee);
//   ASSERT_TRUE(ownee.getOwner() == &owner); // TODO change
// }

TEST_F(ElementTest, isSameOrNullTest) {
  UmlManager m;
  Package& e = m.create<Package>();
  Package& c1 = m.create<Package>();
  Package& c2 = m.create<Package>();

  e.getPackagedElements().add(c1);
  e.getPackagedElements().add(c2);
  ASSERT_NO_THROW(c1.setOwningPackage(&e));
  ASSERT_TRUE(&e.getOwnedElements().front() == &c1);
  ASSERT_TRUE(&e.getOwnedElements().back() == &c2);
}

// TEST_F(ElementTest, checkAppliedStereotypeFunctorTest) {
//   UmlManager m;
//   Class& c = m.create<Class>();
//   InstanceSpecification& s = m.create<InstanceSpecification>();
//   ASSERT_THROW(c.getAppliedStereotypes().add(s), InvalidAppliedStereotypeException());
// }

TEST_F(ElementTest, AddAndRemoveAppliedStereotypetest) {
  UmlManager m;
  Class& c = m.create<Class>();
  InstanceSpecification& i = m.create<InstanceSpecification>();
  Stereotype& s = m.create<Stereotype>();
  i.setClassifier(&s);
  ASSERT_NO_THROW(c.getAppliedStereotypes().add(i));
  ASSERT_EQ(c.getOwnedElements().size(), 1);
  ASSERT_NO_THROW(c.getAppliedStereotypes().remove(i));
  ASSERT_EQ(c.getOwnedElements().size(), 0);
}

TEST_F(ElementTest, asFuncTest) {
  UmlManager m;
  Classifier& classifier = m.create<Class>();
  Class& clazz = classifier.as<Class>();
  ASSERT_EQ(classifier.getID(), clazz.getID());
}

TEST_F(ElementTest, copyAndChangeTest) {
  UmlManager m;
  Package& package = m.create<Package>();
  Class& ownedEl = m.create<Class>();
  {
    Package copy = package;
    copy.getPackagedElements().add(ownedEl);
    ASSERT_COPY_SEQUENCE_CORRECTLY(package, copy, &Element::getOwnedElements,
                                         &Namespace::getMembers,
                                         &Namespace::getOwnedMembers,
                                         &Package::getPackagedElements);
    ASSERT_EQ(package.getOwnedElements().size(), 1);
    ASSERT_EQ(package.getOwnedElements().front().getID(), ownedEl.getID());
    ASSERT_EQ(package.getOwnedMembers().size(), 1);
    ASSERT_EQ(package.getOwnedMembers().front().getID(), ownedEl.getID());
    ASSERT_EQ(package.getMembers().size(), 1);
    ASSERT_EQ(package.getMembers().front().getID(), ownedEl.getID());
    ASSERT_EQ(package.getPackagedElements().size(), 1);
    ASSERT_EQ(package.getPackagedElements().front().getID(), ownedEl.getID());
    ASSERT_TRUE(ownedEl.getOwner() != 0);
    ASSERT_EQ(ownedEl.getOwner()->getID(), package.getID());
    ASSERT_TRUE(ownedEl.getNamespace() != 0);
    ASSERT_EQ(ownedEl.getNamespace()->getID(), package.getID());
    ASSERT_EQ(ownedEl.getMemberNamespace().size(), 1);
    ASSERT_EQ(ownedEl.getMemberNamespace().front().getID(), package.getID());
    ASSERT_TRUE(ownedEl.getOwningPackage() != 0);
    ASSERT_EQ(ownedEl.getOwningPackage()->getID(), package.getID());
    package.getPackagedElements().remove(ownedEl);
    ASSERT_EQ(copy.getOwnedElements().size(), 0);
    ASSERT_EQ(copy.getOwnedMembers().size(), 0);
    ASSERT_EQ(copy.getMembers().size(), 0);
    ASSERT_EQ(copy.getPackagedElements().size(), 0);
    copy.getPackagedElements().add(m.create<Package>());
    ASSERT_COPY_SEQUENCE_CORRECTLY(package, copy, &Element::getOwnedElements,
                                         &Namespace::getMembers,
                                         &Namespace::getOwnedMembers,
                                         &Package::getPackagedElements);
  }
  ASSERT_EQ(package.getPackagedElements().size(), 1);
}