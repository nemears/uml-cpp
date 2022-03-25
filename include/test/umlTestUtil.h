#ifndef UML_UML_TEST_UTIL
#define UML_UML_TEST_UTIL

#include "uml/umlManager.h"
#include "uml/package.h"

namespace UML {
    namespace {

        template <class T = Element, class U = Element> void ASSERT_COPY_SEQUENCE_CORRECTLY(size_t index, T& og, T& copy) {};
        template <class T = Element, class U = Element> void ASSERT_COPY_SINGLETON_CORRECTLY(size_t index, T& og, T& copy) {};
        void ASSERT_PROPER_MOUNT_HELPER(Element& el, std::string parentPath) {
            // ASSERT_TRUE(std::filesystem::exists(parentPath + "/" + el.getID().string()));
            ASSERT_TRUE(std::filesystem::exists(parentPath + "/" + el.getID().string() + ".yml"));
            for (auto& child : el.getOwnedElements()) {
                ASSERT_PROPER_MOUNT_HELPER(child, parentPath);
            }
        };

    }

    template <class T = Element, class U = Element, typename S, typename... Ss>
    void ASSERT_COPY_SEQUENCE_CORRECTLY(size_t index, T& og, T& copy, S seq, Ss... sequences) {
        ASSERT_EQ((og.*seq)().size(), (copy.*seq)().size()) << " at index " << index;
        for (size_t i = 0; i < (og.*seq)().size(); i++) {
            ASSERT_TRUE((copy.*seq)().contains((og.*seq)().get(i))) << " at index " << index;
        }
        index++;
        ASSERT_COPY_SEQUENCE_CORRECTLY<T, U>(index, og, copy, sequences...);
    };

    template <class T = Element, class U = Element, typename S, typename... Ss>
    void ASSERT_COPY_SEQUENCE_CORRECTLY(T& og, T& copy, S seq, Ss... sequences) {
        ASSERT_COPY_SEQUENCE_CORRECTLY<T, U>(0, og, copy, seq, sequences...);
    };

    template <class T = Element, class U = Element, typename F, typename... Fs>
    void ASSERT_COPY_SINGLETON_CORRECTLY(size_t index, T& og, T& copy, F func, Fs... funcs) {
        if ((og.*func)()) {
            ASSERT_TRUE((copy.*func)());
            ASSERT_EQ((og.*func)()->getID(), (copy.*func)()->getID());
        }
        else {
            ASSERT_TRUE((copy.*func)() == 0);
        }
        ASSERT_COPY_SINGLETON_CORRECTLY(index, og, copy, funcs...);
    };

    template <class T = Element, class U = Element, typename F, typename... Fs>
    void ASSERT_COPY_SINGLETON_CORRECTLY(T& og, T& copy, F func, Fs... funcs) {
        ASSERT_COPY_SINGLETON_CORRECTLY(0, og, copy, funcs...);
    };

    inline void ASSERT_PROPER_MOUNT(Element& root, std::string mountPath) {
        std::string mountPath1 = mountPath + "/mount";
        ASSERT_TRUE(std::filesystem::exists(mountPath1));
        ASSERT_PROPER_MOUNT_HELPER(root, mountPath1);
    }

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

    template <class V, class W, class T = Element, class U = Element>
    void setIntegrationTest(Set<T,U>& (U::*acessor)()) {
        UmlManager m;
        UmlPtr<W> u = m.create<W>();
        UmlPtr<V> t = m.create<V>();
        ASSERT_NO_THROW(((*u).*acessor)().add(*t));
        ASSERT_EQ(((*u).*acessor)().size(), 1);
        ASSERT_EQ(((*u).*acessor)().front(), *t);
        if (t->isSubClassOf(ElementType::NAMED_ELEMENT)) {
            t->setName("name");
            ASSERT_NO_THROW(((*u).*acessor)().get("name"));
            ASSERT_EQ(((*u).*acessor)().get("name"), *t);
            t->setName("test");
            ASSERT_NO_THROW(((*u).*acessor)().get("test"));
            ASSERT_EQ(((*u).*acessor)().get("test"), *t);
        }
        ID id = ID::randomID();
        t->setID(id);
        ASSERT_NO_THROW(((*u).*acessor)().get(id));
        ASSERT_EQ(((*u).*acessor)().get(id), *t);
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
        ASSERT_EQ(((*u).*acessor)().size(), 0);
    };

    #define UML_SET_INTEGRATION_TEST(TEST_NAME, T, U, signature) \
    class TEST_NAME ## _integrationTest : public ::testing::Test {}; \
    TEST_F( TEST_NAME ## _integrationTest, setAndGetTest) { \
        ASSERT_NO_FATAL_FAILURE((setIntegrationTest<T , U>(signature)));\
    }

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
    class TEST_NAME ## _integrationTest : public ::testing::Test {}; \
    TEST_F(TEST_NAME ## _integrationTest, setAndGetTest) { \
        ASSERT_NO_FATAL_FAILURE((singletonIntegrationTest<T,U>(acessor, mutator))); \
    }
    
}
#endif