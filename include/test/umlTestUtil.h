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

//     template <class T = Element, class U = Element>
//     struct SingletonWrapper {
//         UmlPtr<T> (U::*m_getter)() const;
//         void (U::*m_setter)(T&);
//     };

//     template <class T, class U, typename F>
//     struct setAndGetIntegrationTest {
//         setAndGetIntegrationTest(F f);
//         void operator()() const;
//     };

//     template <class T, class U>
//     struct setAndGetIntegrationTest<T, U, Set<T,U>& (U::*)()> {
//         Set<T,U>& (U::*f)();
//         setAndGetIntegrationTest(Set<T,U>& (U::*ff)()) : f(ff) {};
//         void operator()() const {
//             UmlManager m;
//             UmlPtr<U> u = m.create<U>();
//             UmlPtr<T> t = m.create<T>();
//             ASSERT_NO_THROW((u->*f)().add(*t));
//             ASSERT_EQ((u->*f)().size(), 1);
//             ASSERT_EQ((u->*f)().front(), *t);
//         };
//     };

//     template <class T, class U>
//     struct setAndGetIntegrationTest<T, U, SingletonWrapper<T,U>> {
//         UmlPtr<T> (U::*m_getter)();
//         void (U::*m_setter)(T&);
//         setAndGetIntegrationTest(SingletonWrapper<T,U> wrapper) : m_getter(wrapper.m_getter) , m_setter(wrapper.m_setter) {};
//         void operator()() const {
//             UmlManager m;
//             UmlPtr<U> u = m.create<U>();
//             UmlPtr<T> t = m.create<T>();
//             ASSERT_NO_THROW((u->*m_setter)(*t));
//             ASSERT_EQ((u->*m_getter)(), t);
//         };
//     };

//    #define UML_INTEGRATION_TEST( ELEMENT_NAME, T, U, signature) \
//     class ELEMENT_NAME ## _integrationTest : public ::testing::Test {}; \
//     TEST_F( ELEMENT_NAME ## _integrationTest , setAndGetTest) { \
//         setAndGetIntegrationTest<T, U> testFunctor(signature); \
//         ASSERT_NO_FATAL_FAILURE(testFunctor()); \
//     }

    template <class T = Element, class U = Element>
    void setIntegrationTest(Set<T,U>& (U::*acessor)()) {
        UmlManager m;
        UmlPtr<U> u = m.create<U>();
        UmlPtr<S> t = m.create<S>();
        ASSERT_NO_THROW(((*u).*acessor)().add(*t));
        ASSERT_EQ(((*u).*acessor)().size(), 1);
        ASSERT_EQ(((*u).*acessor)().front(), *t);
        if (t->isSubClassOf(ElementType::NAMED_ELEMENT)) {
            t->setName("name");
            ASSERT_NO_THROW(((*u).*acessor)().get("name"));
            ASSERT_EQ(((*u).*acessor)().get("name"), *t);
        }
        ID id = ID::randomID();
        t->setID(id);
        ASSERT_NO_THROW(((*u).*acessor)().get(id));
        ASSERT_EQ(((*u).*acessor)().get(id), *t);
        m.setRoot(*u);
        m.mount(".");
        u.release();
        ASSERT_EQ(((*u).*acessor)().front(), *t);
        t.release();
        ASSERT_EQ(((*u).*acessor)().front(), *t);
    };

    #define UML_SET_INTEGRATION_TEST(TEST_NAME, signature) \
    class TEST_NAME ## _integrationTest : public ::testing::Test {}; \
    TEST_F( TEST_NAME ## _integrationTest, setAndGetTest) { \
        ASSERT_NO_FATAL_FAILURE(setIntegrationTest(signature));\
    }
    
}
#endif