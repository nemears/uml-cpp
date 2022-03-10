#include "gtest/gtest.h"

#include "uml/uml-stable.h"


// #include "test/umlTestUtil.h"
// #include "test/yumlParsersTest.h"

using namespace UML;

class ElementTest : public ::testing::Test {};

TEST_F(ElementTest, UmlPtrTest) {
    UmlManager m;
    Package& pckg = *m.create<Package>();
    Package& child = *m.create<Package>();
    pckg.getPackagedElements().add(child);
    ASSERT_FALSE(pckg.getOwningPackage().has());
    ASSERT_FALSE(pckg.getOwningPackage());
    ASSERT_TRUE(!pckg.getOwningPackage());
    ASSERT_TRUE(child.getOwningPackage().has());
    ASSERT_TRUE(child.getOwningPackage());
    ASSERT_FALSE(!child.getOwningPackage());
    ASSERT_EQ(child.getOwningPackage(), &pckg);
    ASSERT_EQ(*(child.getOwningPackage()), pckg);
    ASSERT_EQ(child.getOwningPackage()->getID(), pckg.getID());
}

TEST_F(ElementTest, UmlPtrComparisonTest) {
    UmlManager m;
    PackagePtr pckg = m.create<Package>();
    PackagePtr child = m.create<Package>();
    pckg->getPackagedElements().add(*child);
    ASSERT_FALSE(pckg->getOwningPackage());
    ASSERT_TRUE(!pckg->getOwningPackage());
    ASSERT_EQ(child->getOwningPackage(), pckg);
    ASSERT_EQ(*(child->getOwningPackage()), *pckg);
    ASSERT_EQ(child->getNamespace(), pckg);
    ASSERT_EQ(*(child->getNamespace()), *pckg);
    ASSERT_EQ(child->getOwner(), pckg);
    ASSERT_EQ(*(child->getOwner()), *pckg);
}

TEST_F(ElementTest, UmlPtrScopeTest) {
    UmlManager m;
    m.lossless(false);
    m.lazy(false);
    ID pckgID;
    {
        PackagePtr pckg = m.create<Package>();
        pckgID = pckg->getID();
        ASSERT_TRUE(m.loaded(pckgID));
    }
    ASSERT_FALSE(m.loaded(pckgID));
    m.lossless(true);
    m.lazy(true);
    {
        PackagePtr pckg = m.create<Package>();
        pckgID = pckg->getID();
        ASSERT_TRUE(m.loaded(pckgID));
    }
    ASSERT_TRUE(m.loaded(pckgID));
}

TEST_F(ElementTest, UmlPtrReleaseTest) {
    UmlManager m;
    m.mount(".");
    PackagePtr pckg = m.create<Package>();
    PackagePtr child = m.create<Package>();
    ID pckgID = pckg->getID();
    ID childID = child->getID();
    pckg->getPackagedElements().add(*child);
    ASSERT_TRUE(m.loaded(pckgID));
    pckg.release();
    ASSERT_FALSE(m.loaded(pckgID));
    ASSERT_EQ(child->getOwningPackage().id(), pckgID);
    ASSERT_FALSE(m.loaded(pckgID));
    pckg.aquire();
    ASSERT_TRUE(m.loaded(pckgID));
}

TEST_F(ElementTest, AcessReleasedPtrTest) {
    UmlManager m;
    m.mount(".");
    // m.lazy(false);
    // m.lossless(false);
    PackagePtr pckg = m.create<Package>();
    pckg.release();
    ASSERT_FALSE(m.loaded(pckg.id()));
    ASSERT_EQ(pckg->getID(), pckg.id());
    ASSERT_TRUE(m.loaded(pckg.id()));
}

TEST_F(ElementTest, reassignPtrTest) { // TODO recreate managermountstresstest error
    UmlManager m;
    PackagePtr pckg = m.create<Package>();
    PackagePtr ogPckg = pckg;
    ID ogID = pckg.id();    
    {
        PackagePtr newPckg = m.create<Package>();
        pckg = newPckg;
    }
    pckg->setOwningPackage(*pckg);
    ASSERT_NE(pckg.id(), ogID);
    ASSERT_NE(pckg.id(), ogPckg.id());
    ASSERT_EQ(pckg.id(), pckg->getID());
    ASSERT_EQ(ogPckg.id(), ogPckg->getID());
    m.mount(".");
    pckg.release();
    ASSERT_FALSE(pckg.loaded());
    ASSERT_TRUE(ogPckg.loaded());
    ogPckg.release();
    ASSERT_FALSE(pckg.loaded());
    ASSERT_FALSE(ogPckg.loaded());
}

TEST_F(ElementTest, OverrideID_Test) {
    UmlManager m;
    PackagePtr el = m.create<Package>();
    el->setID("7d18ee4282c64f528ec4fab67a75");
    ID id = ID::fromString("7d18ee4282c64f528ec4fab67a75");
    EXPECT_EQ(el->getID(), id);
}

TEST_F(ElementTest, GetOwnedElementsTest) {
  UmlManager m;
  Package& el2 = *m.create<Package>();
  Package& el3 = *m.create<Package>();
  el2.getPackagedElements().add(el3);
  EXPECT_FALSE(el2.getOwnedElements().empty());
  EXPECT_EQ(el2.getOwnedElements().front(), el3);
}

TEST_F(ElementTest, InvalidID_Test) {
    UmlManager m;
    Package& el3 = *m.create<Package>();
    EXPECT_THROW(el3.setID("not a uuid4"), InvalidID_Exception);
    EXPECT_NO_THROW(el3.setID("7d18ee4282c64f528ec4fab67a75"));
}

TEST_F(ElementTest, getNullOwnerTest) {
    UmlManager m;
    PackagePtr e = m.create<Package>();
    ASSERT_THROW(e->getOwner()->getID(), NullPtrException);
    ASSERT_FALSE(e->getOwner());
    ASSERT_EQ(e->getOwner().id(), ID::nullID());
    ASSERT_FALSE(e->getOwner().has());
    ASSERT_THROW(*e->getOwner(), NullPtrException);
}

TEST_F(ElementTest, setAndGetOwnerTest) {
    UmlManager m;
    Package& e = *m.create<Package>();
    Package& c = *m.create<Package>();
    c.setOwningPackage(e);
    ASSERT_TRUE(c.getOwner());
    ASSERT_EQ(*c.getOwner(), e);
    ASSERT_TRUE(c.getOwner()->getID() == e.getID());
}

TEST_F(ElementTest, getOwnedElementsBasicTest) {
    UmlManager m;
    Package& e = *m.create<Package>();
    Package& c = *m.create<Package>();
    ASSERT_NO_THROW(e.getPackagedElements().add(c));
    ASSERT_NO_THROW(e.getOwnedElements().get(c.getID()));
}

TEST_F(ElementTest, getOwnedElementByNameTest) {
    UmlManager m;
    Package& e = *m.create<Package>();
    Package& n = *m.create<Package>();
    n.setName("name");
    Package& b = *m.create<Package>();
    ASSERT_NO_THROW(e.getPackagedElements().add(b));
    ASSERT_NO_THROW(e.getPackagedElements().add(n));
    ASSERT_EQ(e.getOwnedElements().get("name"), n);
    ASSERT_NO_THROW(&e.getOwnedElements().get(n.getID()) == &n);
    ASSERT_NO_THROW(&e.getOwnedElements().get(b.getID()) == &b);
}

TEST_F(ElementTest, reIndexID_Test) {
    UmlManager m;
    Package& e1 = *m.create<Package>();
    Package& e2 = *m.create<Package>();
    e1.getPackagedElements().add(e2);
    e2.setID("190d1cb913dc44e6a064126891ae");
    ASSERT_NO_THROW(e1.getOwnedElements().get(e2.getID()));
}

TEST_F(ElementTest, basicRelationshipTest) {
  UmlManager m;
  Package& e = *m.create<Package>();
  Package& a = *m.create<Package>();
  PackageMerge& r = *m.create<PackageMerge>();
  e.getPackageMerge().add(r);
  r.setMergedPackage(&a);
  ASSERT_EQ(r.getRelatedElements().size(), 2);
  ASSERT_TRUE(r.getRelatedElements().contains(e.getID()));
  ASSERT_TRUE(r.getRelatedElements().contains(a.getID()));
  ASSERT_EQ(e.getPackageMerge().size(), 1);
  ASSERT_EQ(e.getPackageMerge().front(), r);
}

TEST_F(ElementTest, reindexRelationshipID_test) {
    UmlManager m;
    Package& e = *m.create<Package>();
    Package& a = *m.create<Package>();
    PackageMerge& r = *m.create<PackageMerge>();
    e.getPackageMerge().add(r);
    r.setMergedPackage(&a);
    r.setID("190d1cb913dc44e6a064126891ae");
    e.setID("7d18ee4282c64f528ec4fab67a75");
    ASSERT_NO_THROW(r.getRelatedElements().get(e.getID()));
    ASSERT_NO_THROW(e.getPackageMerge().get(r.getID()));
}

TEST_F(ElementTest, setOwnerFunctorTest) {
    UmlManager m;
    Package& e = *m.create<Package>();
    Package& c = *m.create<Package>();
    e.getPackagedElements().add(c);
    ASSERT_EQ(*c.getOwner(), e);
    ASSERT_EQ(e.getOwnedElements().size(), 1);
}

TEST_F(ElementTest, setOwnerTest) {
    UmlManager m;
    Package& e = *m.create<Package>();
    Package& c = *m.create<Package>();
    c.setOwningPackage(&e);
    ASSERT_TRUE(e.getOwnedElements().contains(c.getID()));
    ASSERT_TRUE(e.getOwnedElements().size() == 1);
}

TEST_F(ElementTest, overwriteOwnerTest) {
  UmlManager m;
  Package& p1 = *m.create<Package>();
  Package& p2 = *m.create<Package>();
  Package& c = *m.create<Package>();
  p1.getPackagedElements().add(c);
  c.setOwningPackage(p2);
  ASSERT_EQ(p2.getOwnedElements().size(), 1);
  ASSERT_EQ(p2.getOwnedElements().front(), c);
  ASSERT_EQ(*c.getOwner(), p2);
  ASSERT_EQ(p1.getOwnedElements().size(), 0);
}

TEST_F(ElementTest, overwriteOwnerByOwnedElementsAddTest) {
  UmlManager m;
  Package& p1 = *m.create<Package>();
  Package& p2 = *m.create<Package>();
  Package& c = *m.create<Package>();
  p1.getPackagedElements().add(c);
  p2.getPackagedElements().add(c);
  ASSERT_EQ(p2.getOwnedElements().size(), 1);
  ASSERT_EQ(p2.getOwnedElements().front(), c);
  ASSERT_EQ(*c.getOwner(), p2);
  ASSERT_EQ(p1.getOwnedElements().size(), 0);
}

TEST_F(ElementTest, readOnlySequenceTest) {
    UmlManager m;
    Package& p = *m.create<Package>();
    Package& c1 = *m.create<Package>();
    Package& c2 = *m.create<Package>();
    ASSERT_NO_THROW(p.getPackagedElements().add(c1));
    ASSERT_THROW(p.getOwnedElements().add(c2), ReadOnlySetException);
    ASSERT_THROW(p.getOwnedElements().remove(c1), ReadOnlySetException);
}

TEST_F(ElementTest, readOnlyRelatedElementsTest) {
    UmlManager mm;
    Package& p = *mm.create<Package>();
    Package& m = *mm.create<Package>();
    PackageMerge& r = *mm.create<PackageMerge>();
    Package& h = *mm.create<Package>();
    p.getPackageMerge().add(r);
    r.setMergedPackage(&m);
    ASSERT_THROW(r.getRelatedElements().remove(p), ReadOnlySetException);
    ASSERT_THROW(r.getRelatedElements().add(h), ReadOnlySetException);
}

// TEST_F(ElementTest, checkAppliedStereotypeFunctorTest) {
//   UmlManager m;
//   Class& c = m.create<Class>();
//   InstanceSpecification& s = m.create<InstanceSpecification>();
//   ASSERT_THROW(c.getAppliedStereotypes().add(s), InvalidAppliedStereotypeException());
// }

TEST_F(ElementTest, AddAndRemoveAppliedStereotypetest) {
  UmlManager m;
  Class& c = *m.create<Class>();
  InstanceSpecification& i = *m.create<InstanceSpecification>();
  Stereotype& s = *m.create<Stereotype>();
  i.getClassifiers().add(s);
  ASSERT_NO_THROW(c.getAppliedStereotypes().add(i));
  ASSERT_EQ(c.getOwnedElements().size(), 1);
  ASSERT_NO_THROW(c.getAppliedStereotypes().remove(i));
  ASSERT_EQ(c.getOwnedElements().size(), 0);
}

TEST_F(ElementTest, asFuncTest) {
  UmlManager m;
  UmlPtr<Classifier> classifier = m.create<Class>();
  Class& clazz = classifier->as<Class>();
  ASSERT_EQ(classifier->getID(), clazz.getID());
}