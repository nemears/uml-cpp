#pragma once

#include "uml/uml-stable.h"
#define YML_FILES_PATH "../examples/" 

namespace UML {

    inline void ASSERT_RESTORED_NAMESPACE(UmlManager::Implementation<NamedElement>& el, UmlManager::Implementation<Namespace>& nmspc) {
        ASSERT_TRUE(el.getNamespace());
        ASSERT_EQ(*el.getNamespace(), nmspc);
        // ASSERT_TRUE(el.getMemberNamespace().count(nmspc.getID()));
        ASSERT_TRUE(el.getOwner());
        ASSERT_EQ(*el.getOwner(), nmspc);
        ASSERT_TRUE(nmspc.getOwnedMembers().contains(el.getID()));
        ASSERT_TRUE(nmspc.getMembers().contains(el.getID()));
        ASSERT_TRUE(nmspc.getOwnedElements().contains(el.getID()));
    }

    inline void ASSERT_RESTORED_OWNING_PACKAGE(UmlManager::Implementation<PackageableElement>& el, UmlManager::Implementation<Package>& pckg) {
        ASSERT_TRUE(el.getOwningPackage());
        ASSERT_EQ(*el.getOwningPackage(), pckg);
        ASSERT_TRUE(pckg.getPackagedElements().contains(el.getID()));
        ASSERT_RESTORED_NAMESPACE(el, pckg);
    }

    // inline void recursiveSetContains(EGM::ID id, EGM::AbstractSet* set) {
    //     ASSERT_TRUE(set->contains(id));
    //     for (auto& supserSet : set->m_superSets) {
    //         ASSERT_NO_FATAL_FAILURE(recursiveSetContains(id, supserSet));
    //     }
    // };

    template <template <class> class V, template <class> class W, template <class> class U, class S>
    void setIntegrationTestBasic(S& (U<typename UmlManager::template GenBaseHierarchy<U>>::*acessor)()) {
        UmlManager m;
        auto u = m.create<W>();
        auto t = m.create<V>();
        ASSERT_NO_THROW(((*u).*acessor)().add(*t));
        ASSERT_EQ(((*u).*acessor)().size(), 1);
        ASSERT_EQ(((*u).*acessor)().front(), t);
        // ASSERT_NO_FATAL_FAILURE(recursiveSetContains(t.id(), &((*u).*acessor)()));
        ASSERT_NO_THROW(((*u).*acessor)().remove(*t));
        ASSERT_EQ(((*u).*acessor)().size(), 0);
        auto t2 = m.create<V>();
        ASSERT_NO_THROW(((*u).*acessor)().add(*t2));
        ASSERT_NO_THROW(((*u).*acessor)().add(*t));
        ASSERT_EQ(((*u).*acessor)().size(), 2);
        ASSERT_TRUE(((*u).*acessor)().contains(*t2));
        ASSERT_TRUE(((*u).*acessor)().contains(*t));
    }

    template <template <class> class V, template <class> class W, template <class> class U, class S>
    void setIntegrationTestReindex(S& (U<typename UmlManager::template GenBaseHierarchy<U>>::*acessor)()) {
        UmlManager m;
        auto u = m.create<W>();
        auto t = m.create<V>();
        ASSERT_NO_THROW(((*u).*acessor)().add(*t));
        if (t->template is<NamedElement>()) {
            t->template as<NamedElement>().setName("name");
            ASSERT_NO_THROW(((*u).*acessor)().get("name"));
            ASSERT_EQ(((*u).*acessor)().get("name"), *t);
            t->template as<NamedElement>().setName("test");
            ASSERT_NO_THROW(((*u).*acessor)().get("test"));
            ASSERT_EQ(((*u).*acessor)().get("test"), *t);
        }
        EGM::ID id = EGM::ID::randomID();
        t->setID(id);
        ASSERT_NO_THROW(((*u).*acessor)().get(id));
        ASSERT_EQ(((*u).*acessor)().get(id), *t);
    }

    template <template <class> class V, template <class> class W, template <class> class U , class S>
    void setIntegrationTestMount(S& (U<typename UmlManager::template GenBaseHierarchy<U>>::*acessor)()) {
        UmlManager m;
        auto u = m.create<W>();
        auto t = m.create<V>();
        auto t2 = m.create<V>();
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

    template <template <class> class V, template <class> class W, template <class> class U, class S>
    void setIntegrationTestErase(S& (U<typename UmlManager::template GenBaseHierarchy<U>>::*acessor)()) {
        UmlManager m;
        auto u = m.create<W>();
        auto t = m.create<V>();
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
        ASSERT_NO_FATAL_FAILURE((setIntegrationTestBasic<T , U>(&U<UmlManager::GenBaseHierarchy<U>>::signature)));\
    } \
    TEST_F( TEST_NAME ## Method , mountSetAndReleaseIntegrationTest ) { \
        ASSERT_NO_FATAL_FAILURE((setIntegrationTestMount<T , U>(&U<UmlManager::GenBaseHierarchy<U>>::signature)));\
    } \
    TEST_F( TEST_NAME ## Method , eraseSetIntegrationTest ) { \
        ASSERT_NO_FATAL_FAILURE((setIntegrationTestErase<T , U>(&U<UmlManager::GenBaseHierarchy<U>>::signature)));\
    }

    template <template <class> class V, template <class> class W, template <class> class U, class S>
    void singletonIntegrationTestBasic(S& (U<typename UmlManager::template GenBaseHierarchy<U>>::*acessor)()) {
        UmlManager m;
        auto u = m.create<W>();
        auto t = m.create<V>();
        auto t2 = m.create<V>();
        ASSERT_NO_THROW(((*u).*acessor)().set(t));
        ASSERT_EQ(((*u).*acessor)().get(), t);
        ASSERT_NO_THROW(((*u).*acessor)().set(0));
        ASSERT_FALSE(((*u).*acessor)().get());
        ((*u).*acessor)().set(t);
        ASSERT_NO_THROW(((*u).*acessor)().set(t2));
        ASSERT_EQ(*((*u).*acessor)().get(), *t2);
    }

    template <template <class> class V, template <class> class W, template <class> class U, class S>
    void singletonIntegrationTestReindex(S& (U<typename UmlManager::template GenBaseHierarchy<U>>::*acessor)()) {
        UmlManager m;
        auto u = m.create<W>();
        auto t = m.create<V>();
        ((*u).*acessor)().set(t);
        EGM::ID newID = EGM::ID::randomID();
        t->setID(newID);
        ASSERT_EQ(((*u).*acessor)().get().id(), newID);
        // TODO name?
    }

    template <template <class> class V, template <class> class W, template <class> class U, class S>
    void singletonIntegrationTestMount(S& (U<typename UmlManager::template GenBaseHierarchy<U>>::*acessor)()) {
        UmlManager m;
        auto u = m.create<W>();
        auto t = m.create<V>();
        m.setRoot(u);
        m.mount(".");
        ASSERT_NO_THROW(((*u).*acessor)().set(t));
        ASSERT_EQ(((*u).*acessor)().get(), t);
        u.release();
        ASSERT_EQ(((*u).*acessor)().get(), t);
        t.release();
        ASSERT_EQ(((*u).*acessor)().get(), t);
        u.release();
        t.release();
        m.erase(*t);
        ASSERT_FALSE(((*u).*acessor)().get());
        auto t2 = m.create<V>();
        ASSERT_NO_THROW(((*u).*acessor)().set(t2));
        u.release();
        t2.release();
        m.erase(*u);
        ASSERT_NO_THROW(t2.aquire());
        ASSERT_NO_THROW(t2.release());
    }

    template <template <class> class V, template <class> class W, template <class> class U, class S>
    void singletonIntegrationTestErase(S& (U<typename UmlManager::template GenBaseHierarchy<U>>::*acessor)()) {
        UmlManager m;
        auto u = m.create<W>();
        auto t = m.create<V>();
        ASSERT_NO_THROW(((*u).*acessor)().set(t));
        ASSERT_NO_THROW(m.erase(*t));
        ASSERT_FALSE(((*u).*acessor)().get());
    }

    #define UML_SINGLETON_INTEGRATION_TEST(TEST_NAME, T, U, acessor) \
    class TEST_NAME ## Method : public ::testing::Test { \
        void SetUp() override { \
            std::hash<std::string> hasher; \
            srand(static_cast<unsigned int>(time(0)) ^ hasher(std::string(#TEST_NAME))); \
        }; \
    }; \
    TEST_F(TEST_NAME ## Method , basicSingletonIntegrationTest) { \
        ASSERT_NO_FATAL_FAILURE((singletonIntegrationTestBasic<T,U>(&U<UmlManager::GenBaseHierarchy<U>>::acessor))); \
    } \
    TEST_F(TEST_NAME ## Method, mountSingletonIntegrationTest) { \
        ASSERT_NO_FATAL_FAILURE((singletonIntegrationTestMount<T,U>(&U<UmlManager::GenBaseHierarchy<U>>::acessor))); \
    } \
    TEST_F(TEST_NAME ## Method, eraseSingletonIntegrationTest) { \
        ASSERT_NO_FATAL_FAILURE((singletonIntegrationTestErase<T,U>(&U<UmlManager::GenBaseHierarchy<U>>::acessor))); \
    }
    
}
