#include "gtest/gtest.h"
#include <type_traits>

#include "uml/managers/abstractManager.h"
#include "uml/managers/dummyManager.h"
#include "uml/managers/templateTypeList.h"
#include "uml/managers/typeInfo.h"
#include "uml/managers/umlManager.h"
#include "uml/types/namedElement/definition.h"
#include "uml/types/namespace/impl.h"
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
}

namespace UML {

    template <class>
    class ProxyNamespaceDefinition;

    template <class>
    class ProxyNamedElement;

    template <class ManagerPolicy>
    class ProxyNamedElementDefinition : public ManagerPolicy {
        public:
           using Info = TypeInfo<ProxyNamedElement, TemplateTypeList<Element>>;
           virtual ReadOnlySet<ProxyNamespaceDefinition, ProxyNamedElementDefinition>& getNmspc() = 0;
           using ManagerPolicy::ManagerPolicy;
    };

    template <>
    struct ElementInfo<ProxyNamedElement> : public DefaultInfo {};

    template <class>
    class ProxyNamespace;

    template <class ManagerPolicy>
    class ProxyNamespaceDefinition : public ManagerPolicy {
        public:
            using Info = TypeInfo<ProxyNamespace, TemplateTypeList<ProxyNamedElement>>;
            virtual Set<ProxyNamedElementDefinition, ProxyNamespaceDefinition<ManagerPolicy>>& getEl() = 0; 
            using ManagerPolicy::ManagerPolicy;
    };

    template <>
    struct ElementInfo<ProxyNamespace> : public DefaultInfo {
        static const bool abstract = false;
    };

    template <class ManagerPolicy>
    class ProxyNamespace : public ProxyNamespaceDefinition<ManagerPolicy> {
        public:
            Set<ProxyNamedElementDefinition, ProxyNamespaceDefinition<ManagerPolicy>> set = Set<ProxyNamedElementDefinition, ProxyNamespaceDefinition<ManagerPolicy>>(this);
            Set<ProxyNamedElementDefinition, ProxyNamespaceDefinition<ManagerPolicy>>& getEl() override {
                return set;
            }
            ProxyNamespace(std::size_t elementType, AbstractManager& manager) :
                ManagerPolicy::manager::BaseElement(elementType, manager),
                ProxyNamespaceDefinition<ManagerPolicy>(elementType, manager)
            {}
    };

    template <class ManagerPolicy>
    class ProxyNamedElement : public ProxyNamedElementDefinition<ManagerPolicy> {
        public:
           ReadOnlySet<ProxyNamespaceDefinition, ProxyNamedElementDefinition<ManagerPolicy>> set = ReadOnlySet<ProxyNamespaceDefinition, ProxyNamedElementDefinition<ManagerPolicy>>(this); 
           ReadOnlySet<ProxyNamespaceDefinition, ProxyNamedElementDefinition<ManagerPolicy>>& getNmspc() override {
                return set;
           }
           static constexpr auto proxyNamedElementElementType = ManagerPolicy::manager::template ElementType<ProxyNamedElement>::result;
           ProxyNamedElement() :
               ManagerPolicy::manager::BaseElement(proxyNamedElementElementType, dummyManager),
               ProxyNamedElementDefinition<ManagerPolicy>(proxyNamedElementElementType, dummyManager)
           {}
           ProxyNamedElement(std::size_t elementType, AbstractManager& manager) :
               ManagerPolicy::manager::BaseElement(elementType, manager),
               ProxyNamedElementDefinition<ManagerPolicy>(elementType, manager)
           {}
    };
    
    using ProxyNamespaceTypes = TemplateTypeList<Element, ProxyNamedElement, ProxyNamespace>;
    using ProxyNamespaceManager = Manager<ProxyNamespaceTypes>;
}

TEST_F(ElementTest, proxyNamespaceTest) {
    ProxyNamespaceManager m;
    auto nmspc1 = m.create<ProxyNamespace>();
    auto nmspc2 = m.create<ProxyNamespace>();
    nmspc1->set.add(nmspc2);
    ASSERT_TRUE(nmspc1->set.contains(nmspc2));
    ASSERT_TRUE(nmspc1->is<ProxyNamedElement>());
    ASSERT_TRUE(nmspc1->is<Element>());
    auto& namedElement = nmspc1->as<ProxyNamedElement>();
    ASSERT_EQ(namedElement, dynamic_cast<ProxyNamedElement<ProxyNamespaceManager::GenBaseHierarchy<ProxyNamedElement>>&>(*nmspc1));
    auto& element = nmspc1->as<Element>();
    ASSERT_EQ(element, dynamic_cast<Element<ProxyNamespaceManager::GenBaseHierarchy<Element>>&>(*nmspc1));
}

namespace UML {
    template <class ManagerPolicy>
    struct Base : public ManagerPolicy {
        using Info = TypeInfo<Base>;
        Base() :
            ManagerPolicy::manager::BaseElement(0, dummyManager),
            ManagerPolicy(0, dummyManager) 
        {}
        Base(std::size_t elementType, AbstractManager& manager) :
            ManagerPolicy::manager::BaseElement(elementType, manager),
            ManagerPolicy(elementType, manager) 
        {}
        virtual ~Base() {}
    };

    template <>
    struct ElementInfo<Base> : DefaultInfo {};

    template <class ManagerPolicy>
    struct Right : public ManagerPolicy {
        using Info = TypeInfo<Right, TemplateTypeList<Base>>;
        Right() :
            ManagerPolicy::manager::BaseElement(0, dummyManager),
            ManagerPolicy(0, dummyManager) 
        {}
        Right(std::size_t elementType, AbstractManager& manager) :
            ManagerPolicy::manager::BaseElement(elementType, manager),
            ManagerPolicy(elementType, manager) 
        {}
        virtual ~Right() {}
    };
    
    template <>
    struct ElementInfo<Right> : DefaultInfo {};

    template <class ManagerPolicy>
    struct Left : public ManagerPolicy {
        using Info = TypeInfo<Left, TemplateTypeList<Base>>;
        Left () :
            ManagerPolicy::manager::BaseElement(0, dummyManager),
            ManagerPolicy(0, dummyManager) 
        {}
        Left (std::size_t elementType, AbstractManager& manager) :
            ManagerPolicy::manager::BaseElement(elementType, manager),
            ManagerPolicy(elementType, manager) 
        {}
        virtual ~Left() {}
    };
    
    template <>
    struct ElementInfo<Left> : DefaultInfo {};

    template <class ManagerPolicy>
    struct Diamond : public ManagerPolicy {
        using Info = TypeInfo<Diamond, TemplateTypeList<Right, Left>>;
        Diamond() :
            ManagerPolicy::manager::BaseElement(0, dummyManager),
            ManagerPolicy(0, dummyManager) 
        {}
        Diamond(std::size_t elementType, AbstractManager& manager) :
            ManagerPolicy::manager::BaseElement(elementType, manager),
            ManagerPolicy(elementType, manager) 
        {}
        virtual ~Diamond() {}
    };
    
    template <>
    struct ElementInfo<Diamond> : DefaultInfo {
        static constexpr bool abstract = false;
    };

    using DiamondTypes = TemplateTypeList<Base, Right, Left, Diamond>;
    using DiamondManager = Manager<DiamondTypes>;
}

TEST_F(ElementTest, diamondElementTest) {
    DiamondManager m;
    auto diamond = m.create<Diamond>();
    auto& right = diamond->as<Right>();
    ASSERT_EQ(right, dynamic_cast<Right<DiamondManager::GenBaseHierarchy<Right>>&>(*diamond));
    auto& left = diamond->as<Left>();
    ASSERT_EQ(left, dynamic_cast<Left<DiamondManager::GenBaseHierarchy<Left>>&>(*diamond));
    auto& base = diamond->as<Base>();
    ASSERT_EQ(base, dynamic_cast<Base<DiamondManager::GenBaseHierarchy<Base>>&>(*diamond));
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

// `TEST_F(ElementTest, AddToSimpleSetTest) {
// `    UmlManager m;
// `    auto& pckg = *m.create<Package>();
// `    auto& child = *m.create<Package>();
// `    pckg.getPackagedElements().add(child);
// `    ASSERT_TRUE(pckg.getPackagedElements().contains(child));
// `    ASSERT_EQ(*pckg.getPackagedElements().front(), child);
// `}
// `
// `TEST_F(ElementTest, simpleOppositeTest) {
// `    UmlManager m;
// `    auto& pckg = *m.create<Package>();
// `    auto& child = *m.create<Package>();
// `    pckg.getPackagedElements().add(child);
// `    ASSERT_TRUE(pckg.getPackagedElements().contains(child));
// `    ASSERT_EQ(*pckg.getPackagedElements().front(), child);
// `    ASSERT_TRUE(child.getOwningPackage());
// `    ASSERT_EQ(*child.getOwningPackage(), pckg);
// `}

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
