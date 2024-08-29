#pragma once

#include "uml/set/orderedSet.h"
#include "uml/set/set.h"
#include "uml/types/element.h"
#include "uml/types/namedElement.h"


// This is just an idea, there are other ways to implement this
// the main reason for this is to have minimally reproducible set type 
// with a NamedElementPtr get(string) const; method
// there are other approaches we could take, see note below

namespace UML {

    typedef UmlPtr<NamedElement> NamedElementPtr;
    
    class IndexablePolicy : public DoNothingPolicy , virtual public AbstractSet {
        public:
            NamedElementPtr get(std::string qualifiedName) const {
                auto it = this->beginPtr();
                while (*it != *this->endPtr()) {
                    auto curr = it->getCurr();
                    if (curr->is<NamedElement>()) {
                        NamedElementPtr namedCurr = curr;
                        auto elQualifiedName = namedCurr->getQualifiedName();
                        auto elName = namedCurr->getName();
                        if (elQualifiedName == qualifiedName) {
                            return curr;
                        }
                        if (elName == qualifiedName) {
                            return curr;
                        }
                    }
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
    template <class T, class U, class ApiPolicy = IndexablePolicy>
    class ReadOnlyIndexableSet : public ReadOnlySet<T, U, ApiPolicy> {
        public:
            ReadOnlyIndexableSet(U* me) : ReadOnlySet<T, U, ApiPolicy>(me) {}
            UmlPtr<T> get(ID id) const {
                return SetDataPolicy<T>::get(id);
            }
            UmlPtr<T> get(std::string name) const {
                return IndexablePolicy::get(name);
            }
    };

    template <class T, class U, class ApiPolicy = IndexablePolicy>
    class IndexableSet : public Set<T, U, ApiPolicy> {
        using SetDataPolicy<T>::get;
        public:
            IndexableSet(U* me) : Set<T, U, ApiPolicy>(me) {}
            UmlPtr<T> get(ID id) const {
                return SetDataPolicy<T>::get(id);
            }
            UmlPtr<T> get(std::string name) const {
                return IndexablePolicy::get(name);
            }
    }; 

    template <class T, class U, class ApiPolicy = IndexablePolicy>
    class ReadOnlyOrderedIndexableSet : public ReadOnlyOrderedSet<T, U, ApiPolicy> {
        using OrderedSetDataPolicy<T>::get;
        public:
            ReadOnlyOrderedIndexableSet(U* me) : ReadOnlyOrderedSet<T, U, ApiPolicy>(me) {}
            UmlPtr<T> get(ID id) const {
                return SetDataPolicy<T>::get(id);
            }
            UmlPtr<T> get(size_t index) const {
                return OrderedSetDataPolicy<T>::get(index);
            }
            UmlPtr<T> get(std::string name) const {
                return IndexablePolicy::get(name);
            }
    };

    template <class T, class U, class ApiPolicy = IndexablePolicy>
    class IndexableOrderedSet : public OrderedSet<T, U, ApiPolicy> {
        using OrderedSetDataPolicy<T>::get;
        public:
            IndexableOrderedSet(U* me) : OrderedSet<T, U, ApiPolicy>(me) {}
            UmlPtr<T> get(ID id) const {
                return SetDataPolicy<T>::get(id);
            }
            UmlPtr<T> get(size_t index) const {
                return OrderedSetDataPolicy<T>::get(index);
            }
            UmlPtr<T> get(std::string name) const {
                return IndexablePolicy::get(name);
            }
    };
}
