#ifndef _UML_UML_TEST_UTIL_H_
#define _UML_UML_TEST_UTIL_H_

#include "uml/managers/umlClient.h"
#include "uml/package.h"

namespace UML {

    inline void ASSERT_RESTORED_NAMESPACE(NamedElement& el, Namespace& nmspc) {
        ASSERT_TRUE(el.getNamespace());
        ASSERT_EQ(*el.getNamespace(), nmspc);
        // ASSERT_TRUE(el.getMemberNamespace().count(nmspc.getID()));
        ASSERT_TRUE(el.getOwner());
        ASSERT_EQ(*el.getOwner(), nmspc);
        ASSERT_TRUE(nmspc.getOwnedMembers().count(el.getID()));
        ASSERT_TRUE(nmspc.getMembers().count(el.getID()));
        ASSERT_TRUE(nmspc.getOwnedElements().count(el.getID()));
    }

    inline void ASSERT_RESTORED_OWNING_PACKAGE(PackageableElement& el, Package& pckg) {
        ASSERT_TRUE(el.getOwningPackage());
        ASSERT_EQ(*el.getOwningPackage(), pckg);
        ASSERT_TRUE(pckg.getPackagedElements().count(el.getID()));
        ASSERT_RESTORED_NAMESPACE(el, pckg);
    }

    inline void recursiveSetContains(ID id, AbstractSet* set) {
        ASSERT_TRUE(set->contains(id));
        for (auto& supserSet : set->m_superSets) {
            ASSERT_NO_FATAL_FAILURE(recursiveSetContains(id, supserSet));
        }
    };

    template <class V, class W, class T = Element, class U = Element, class S = Set<T,U>>
    void setIntegrationTestBasic(S& (U::*acessor)()) {
        UmlManager m;
        UmlPtr<W> u = m.create<W>();
        UmlPtr<V> t = m.create<V>();
        ASSERT_NO_THROW(((*u).*acessor)().add(*t));
        ASSERT_EQ(((*u).*acessor)().size(), 1);
        ASSERT_EQ(((*u).*acessor)().front(), *t);
        ASSERT_NO_FATAL_FAILURE(recursiveSetContains(t.id(), &((*u).*acessor)()));
        ASSERT_NO_THROW(((*u).*acessor)().remove(*t));
        ASSERT_EQ(((*u).*acessor)().size(), 0);
        UmlPtr<V> t2 = m.create<V>();
        ASSERT_NO_THROW(((*u).*acessor)().add(*t2));
        ASSERT_NO_THROW(((*u).*acessor)().add(*t));
        ASSERT_EQ(((*u).*acessor)().size(), 2);
        ASSERT_TRUE(((*u).*acessor)().contains(*t2));
        ASSERT_TRUE(((*u).*acessor)().contains(*t));
    };

    template <class V, class W, class T = Element, class U = Element, class S = Set<T,U>>
    void setIntegrationTestReindex(S& (U::*acessor)()) {
        UmlManager m;
        UmlPtr<W> u = m.create<W>();
        UmlPtr<V> t = m.create<V>();
        ASSERT_NO_THROW(((*u).*acessor)().add(*t));
        if (t->isSubClassOf(ElementType::NAMED_ELEMENT)) {
            t->template as<NamedElement>().setName("name");
            ASSERT_NO_THROW(((*u).*acessor)().get("name"));
            ASSERT_EQ(((*u).*acessor)().get("name"), *t);
            t->template as<NamedElement>().setName("test");
            ASSERT_NO_THROW(((*u).*acessor)().get("test"));
            ASSERT_EQ(((*u).*acessor)().get("test"), *t);
        }
        ID id = ID::randomID();
        t->setID(id);
        ASSERT_NO_THROW(((*u).*acessor)().get(id));
        ASSERT_EQ(((*u).*acessor)().get(id), *t);
    };

    template <class V, class W, class T = Element, class U = Element, class S = Set<T,U>>
    void setIntegrationTestMount(S& (U::*acessor)()) {
        UmlManager m;
        UmlPtr<W> u = m.create<W>();
        UmlPtr<V> t = m.create<V>();
        UmlPtr<V> t2 = m.create<V>();
        ASSERT_NO_THROW(((*u).*acessor)().add(*t));
        m.setRoot(*u);
        m.mount(".");
        u.release();
        ASSERT_FALSE(u.loaded());
        ASSERT_EQ(((*u).*acessor)().front(), *t);
        ASSERT_EQ(&((*u).*acessor)().front(), t.ptr());
        t.release();
        ASSERT_FALSE(t.loaded());
        ASSERT_EQ(((*u).*acessor)().front(), *t);
        ASSERT_NO_THROW(((*u).*acessor)().remove(*t));
        ASSERT_NO_THROW(((*u).*acessor)().add(*t2));
        ASSERT_NO_THROW(((*u).*acessor)().add(*t));
        ASSERT_EQ(((*u).*acessor)().size(), 2);
        ASSERT_TRUE(((*u).*acessor)().contains(*t2));
        ASSERT_TRUE(((*u).*acessor)().contains(*t));
        ASSERT_EQ(((*u).*acessor)().size(), 2);
    };

    template <class V, class W, class T = Element, class U = Element, class S = Set<T,U>>
    void setIntegrationTestClientServer(S& (U::*acessor)()) { // TODO test this one a lil more thouroghly
        UmlClient m;
        UmlPtr<W> u = m.create<W>();
        UmlPtr<V> t = m.create<V>();
        UmlPtr<V> t2 = m.create<V>();
        ASSERT_NO_THROW(m.get(u.id()));
        ASSERT_NO_THROW(m.get(t.id()));
        ASSERT_NO_THROW(((*u).*acessor)().add(*t));
        u.release();
        // m.put(*u);
        // ASSERT_FALSE(u.loaded());
        ASSERT_EQ(((*u).*acessor)().front(), *t);
        ASSERT_EQ(&((*u).*acessor)().front(), t.ptr());
        m.put(*t);
        // ASSERT_FALSE(t.loaded());
        ASSERT_EQ(((*u).*acessor)().front(), *t);
        ASSERT_NO_THROW(((*u).*acessor)().remove(*t));
        ASSERT_NO_THROW(((*u).*acessor)().add(*t2));
        ASSERT_NO_THROW(((*u).*acessor)().add(*t));
        ASSERT_EQ(((*u).*acessor)().size(), 2);
        ASSERT_TRUE(((*u).*acessor)().contains(*t2));
        ASSERT_TRUE(((*u).*acessor)().contains(*t));
        ASSERT_EQ(((*u).*acessor)().size(), 2);
    };

    template <class V, class W, class T = Element, class U = Element, class S = Set<T,U>>
    void setIntegrationTestErase(S& (U::*acessor)()) {
        UmlManager m;
        UmlPtr<W> u = m.create<W>();
        UmlPtr<V> t = m.create<V>();
        ASSERT_NO_THROW(((*u).*acessor)().add(*t));
        m.erase(*t);
        ASSERT_EQ(((*u).*acessor)().size(), 0);
        ASSERT_FALSE(m.loaded(t.id()));
    };

    #define UML_SET_INTEGRATION_TEST(TEST_NAME, T, U, signature) \
    class TEST_NAME ## Method : public ::testing::Test { \
        void SetUp() override { \
            std::hash<std::string> hasher; \
            srand(static_cast<unsigned int>(time(0)) ^ hasher(std::string(#TEST_NAME))); \
        }; \
    }; \
    TEST_F( TEST_NAME ## Method , basicIntegrationTest ) { \
        ASSERT_NO_FATAL_FAILURE((setIntegrationTestBasic<T , U>(signature)));\
    } \
    TEST_F( TEST_NAME ## Method , reindexIntegrationTest ) { \
        ASSERT_NO_FATAL_FAILURE((setIntegrationTestReindex<T , U>(signature)));\
    } \
    TEST_F( TEST_NAME ## Method , mountAndReleaseIntegrationTest ) { \
        ASSERT_NO_FATAL_FAILURE((setIntegrationTestMount<T , U>(signature)));\
    } \
    TEST_F( TEST_NAME ## Method , clientServerIntegrationTest ) { \
        ASSERT_NO_FATAL_FAILURE((setIntegrationTestClientServer<T , U>(signature)));\
    } //\
    // TEST_F( TEST_NAME ## Method , eraseIntegrationTest ) { \
    //     ASSERT_NO_FATAL_FAILURE((setIntegrationTestErase<T , U>(signature)));\
    // }

    template <class V, class W, class T = Element, class U = Element>
    void singletonIntegrationTest(UmlPtr<T> (U::*acessor)() const, void (U::*mutator)(T*)) {
        UmlManager m;
        UmlPtr<W> u = m.create<W>();
        UmlPtr<V> t = m.create<V>();
        ASSERT_NO_THROW(((*u).*mutator)(t.ptr()));
        ASSERT_EQ(((*u).*acessor)(), t);
        m.setRoot(*u);
        m.mount(".");
        u.release();
        ASSERT_EQ(((*u).*acessor)(), t);
        t.release();
        ASSERT_EQ(*((*u).*acessor)(), *t);
        ASSERT_NO_THROW(((*u).*mutator)(0));
        ASSERT_FALSE(((*u).*acessor)());
    }

    #define UML_SINGLETON_INTEGRATION_TEST(TEST_NAME, T, U, acessor, mutator) \
    class TEST_NAME ## Method : public ::testing::Test {}; \
    TEST_F(TEST_NAME ## Method , integrationTest) { \
        ASSERT_NO_FATAL_FAILURE((singletonIntegrationTest<T,U>(acessor, mutator))); \
    }
    
}
#endif