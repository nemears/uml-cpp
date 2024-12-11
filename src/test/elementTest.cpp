#include "gtest/gtest.h"
#include <type_traits>

#include "uml/managers/templateTypeList.h"
#include "uml/managers/umlManager.h"
#include "uml/types/packageableElement/impl.h"
#include "uml/uml-stable.h"
// #include "test/umlTestUtil.h"

using namespace UML;

// UML_SET_INTEGRATION_TEST(ElementAppliedStereotypes, InstanceSpecification, EnumerationLiteral, &Element::getAppliedStereotypes)

class ElementTest : public ::testing::Test {};

template <class T>
struct TClass1 {};

template <class T>
struct TClass2 {};

TEST_F(ElementTest, TemplateTypeListTypeTest) {

    using TestTypes = TemplateTypeList<TClass1, TClass2>;

    auto isTypeResultSame = std::is_same<
            TClass1<int>,
            TemplateTypeListType<0, TestTypes>::result<int>
        >::value;

    ASSERT_TRUE(isTypeResultSame);

    auto indexResult1 = TemplateTypeListIndex<TClass1, TestTypes>::result;
    ASSERT_EQ(indexResult1, 0);
    auto indexResult2 = TemplateTypeListIndex<TClass2, TestTypes>::result;
    ASSERT_EQ(indexResult2, 1);
    auto indexResult3 = TemplateTypeListIndex<TemplateTypeListType<0, TestTypes>::result, TestTypes>::result;
    ASSERT_EQ(indexResult3, 0);

    // auto baseElResult = std::is_same<
    //         Package<UmlManager::BaseElement>, 
    //         TemplateTypeListType<0, UmlTypes>::result<UmlManager::BaseElement>
    //     >::value;
    // ASSERT_TRUE(baseElResult);
    // 
    // auto baseElResult2 = std::is_same<
    //         PackageableElement<UmlManager::BaseElement>, 
    //         TemplateTypeListType<1, UmlTypes>::result<UmlManager::BaseElement>
    //     >::value;
    // ASSERT_TRUE(baseElResult2);

    // auto packageIndex0 = TemplateTypeListIndex<Package, UmlTypes>::result;
    // ASSERT_EQ(packageIndex0, 0);
    // auto packageIndex = TemplateTypeListIndex<TemplateTypeListType<0, UmlTypes>::result, UmlTypes>::result; 
    // ASSERT_EQ(packageIndex, 0);

    // auto genBaseHierarchyResult0 = std::is_base_of<
    //         // UmlManager::GenBaseHierarchy<PackageableElement>,
    //         TemplateTypeListType<0, Package<UmlManager::BaseElement>::Info::BaseList>::template result<UmlManager::GenBaseHierarchy<TemplateTypeListType<0, Package<UmlManager::BaseElement>::Info::BaseList>::template result>>,
    //         UmlManager::GenBaseHierarchy<Package>
    //     >::value;
    // ASSERT_TRUE(genBaseHierarchyResult0);

    // is_same will not work because of temp.alias in spec
    // see https://open-std.org/JTC1/SC22/WG21/docs/cwg_active.html#1286
    // auto genBaseHierarchyResult1 = std::is_convertible<
    //         UmlManager::GenBaseHierarchy<PackageableElement>,
    //         UmlManager::GenBaseHierarchy<TemplateTypeListType<1, UmlTypes>::template result>
    //     >::value;
    // ASSERT_TRUE(genBaseHierarchyResult1);

    // auto result = std::is_convertible<
    //         TemplateTypeListType<0, UmlManager::Types>::template result<UmlManager::GenBaseHierarchy<TemplateTypeListType<0, UmlManager::Types>::template result>>, 
    //         Package<UmlManager::GenBaseHierarchy<Package>>
    //     >::value;
    // ASSERT_TRUE(result);
}

TEST_F(ElementTest, GenBaseHierarchyTest) {
    auto isBaseOfBaseElement = std::is_base_of<
            UmlManager::BaseElement, 
            Package<UmlManager::GenBaseHierarchy<Package>>
        >::value;
    ASSERT_TRUE(isBaseOfBaseElement);

    // auto isBaseOfGenBaseHierarchy = std::is_base_of<
    //         UmlManager::GenBaseHierarchy<Package, 1, false>,
    //         Package<UmlManager::GenBaseHierarchy<Package>>
    //     >::value;
    // ASSERT_TRUE(isBaseOfGenBaseHierarchy);

    // auto isBaseOfTypeList = std::is_base_of<
    //         TemplateTypeListType<0,Package<UmlManager::BaseElement>::Info::BaseList>::result<UmlManager::GenBaseHierarchy<TemplateTypeListType<0, Package<UmlManager::BaseElement>::Info::BaseList>::result>>,
    //         Package<UmlManager::GenBaseHierarchy<Package>>
    //     >::value;
    // ASSERT_TRUE(isBaseOfTypeList);

    // auto isInBaseList = std::is_same<
    //         PackageableElement<UmlManager::BaseElement>,
    //         TemplateTypeListType<0, Package<UmlManager::BaseElement>::Info::BaseList>::result<UmlManager::BaseElement>
    //     >::value;
    // ASSERT_TRUE(isInBaseList);

    // auto isInBaseList2 = std::is_same<
    //         PackageableElement<UmlManager::GenBaseHierarchy<PackageableElement>>,
    //         TemplateTypeListType<0, Package<UmlManager::BaseElement>::Info::BaseList>::result<UmlManager::GenBaseHierarchy<TemplateTypeListType<0, Package<UmlManager::BaseElement>::Info::BaseList>::result>>
    //     >::value;
    // ASSERT_TRUE(isInBaseList2);

    // auto isBase = std::is_base_of<
    //         PackageableElement<UmlManager::GenBaseHierarchy<PackageableElement>>, 
    //         Package<UmlManager::GenBaseHierarchy<Package>>
    //     >::value;
    // ASSERT_TRUE(isBase);

    // UmlManager m;
    // UmlManager::GenBaseHierarchy<PackageableElement>(1, m);
}

TEST_F(ElementTest, IsTest) {
    UmlManager m;
    auto pckg = m.create<Package>();
    ASSERT_TRUE(pckg->is<PackageableElement>());
    auto& pckgableEl = pckg->as<PackageableElement>();
    ASSERT_TRUE(pckgableEl.is<Package>());
    ASSERT_TRUE(pckgableEl.is<Namespace>());
    ASSERT_TRUE(pckgableEl.is<NamedElement>());
    ASSERT_TRUE(pckgableEl.is<Element>());
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

// Commenting out for now until compilable
/**

TEST_F(ElementTest, UmlPtrComparisonTest) {
    UmlManager m;
    auto pckg = m.create<Package>();
    auto child = m.create<Package>();
    pckg->getPackagedElements().add(*child);
    ASSERT_FALSE(pckg->getOwningPackage());
    ASSERT_TRUE(!pckg->getOwningPackage());
    ASSERT_EQ(child->getOwningPackage(), pckg);
    ASSERT_EQ(*(child->getOwningPackage()), *pckg);
    ASSERT_EQ(*child->getNamespace(), pckg);
    ASSERT_EQ(*(child->getNamespace()), *pckg);
    ASSERT_EQ(child->getOwner(), pckg);
    ASSERT_EQ(*(child->getOwner()), *pckg);
}

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
    ID pckgID = pckg->getID();
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
    ID newID = ID::randomID();
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
    ID ogID = pckg.id();    
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
    el->setID(ID::fromString("7d18ee4282c64f528ec4fab67a75"));
    ID id = ID::fromString("7d18ee4282c64f528ec4fab67a75");
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
    EXPECT_THROW(el3.setID(ID::fromString("not a uuid4")), InvalidUmlID_Exception);
    EXPECT_NO_THROW(el3.setID(ID::fromString("7d18ee4282c64f528ec4fab67a75")));
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
  ASSERT_EQ(*p2.getOwnedElements().front(), c);
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
**/
