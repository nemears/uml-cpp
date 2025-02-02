#include "gtest/gtest.h"

#include "uml/uml-stable.h"
#include "egm/egm.h"

using namespace UML;

// UML_SET_INTEGRATION_TEST(ElementAppliedStereotypes, InstanceSpecification, EnumerationLiteral, &Element::getAppliedStereotypes)

class ElementTest : public ::testing::Test {};

TEST_F(ElementTest, IsTest) {
    UmlManager m;
    auto pckg = m.create<Package>();
    ASSERT_TRUE(pckg->is<PackageableElement>());
    auto& pckgableEl = pckg->as<PackageableElement>();
    ASSERT_TRUE(pckgableEl.is<Package>());
    ASSERT_TRUE(pckgableEl.is<Namespace>());
    ASSERT_TRUE(pckgableEl.is<NamedElement>());
    ASSERT_TRUE(pckgableEl.is<Element>());
    ASSERT_EQ(pckgableEl.as<Package>(), dynamic_cast<Package<UmlManager::GenBaseHierarchy<Package>>&>(pckgableEl));
    auto& nmspc = pckg->as<Namespace>(); 
    ASSERT_EQ(nmspc, dynamic_cast<Namespace<UmlManager::GenBaseHierarchy<Namespace>>&>(pckgableEl));
    ASSERT_EQ(pckgableEl.as<NamedElement>(), dynamic_cast<NamedElement<UmlManager::GenBaseHierarchy<NamedElement>>&>(pckgableEl));
    ASSERT_EQ(pckgableEl.as<Element>(), dynamic_cast<Element<UmlManager::GenBaseHierarchy<Element>>&>(pckgableEl));
}

TEST_F(ElementTest, UmlPtrTest) {
    UmlManager m;
    auto& pckg = *m.create<Package>();
    auto& child = *m.create<Package>();
    child.setOwningPackage(pckg);
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

TEST_F(ElementTest, AddToSimpleSetTest) {
    UmlManager m;
    auto& pckg = *m.create<Package>();
    auto& child = *m.create<Package>();
    pckg.getPackagedElements().add(child);
    ASSERT_TRUE(pckg.getPackagedElements().contains(child));
    ASSERT_EQ(*pckg.getPackagedElements().front(), child);
}

TEST_F(ElementTest, simpleOppositeTest) {
    UmlManager m;
    auto& pckg = *m.create<Package>();
    auto& child = *m.create<Package>();
    pckg.getPackagedElements().add(child);
    ASSERT_TRUE(pckg.getPackagedElements().contains(child));
    ASSERT_EQ(*pckg.getPackagedElements().front(), child);
    ASSERT_TRUE(child.getOwningPackage());
    ASSERT_EQ(*child.getOwningPackage(), pckg);
}

TEST_F(ElementTest, UmlPtrComparisonTest) {
    UmlManager m;
    auto pckg = m.create<Package>();
    auto child = m.create<Package>();
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

// Commenting out for now until compilable

// TEST_F(ElementTest, UmlPtrScopeTest) {
//     UmlManager m;
//     m.mount(".");
//     // m.lossless(false);
//     m.lazy(false);
//     ID pckgID;
//     {
//         PackagePtr pckg = m.create<Package>();
//         pckgID = pckg->getID();
//         ASSERT_TRUE(m.loaded(pckgID));
//     }
//     ASSERT_FALSE(m.loaded(pckgID));
//     // m.lossless(true);
//     m.lazy(true);
//     {
//         PackagePtr pckg = m.create<Package>();
//         pckgID = pckg->getID();
//         ASSERT_TRUE(m.loaded(pckgID));
//     }
//     ASSERT_TRUE(m.loaded(pckgID));
// }

TEST_F(ElementTest, UmlPtrReleaseTest) {
    UmlManager m;
    m.mount(".");
    auto pckg = m.create<Package>();
    auto child = m.create<Package>();
    EGM::ID pckgID = pckg->getID();
    pckg->getPackagedElements().add(*child);
    ASSERT_TRUE(m.loaded(pckgID));
    pckg.release();
    ASSERT_FALSE(m.loaded(pckgID));
    ASSERT_EQ(child->getOwningPackage().id(), pckgID);
    ASSERT_FALSE(m.loaded(pckgID)); 
}

TEST_F(ElementTest, UmlPtrReleaseAquireTest) {
    UmlManager m;
    m.mount(".");
    auto pckg = m.create<Package>();
    auto child = m.create<Package>();
    EGM::ID pckgID = pckg->getID();
    pckg->getPackagedElements().add(*child);
    ASSERT_TRUE(m.loaded(pckgID));
    pckg.release();
    ASSERT_FALSE(m.loaded(pckgID));
    ASSERT_EQ(child->getOwningPackage().id(), pckgID);
    ASSERT_FALSE(m.loaded(pckgID));
    pckg.aquire();
    ASSERT_TRUE(m.loaded(pckgID));
}

TEST_F(ElementTest, ReindexTest) {

    UmlManager m;
    auto p = m.create<Package>();
    auto alsoP = p;
    EGM::ID newID = EGM::ID::randomID();
    p->setID(newID);
    ASSERT_EQ(p->getID(), newID);
    ASSERT_TRUE(p.loaded());
    ASSERT_TRUE(p.loaded());
    ASSERT_EQ(p.id(), p->getID());
    ASSERT_EQ(p.id(), newID);
    ASSERT_EQ(alsoP.id(), alsoP->getID());
    ASSERT_EQ(alsoP.id(), newID);
}

TEST_F(ElementTest, AcessReleasedPtrTest) {
    UmlManager m;
    m.mount(".");
    // m.lazy(false);
    // m.lossless(false);
    auto pckg = m.create<Package>();
    pckg.release();
    ASSERT_FALSE(m.loaded(pckg.id()));
    ASSERT_EQ(pckg->getID(), pckg.id());
    ASSERT_TRUE(m.loaded(pckg.id()));
}

TEST_F(ElementTest, reassignPtrTest) { // TODO recreate managermountstresstest error
    UmlManager m;
    auto pckg = m.create<Package>();
    auto ogPckg = pckg;
    EGM::ID ogID = pckg.id();    
    {
        auto newPckg = m.create<Package>();
        pckg = newPckg;
    }
    pckg->setOwningPackage(*ogPckg);
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
    auto el = m.create<Package>();
    el->setID(EGM::ID::fromString("7d18ee4282c64f528ec4fab67a75"));
    EGM::ID id = EGM::ID::fromString("7d18ee4282c64f528ec4fab67a75");
    EXPECT_EQ(el->getID(), id);
}

TEST_F(ElementTest, GetOwnedElementsTest) {
  UmlManager m;
  auto& el2 = *m.create<Package>();
  auto& el3 = *m.create<Package>();
  el2.getPackagedElements().add(el3);
  EXPECT_FALSE(el2.getOwnedElements().empty());
  EXPECT_EQ(*el2.getOwnedElements().front(), el3);
}

TEST_F(ElementTest, InvalidID_Test) {
    UmlManager m;
    auto& el3 = *m.create<Package>();
    EXPECT_THROW(el3.setID(EGM::ID::fromString("not a uuid4")), EGM::InvalidUmlID_Exception);
    EXPECT_NO_THROW(el3.setID(EGM::ID::fromString("7d18ee4282c64f528ec4fab67a75")));
}

TEST_F(ElementTest, getNullOwnerTest) {
    UmlManager m;
    auto e = m.create<Package>();
    ASSERT_THROW(e->getOwner()->getID(), EGM::NullPtrException);
    ASSERT_FALSE(e->getOwner());
    ASSERT_EQ(e->getOwner().id(), EGM::ID::nullID());
    ASSERT_FALSE(e->getOwner().has());
    ASSERT_THROW(*e->getOwner(), EGM::NullPtrException);
}

TEST_F(ElementTest, setAndGetOwnerTest) {
    UmlManager m;
    auto& e = *m.create<Package>();
    auto& c = *m.create<Package>();
    c.setOwningPackage(e);
    ASSERT_TRUE(c.getOwner());
    ASSERT_EQ(*c.getOwner(), e);
    ASSERT_TRUE(c.getOwner()->getID() == e.getID());
}

TEST_F(ElementTest, getOwnedElementsBasicTest) {
    UmlManager m;
    auto& e = *m.create<Package>();
    auto& c = *m.create<Package>();
    ASSERT_NO_THROW(e.getPackagedElements().add(c));
    ASSERT_NO_THROW(e.getOwnedElements().get(c.getID()));
}

// TEST_F(ElementTest, getOwnedElementByNameTest) {
//     UmlManager m;
//     Package& e = *m.create<Package>();
//     Package& n = *m.create<Package>();
//     n.setName("name");
//     Package& b = *m.create<Package>();
//     ASSERT_NO_THROW(e.getPackagedElements().add(b));
//     ASSERT_NO_THROW(e.getPackagedElements().add(n));
//     ASSERT_EQ(e.getOwnedElements().get("name"), n);
//     ASSERT_NO_THROW(&e.getOwnedElements().get(n.getID()));
//     ASSERT_NO_THROW(&e.getOwnedElements().get(b.getID()));
// }

// TODO uncomment!!
// TEST_F(ElementTest, basicRelationshipTest) {
//   UmlManager m;
//   Package& e = *m.create<Package>();
//   Package& a = *m.create<Package>();
//   PackageMerge& r = *m.create<PackageMerge>();
//   e.getPackageMerge().add(r);
//   r.setMergedPackage(&a);
//   ASSERT_EQ(r.getRelatedElements().size(), 2);
//   ASSERT_TRUE(r.getRelatedElements().contains(e.getID()));
//   ASSERT_TRUE(r.getRelatedElements().contains(a.getID()));
//   ASSERT_EQ(e.getPackageMerge().size(), 1);
//   ASSERT_EQ(e.getPackageMerge().front(), r);
// }

TEST_F(ElementTest, setOwnerFunctorTest) {
    UmlManager m;
    auto& e = *m.create<Package>();
    auto& c = *m.create<Package>();
    e.getPackagedElements().add(c);
    ASSERT_EQ(*c.getOwner(), e);
    ASSERT_EQ(e.getOwnedElements().size(), 1);
}

TEST_F(ElementTest, setOwnerTest) {
    UmlManager m;
    auto& e = *m.create<Package>();
    auto& c = *m.create<Package>();
    c.setOwningPackage(&e);
    ASSERT_TRUE(e.getOwnedElements().contains(c.getID()));
    ASSERT_TRUE(e.getOwnedElements().size() == 1);
}

TEST_F(ElementTest, overwriteOwnerTest) {
  UmlManager m;
  auto& p1 = *m.create<Package>();
  auto& p2 = *m.create<Package>();
  auto& c = *m.create<Package>();
  p1.getPackagedElements().add(c);
  c.setOwningPackage(p2);
  ASSERT_EQ(p2.getOwnedElements().size(), 1);
  ASSERT_EQ(*p2.getOwnedElements().front(), c);
  ASSERT_EQ(*c.getOwner(), p2);
  ASSERT_EQ(p1.getOwnedElements().size(), 0);
}

TEST_F(ElementTest, overwriteOwnerByOwnedElementsAddTest) {
  UmlManager m;
  auto& p1 = *m.create<Package>();
  auto& p2 = *m.create<Package>();
  auto& c = *m.create<Package>();
  p1.getPackagedElements().add(c);
  p2.getPackagedElements().add(c);
  ASSERT_EQ(p2.getOwnedElements().size(), 1);
  ASSERT_EQ(*p2.getOwnedElements().front(), c);
  ASSERT_EQ(*c.getOwner(), p2);
  ASSERT_EQ(p1.getOwnedElements().size(), 0);
}

// TODO uncomment
// TEST_F(ElementTest, asFuncTest) {
//   UmlManager m;
//   UmlPtr<Classifier> classifier = m.create<Class>();
//   Class& clazz = classifier->as<Class>();
//   ASSERT_EQ(classifier->getID(), clazz.getID());
// }
// 
// TEST_F(ElementTest, emitJSON_test) {
//     UmlManager m;
//     PackagePtr p = m.create<Package>();
//     PackagePtr c = m.create<Package>();
//     p->setID(ID::fromString("Wmhz0dIdjUbcWmTn7EL4Zz261oy6"));
//     c->setID(ID::fromString("GcJO3kDBnbRcT4f&Kwf9arl6YOmS"));
//     p->getPackagedElements().add(*c);
//     EmitterData data;
//     data.isJSON = true;
//     data.mode = SerializationMode::WHOLE;
//     std::string generatedEmit = emit(*p, data);
//     std::string expectedEmit = "{\"Package\": {\"id\": \"Wmhz0dIdjUbcWmTn7EL4Zz261oy6\", \"packagedElements\": [{\"Package\": {\"id\": \"GcJO3kDBnbRcT4f&Kwf9arl6YOmS\"}}]}}";
//     ASSERT_EQ(generatedEmit, expectedEmit);
// }
//
