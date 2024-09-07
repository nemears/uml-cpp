#pragma once

#include "uml/uml-stable.h"

namespace UML {

    inline void ASSERT_RESTORED_NAMESPACE(NamedElement& el, Namespace& nmspc) {
        ASSERT_TRUE(el.getNamespace());
        ASSERT_EQ(*el.getNamespace(), nmspc);
        // ASSERT_TRUE(el.getMemberNamespace().count(nmspc.getID()));
        ASSERT_TRUE(el.getOwner());
        ASSERT_EQ(*el.getOwner(), nmspc);
        ASSERT_TRUE(nmspc.getOwnedMembers().contains(el.getID()));
        ASSERT_TRUE(nmspc.getMembers().contains(el.getID()));
        ASSERT_TRUE(nmspc.getOwnedElements().contains(el.getID()));
    }

    inline void ASSERT_RESTORED_OWNING_PACKAGE(PackageableElement& el, Package& pckg) {
        ASSERT_TRUE(el.getOwningPackage());
        ASSERT_EQ(*el.getOwningPackage(), pckg);
        ASSERT_TRUE(pckg.getPackagedElements().contains(el.getID()));
        ASSERT_RESTORED_NAMESPACE(el, pckg);
    }

    // inline void recursiveSetContains(ID id, AbstractSet* set) {
    //     ASSERT_TRUE(set->contains(id));
    //     for (auto& supserSet : set->m_superSets) {
    //         ASSERT_NO_FATAL_FAILURE(recursiveSetContains(id, supserSet));
    //     }
    // };

    template <class V, class W, class U, class S>
    void setIntegrationTestBasic(S& (U::*acessor)()) {
        UmlManager m;
        UmlPtr<W> u = m.create<W>();
        UmlPtr<V> t = m.create<V>();
        ASSERT_NO_THROW(((*u).*acessor)().add(*t));
        ASSERT_EQ(((*u).*acessor)().size(), 1);
        ASSERT_EQ(((*u).*acessor)().front(), t);
        // ASSERT_NO_FATAL_FAILURE(recursiveSetContains(t.id(), &((*u).*acessor)()));
        ASSERT_NO_THROW(((*u).*acessor)().remove(*t));
        ASSERT_EQ(((*u).*acessor)().size(), 0);
        UmlPtr<V> t2 = m.create<V>();
        ASSERT_NO_THROW(((*u).*acessor)().add(*t2));
        ASSERT_NO_THROW(((*u).*acessor)().add(*t));
        ASSERT_EQ(((*u).*acessor)().size(), 2);
        ASSERT_TRUE(((*u).*acessor)().contains(*t2));
        ASSERT_TRUE(((*u).*acessor)().contains(*t));
    }

    template <class V, class W, class U = Element, class S>
    void setIntegrationTestReindex(S& (U::*acessor)()) {
        UmlManager m;
        UmlPtr<W> u = m.create<W>();
        UmlPtr<V> t = m.create<V>();
        ASSERT_NO_THROW(((*u).*acessor)().add(*t));
        if (t->template is<NamedElement>()) {
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
    }

    template <class V, class W, class U , class S>
    void setIntegrationTestMount(S& (U::*acessor)()) {
        UmlManager m;
        UmlPtr<W> u = m.create<W>();
        UmlPtr<V> t = m.create<V>();
        UmlPtr<V> t2 = m.create<V>();
        ASSERT_NO_THROW(((*u).*acessor)().add(*t));
        m.setRoot(u);
        m.mount(".");
        u.release();
        ASSERT_FALSE(u.loaded());
        ASSERT_EQ(((*u).*acessor)().front(), t);
        ASSERT_EQ(((*u).*acessor)().front().ptr(), t.ptr());
        t.release();
        ASSERT_FALSE(t.loaded());
        ASSERT_EQ(((*u).*acessor)().front(), t);
        ASSERT_NO_THROW(((*u).*acessor)().remove(*t));
        ASSERT_NO_THROW(((*u).*acessor)().add(*t2));
        ASSERT_NO_THROW(((*u).*acessor)().add(*t));
        ASSERT_EQ(((*u).*acessor)().size(), 2);
        ASSERT_TRUE(((*u).*acessor)().contains(*t2));
        ASSERT_TRUE(((*u).*acessor)().contains(*t));
        ASSERT_EQ(((*u).*acessor)().size(), 2);
    }

    template <class V, class W, class U, class S>
    void setIntegrationTestErase(S& (U::*acessor)()) {
        UmlManager m;
        UmlPtr<W> u = m.create<W>();
        UmlPtr<V> t = m.create<V>();
        ASSERT_NO_THROW(((*u).*acessor)().add(*t));
        m.erase(*t);
        ASSERT_EQ(((*u).*acessor)().size(), 0);
        ASSERT_FALSE(m.loaded(t.id()));
    }

    #define UML_SET_INTEGRATION_TEST(TEST_NAME, T, U, signature) \
    class TEST_NAME ## Method : public ::testing::Test { \
        void SetUp() override { \
            std::hash<std::string> hasher; \
            srand(static_cast<unsigned int>(time(0)) ^ hasher(std::string(#TEST_NAME))); \
        }; \
    }; \
    TEST_F( TEST_NAME ## Method , basicSetIntegrationTest ) { \
        ASSERT_NO_FATAL_FAILURE((setIntegrationTestBasic<T , U>(signature)));\
    } \
    TEST_F( TEST_NAME ## Method , mountSetAndReleaseIntegrationTest ) { \
        ASSERT_NO_FATAL_FAILURE((setIntegrationTestMount<T , U>(signature)));\
    } \
    TEST_F( TEST_NAME ## Method , eraseSetIntegrationTest ) { \
        ASSERT_NO_FATAL_FAILURE((setIntegrationTestErase<T , U>(signature)));\
    }

    template <class V, class W, class T = Element, class U = Element>
    void singletonIntegrationTestBasic(UmlPtr<T> (U::*acessor)() const, void (U::*mutator)(UmlPtr<T>)) {
        UmlManager m;
        UmlPtr<W> u = m.create<W>();
        UmlPtr<V> t = m.create<V>();
        UmlPtr<V> t2 = m.create<V>();
        ASSERT_NO_THROW(((*u).*mutator)(t));
        ASSERT_EQ(((*u).*acessor)(), t);
        ASSERT_NO_THROW(((*u).*mutator)(0));
        ASSERT_FALSE(((*u).*acessor)());
        ((*u).*mutator)(t);
        ASSERT_NO_THROW(((*u).*mutator)(t2));
        ASSERT_EQ(*((*u).*acessor)(), *t2);
    }

    template <class V, class W, class T = Element, class U = Element>
    void singletonIntegrationTestReindex(UmlPtr<T> (U::*acessor)() const, void (U::*mutator)(UmlPtr<T>)) {
        UmlManager m;
        UmlPtr<W> u = m.create<W>();
        UmlPtr<V> t = m.create<V>();
        ((*u).*mutator)(t);
        ID newID = ID::randomID();
        t->setID(newID);
        ASSERT_EQ(((*u).*acessor)().id(), newID);
        // TODO name?
    }

    template <class V, class W, class T = Element, class U = Element>
    void singletonIntegrationTestMount(UmlPtr<T> (U::*acessor)() const, void (U::*mutator)(UmlPtr<T>)) {
        UmlManager m;
        UmlPtr<W> u = m.create<W>();
        UmlPtr<V> t = m.create<V>();
        m.setRoot(u);
        m.mount(".");
        ASSERT_NO_THROW(((*u).*mutator)(t));
        ASSERT_EQ(((*u).*acessor)(), t);
        u.release();
        ASSERT_EQ(((*u).*acessor)(), t);
        t.release();
        ASSERT_EQ(((*u).*acessor)(), t);
        u.release();
        t.release();
        m.erase(*t);
        ASSERT_FALSE(((*u).*acessor)());
        UmlPtr<V> t2 = m.create<V>();
        ASSERT_NO_THROW(((*u).*mutator)(t2));
        u.release();
        t2.release();
        m.erase(*u);
        ASSERT_NO_THROW(t2.aquire());
        ASSERT_NO_THROW(t2.release());
    }

    template <class V, class W, class T = Element, class U = Element>
    void singletonIntegrationTestErase(UmlPtr<T> (U::*acessor)() const, void (U::*mutator)(UmlPtr<T>)) {
        UmlManager m;
        UmlPtr<W> u = m.create<W>();
        UmlPtr<V> t = m.create<V>();
        ASSERT_NO_THROW(((*u).*mutator)(t));
        ASSERT_NO_THROW(m.erase(*t));
        ASSERT_FALSE(((*u).*acessor)());
    }

    #define UML_SINGLETON_INTEGRATION_TEST(TEST_NAME, T, U, acessor, mutator) \
    class TEST_NAME ## Method : public ::testing::Test { \
        void SetUp() override { \
            std::hash<std::string> hasher; \
            srand(static_cast<unsigned int>(time(0)) ^ hasher(std::string(#TEST_NAME))); \
        }; \
    }; \
    TEST_F(TEST_NAME ## Method , basicSingletonIntegrationTest) { \
        ASSERT_NO_FATAL_FAILURE((singletonIntegrationTestBasic<T,U>(acessor, mutator))); \
    } \
    TEST_F(TEST_NAME ## Method, mountSingletonIntegrationTest) { \
        ASSERT_NO_FATAL_FAILURE((singletonIntegrationTestMount<T,U>(acessor, mutator))); \
    } \
    TEST_F(TEST_NAME ## Method, eraseSingletonIntegrationTest) { \
        ASSERT_NO_FATAL_FAILURE((singletonIntegrationTestErase<T,U>(acessor, mutator))); \
    }
    
}
