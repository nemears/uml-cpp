#pragma once

#include <memory>
#include <unordered_set>
#include "../types/element.h"
#include "uml/umlPtr.h"

namespace UML {
    enum class SetType {
        SET,
        SINGLETON,
        ORDERED_SET,
        LIST,
        BAG
    };

    class AbstractSet {

        template <class T, class U, class DataTypePolicy, class ApiPolicy>
        friend class PrivateSet;

        protected:
            std::unordered_set<std::shared_ptr<AbstractSet>> m_subSetsWithData;
            std::unordered_set<std::shared_ptr<AbstractSet>> m_superSets;
            std::unordered_set<std::shared_ptr<AbstractSet>> m_subSets;
            std::unordered_set<std::shared_ptr<AbstractSet>> m_redefinedSets;
            std::shared_ptr<AbstractSet> m_rootRedefinedSet;
            size_t m_size = 0;
            virtual bool hasData() const = 0;
            virtual bool containsData(AbstractUmlPtr ptr) const = 0;
            virtual bool removeData(AbstractUmlPtr ptr) = 0;
            virtual void runAddPolicy(Element& el) = 0;
            virtual void runRemovePolicy(Element& el) = 0;
            virtual bool oppositeEnabled() const = 0;
            virtual void oppositeAdd(Element& el) = 0;
            virtual void oppositeRemove(Element& el) = 0;
            virtual void nonOppositeAdd(AbstractUmlPtr ptr) = 0;
            virtual void innerAdd(AbstractUmlPtr ptr) = 0;
            virtual void nonOppositeRemove(AbstractUmlPtr ptr) = 0;
            virtual void innerRemove(AbstractUmlPtr ptr) = 0;
            virtual void allocatePtr(AbstractUmlPtr ptr) = 0;
            virtual void deAllocatePtr(AbstractUmlPtr ptr) = 0;
            class iterator {
                protected:
                    virtual AbstractUmlPtr getCurr() const = 0;
                    virtual void next() = 0;
                    virtual std::unique_ptr<iterator> clone() const = 0;
                public:
                    virtual ~iterator() {}
            };
        public:
            virtual ~AbstractSet() {}
            virtual void subsets(AbstractSet& superSet) {
                superSet.m_rootRedefinedSet->m_subSets.insert(m_rootRedefinedSet);
                m_subSets.insert(superSet.m_rootRedefinedSet); 
            }
            virtual void redefines(AbstractSet& redefinedSet) {
                for (std::shared_ptr<AbstractSet> superSet : m_rootRedefinedSet->m_superSets) {
                    redefinedSet.m_rootRedefinedSet->m_superSets.insert(superSet);
                }
                for (std::shared_ptr<AbstractSet> subSet : m_rootRedefinedSet->m_subSets) {
                    redefinedSet.m_rootRedefinedSet->m_subSets.insert(subSet);
                }
                for (std::shared_ptr<AbstractSet> alreadyRedefinedSet : m_rootRedefinedSet->m_redefinedSets) {
                    redefinedSet.m_rootRedefinedSet->m_redefinedSets.insert(alreadyRedefinedSet);
                }
                m_rootRedefinedSet->m_superSets.clear();
                m_rootRedefinedSet->m_subSets.clear();
                m_rootRedefinedSet->m_redefinedSets.clear();
                redefinedSet.m_rootRedefinedSet->m_redefinedSets.insert(m_rootRedefinedSet);
                m_rootRedefinedSet = redefinedSet.m_rootRedefinedSet;
            }
            virtual bool contains(AbstractUmlPtr ptr) const = 0;
            size_t size() const {
                return m_size;
            }
            virtual SetType setType() const = 0;
            virtual std::unique_ptr<iterator> begin() const = 0;
            virtual std::unique_ptr<iterator> end() const = 0;
    };
}
