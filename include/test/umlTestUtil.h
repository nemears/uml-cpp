#ifndef UML_UML_TEST_UTIL
#define UML_UML_TEST_UTIL

#include "uml/umlManager.h"
#include "uml/sequence.h"
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
        ASSERT_TRUE(el.hasNamespace());
        ASSERT_EQ(el.getNamespaceRef(), nmspc);
        // ASSERT_TRUE(el.getMemberNamespace().count(nmspc.getID()));
        ASSERT_TRUE(el.hasOwner());
        ASSERT_EQ(el.getOwnerRef(), nmspc);
        ASSERT_TRUE(nmspc.getOwnedMembers().count(el.getID()));
        ASSERT_TRUE(nmspc.getMembers().count(el.getID()));
        ASSERT_TRUE(nmspc.getOwnedElements().count(el.getID()));
    }

    inline void ASSERT_RESTORED_OWNING_PACKAGE(PackageableElement& el, Package& pckg) {
        ASSERT_TRUE(el.hasOwningPackage());
        ASSERT_EQ(el.getOwningPackageRef(), pckg);
        ASSERT_TRUE(pckg.getPackagedElements().count(el.getID()));
        ASSERT_RESTORED_NAMESPACE(el, pckg);
    }
}
#endif