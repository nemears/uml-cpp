#pragma once

#include "egm/egm-basic.h"
#include "uml/types/namedElement.h"


// This is just an idea, there are other ways to implement this
// the main reason for this is to have minimally reproducible set type 
// with a NamedElementPtr get(string) const; method
// there are other approaches we could take, see note below

namespace UML {

    // typedef ManagedPtr<NamedElement> NamedElementPtr;
    
    template <class ManagerPolicy>
    class IndexablePolicy : public EGM::DoNothingPolicy , virtual public EGM::AbstractSet {
        using NamedElementPtr = EGM::ManagedPtr<NamedElement<typename ManagerPolicy::manager::template GenBaseHierarchy<NamedElement>>>;
        public:
            NamedElementPtr get(std::string qualifiedName) const {
                auto it = this->beginPtr();
                while (*it != *this->endPtr()) {
                    auto curr = it->getCurr();
                    try {
                        NamedElementPtr namedCurr = curr;
                        auto elQualifiedName = namedCurr->getQualifiedName();
                        auto elName = namedCurr->getName();
                        if (elQualifiedName == qualifiedName) {
                            return curr;
                        }
                        if (elName == qualifiedName) {
                            return curr;
                        }
                    } catch (std::exception& e) {}
                    it->next();
                }
                return EGM::AbstractElementPtr();
            }
    };


    // soft binding , 
    // Note: could avoid this with compile time subsets and redefines to handle api policies so methods
    // of subsetted and redefined set polices would be inherited, figure out how easy that is to implement?
    // something like this Set<NamedElement, Namespace, [ *m_members, *m_ownedElements ], []> m_ownedMembers ...
    // where the third template argument is subsets, second redefinition, and then an optional policy on the end
    template <template <class> class T, class U, class ApiPolicy = IndexablePolicy<U>>
    class ReadOnlyIndexableSet : public EGM::ReadOnlySet<T, U, ApiPolicy> {
        public:
            using ManagedType = T<typename U::manager::template GenBaseHierarchy<T>>;
            ReadOnlyIndexableSet(U* me) : EGM::ReadOnlySet<T, U, ApiPolicy>(me) {}
            EGM::ManagedPtr<ManagedType> get(EGM::ID id) const {
                return EGM::SetDataPolicy<T, U>::get(id);
            }
            EGM::ManagedPtr<ManagedType> get(std::string name) const {
                return ApiPolicy::get(name);
            }
    };

    template <template <class> class T, class U, class ApiPolicy = IndexablePolicy<U>>
    class IndexableSet : public EGM::Set<T, U, ApiPolicy> {
        using EGM::SetDataPolicy<T, U>::get;
        public:
            using ManagedType = T<typename U::manager::template GenBaseHierarchy<T>>;
            IndexableSet(U* me) : EGM::Set<T, U, ApiPolicy>(me) {}
            EGM::ManagedPtr<ManagedType> get(EGM::ID id) const {
                return EGM::SetDataPolicy<T, U>::get(id);
            }
            EGM::ManagedPtr<ManagedType> get(std::string name) const {
                return ApiPolicy::get(name);
            }
    }; 

    template <template <class> class T, class U, class ApiPolicy = IndexablePolicy<U>>
    class ReadOnlyOrderedIndexableSet : public EGM::ReadOnlyOrderedSet<T, U, ApiPolicy> {
        using EGM::OrderedSetDataPolicy<T, U>::get;
        public:
            using ManagedType = T<typename U::manager::template GenBaseHierarchy<T>>;
            ReadOnlyOrderedIndexableSet(U* me) : EGM::ReadOnlyOrderedSet<T, U, ApiPolicy>(me) {}
            EGM::ManagedPtr<ManagedType> get(EGM::ID id) const {
                return EGM::SetDataPolicy<T, U>::get(id);
            }
            EGM::ManagedPtr<ManagedType> get(size_t index) const {
                return EGM::OrderedSetDataPolicy<T, U>::get(index);
            }
            EGM::ManagedPtr<ManagedType> get(std::string name) const {
                return ApiPolicy::get(name);
            }
    };

    template <template <class> class T, class U, class ApiPolicy = IndexablePolicy<U>>
    class IndexableOrderedSet : public EGM::OrderedSet<T, U, ApiPolicy> {
        using EGM::OrderedSetDataPolicy<T, U>::get;
        public:
            using ManagedType = T<typename U::manager::template GenBaseHierarchy<T>>;
            IndexableOrderedSet(U* me) : EGM::OrderedSet<T, U, ApiPolicy>(me) {}
            EGM::ManagedPtr<ManagedType> get(EGM::ID id) const {
                return EGM::SetDataPolicy<T, U>::get(id);
            }
            EGM::ManagedPtr<ManagedType> get(size_t index) const {
                return EGM::OrderedSetDataPolicy<T,U>::get(index);
            }
            EGM::ManagedPtr<ManagedType> get(std::string name) const {
                return ApiPolicy::get(name);
            }
    };
}
