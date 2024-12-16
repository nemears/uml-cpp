#pragma once

#include "uml/managers/abstractElement.h"
#include <memory>
#include <unordered_set>
#include <uml/managers/umlPtr.h>

namespace UML {
    enum class SetType {
        SET,
        SINGLETON,
        ORDERED_SET,
        LIST,
        BAG
    };
    std::ostream& operator<<(std::ostream& stream, const SetType& setType);

    // This is to convey ownership
    //      COMPOSITE: Element owns all elements in this set
    //      ANTICOMPOSITE: elements in this set must be the owner 
    //                     (can only be singleton, there may only be one anticomposite full set per element)
    //      NONE: no relation to ownership
    enum class CompositionType {
        COMPOSITE,
        ANTI_COMPOSITE,
        NONE
    };

    class SetStructure;
    template <class S, class WrapperPolicy>
    class WrapperSet;
    struct IDPolicy;
    typedef WrapperSet<ID, IDPolicy> IDSet;

    class AbstractSet {

        template <class Tlist, class P1>
        friend class Manager;

        template <template <class> class T, class U, class DataTypePolicy, class ApiPolicy>
        friend class PrivateSet;
        template <template <class> class T, class U>
        friend class SetDataPolicy;
        template <class T>
        friend class SingletonDataPolicy;
        template <template <class> class T, class U, class ApiPolicy>
        friend class Singleton;
        template <template <class> class T, class U, class ApiPolicy>
        friend class OrderedSet;
        template <class T>
        friend class Element;
        template <class S, class WrapperPolicy>
        friend class WrapperSet;
        friend class AbstractElement;
        
        protected:
            std::shared_ptr<SetStructure> m_structure;
            virtual bool hasData() const = 0;
            virtual bool containsData(AbstractElementPtr ptr) const = 0;
            virtual bool removeData(AbstractElementPtr ptr) = 0;
            virtual void runAddPolicy(AbstractElement& el) = 0;
            virtual void runRemovePolicy(AbstractElement& el) = 0;
            virtual bool oppositeEnabled() const = 0;
            virtual void oppositeAdd(AbstractElement& el) = 0;
            virtual void oppositeRemove(AbstractElement& el) = 0;
            virtual void addToOpposite(AbstractElementPtr ptr) = 0;
            virtual void nonOppositeAdd(AbstractElementPtr ptr) = 0;
            virtual void nonPolicyAdd(AbstractElementPtr ptr) = 0;
            virtual void innerAdd(AbstractElementPtr ptr) = 0;
            virtual void nonOppositeRemove(AbstractElementPtr ptr) = 0;
            virtual void innerRemove(AbstractElementPtr ptr) = 0;
            virtual void allocatePtr(__attribute__((unused)) AbstractElementPtr ptr, __attribute__((unused)) SetStructure& set) {}
            virtual void deAllocatePtr(__attribute__((unused)) AbstractElementPtr ptr) {}

            class iterator {
                template <class Tlist, class P1>
                friend class Manager;
                template <template <class> class T, class U>
                friend class SetDataPolicy;
                template <class T>
                friend class SingletonDataPolicy;
                template <class S, class WrapperPolicy>
                friend class WrapperSet;
                template <class>
                friend class IndexablePolicy;

                protected:
                    virtual std::unique_ptr<iterator> clone() const = 0;
                public:
                    size_t m_hash = 0;
                    virtual AbstractElementPtr getCurr() const = 0;
                    virtual void next() = 0;
                    iterator() {}
                    iterator(const iterator& rhs) {
                        m_hash = rhs.m_hash;
                    }
                    virtual ~iterator() {}
                    bool operator==(const iterator& rhs) const {
                        return rhs.m_hash == m_hash;
                    }
                    bool operator!=(const iterator& rhs) const {
                        return rhs.m_hash != m_hash;
                    }
            };
        public:
            AbstractSet();
            virtual ~AbstractSet();
            virtual void subsets(AbstractSet& superSet);
            virtual void redefines(AbstractSet& redefinedSet);
            void setComposition(CompositionType composition);
            virtual std::unique_ptr<iterator> beginPtr() const = 0;
            virtual std::unique_ptr<iterator> endPtr() const = 0;
            CompositionType getComposition() const;
            virtual bool contains(AbstractElementPtr ptr) const = 0;
            virtual bool contains(ID id) const = 0;
            size_t size() const;
            bool empty() const;
            virtual SetType setType() const = 0;
            virtual bool readonly() const {
                return true;
            }
            bool rootSet() const;
            bool isSubSetOf(AbstractSet& set) const;
            AbstractSet* subSetContains(ID id) const;
            IDSet ids() const;
    };

    class SetStructure {
        public:
            AbstractSet& m_set;
            std::unordered_set<std::shared_ptr<SetStructure>> m_subSetsWithData;
            std::unordered_set<std::shared_ptr<SetStructure>> m_superSets;
            std::unordered_set<std::shared_ptr<SetStructure>> m_subSets;
            std::unordered_set<std::shared_ptr<SetStructure>> m_redefinedSets;
            std::shared_ptr<SetStructure> m_rootRedefinedSet;
            size_t m_size = 0;
            CompositionType m_composition = CompositionType::NONE;
            SetStructure(AbstractSet& set) : m_set(set) {}
    };


    class AbstractReadableSet : virtual public AbstractSet {
        public:
            virtual void add(ID id) = 0;
            bool readonly() const override {
                return false;
            }
    };    
}
