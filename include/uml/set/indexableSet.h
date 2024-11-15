#pragma once

#include "uml/set/orderedSet.h"
#include "uml/set/set.h"
#include "uml/types/namedElement.h"


// This is just an idea, there are other ways to implement this
// the main reason for this is to have minimally reproducible set type 
// with a NamedElementPtr get(string) const; method
// there are other approaches we could take, see note below

namespace UML {

    // typedef UmlPtr<NamedElement> NamedElementPtr;
    
    template <class ManagerPolicy>
    class IndexablePolicy : public DoNothingPolicy , virtual public AbstractSet {
        using NamedElementPtr = UmlPtr<NamedElement<typename ManagerPolicy::Manager::template GenBaseHierarchy<NamedElement>>>;
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
                return AbstractElementPtr();
            }
    };


    // soft binding , 
    // Note: could avoid this with compile time subsets and redefines to handle api policies so methods
    // of subsetted and redefined set polices would be inherited, figure out how easy that is to implement?
    // something like this Set<NamedElement, Namespace, [ *m_members, *m_ownedElements ], []> m_ownedMembers ...
    // where the third template argument is subsets, second redefinition, and then an optional policy on the end
    template <template <class> class T, class U, class ApiPolicy = IndexablePolicy<U>>
    class ReadOnlyIndexableSet : public ReadOnlySet<T, U, ApiPolicy> {
            using ManagedType = T<typename U::Manager::template GenBaseHierarchy<T>>;
        public:
            ReadOnlyIndexableSet(U* me) : ReadOnlySet<T, U, ApiPolicy>(me) {}
            UmlPtr<ManagedType> get(ID id) const {
                return SetDataPolicy<ManagedType>::get(id);
            }
            UmlPtr<ManagedType> get(std::string name) const {
                return ApiPolicy::get(name);
            }
    };

    template <template <class> class T, class U, class ApiPolicy = IndexablePolicy<U>>
    class IndexableSet : public Set<T, U, ApiPolicy> {
        using ManagedType = T<typename U::Manager::template GenBaseHierarchy<T>>;
        using SetDataPolicy<ManagedType>::get;
        public:
            IndexableSet(U* me) : Set<T, U, ApiPolicy>(me) {}
            UmlPtr<ManagedType> get(ID id) const {
                return SetDataPolicy<ManagedType>::get(id);
            }
            UmlPtr<ManagedType> get(std::string name) const {
                return ApiPolicy::get(name);
            }
    }; 

    template <template <class> class T, class U, class ApiPolicy = IndexablePolicy<U>>
    class ReadOnlyOrderedIndexableSet : public ReadOnlyOrderedSet<T, U, ApiPolicy> {
        using ManagedType = T<typename U::Manager::template GenBaseHierarchy<T>>;
        using OrderedSetDataPolicy<ManagedType>::get;
        public:
            ReadOnlyOrderedIndexableSet(U* me) : ReadOnlyOrderedSet<T, U, ApiPolicy>(me) {}
            UmlPtr<ManagedType> get(ID id) const {
                return SetDataPolicy<ManagedType>::get(id);
            }
            UmlPtr<ManagedType> get(size_t index) const {
                return OrderedSetDataPolicy<ManagedType>::get(index);
            }
            UmlPtr<ManagedType> get(std::string name) const {
                return ApiPolicy::get(name);
            }
    };

    template <template <class> class T, class U, class ApiPolicy = IndexablePolicy<U>>
    class IndexableOrderedSet : public OrderedSet<T, U, ApiPolicy> {
        using ManagedType = T<typename U::Manager::template GenBaseHierarchy<T>>;
        using OrderedSetDataPolicy<ManagedType>::get;
        public:
            IndexableOrderedSet(U* me) : OrderedSet<T, U, ApiPolicy>(me) {}
            UmlPtr<ManagedType> get(ID id) const {
                return SetDataPolicy<ManagedType>::get(id);
            }
            UmlPtr<ManagedType> get(size_t index) const {
                return OrderedSetDataPolicy<ManagedType>::get(index);
            }
            UmlPtr<ManagedType> get(std::string name) const {
                return ApiPolicy::get(name);
            }
    };
}
