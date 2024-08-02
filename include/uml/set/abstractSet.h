#pragma once

#include <memory>
#include <unordered_set>
#include <uml/umlPtr.h>

namespace UML {
    enum class SetType {
        SET,
        SINGLETON,
        ORDERED_SET,
        LIST,
        BAG
    };

    template <class T>
    class SetDataPolicy;

    class AbstractSet {

        template <class T, class U, class DataTypePolicy, class ApiPolicy>
        friend class PrivateSet;
        template <class T>
        friend class SetDataPolicy;
        template <class T>
        friend class SingletonDataPolicy;
        friend class Element;

        protected:
            std::unordered_set<std::shared_ptr<AbstractSet>> m_subSetsWithData;
            std::unordered_set<std::shared_ptr<AbstractSet>> m_superSets;
            std::unordered_set<std::shared_ptr<AbstractSet>> m_subSets;
            std::unordered_set<std::shared_ptr<AbstractSet>> m_redefinedSets;
            std::shared_ptr<AbstractSet> m_rootRedefinedSet;
            size_t m_size = 0;
            virtual bool hasData() const = 0;
            virtual bool containsData(ElementPtr ptr) const = 0;
            virtual bool removeData(ElementPtr ptr) = 0;
            virtual void runAddPolicy(Element& el) = 0;
            virtual void runRemovePolicy(Element& el) = 0;
            virtual bool oppositeEnabled() const = 0;
            virtual void oppositeAdd(Element& el) = 0;
            virtual void oppositeRemove(Element& el) = 0;
            virtual void nonOppositeAdd(ElementPtr ptr) = 0;
            virtual void innerAdd(ElementPtr ptr) = 0;
            virtual void nonOppositeRemove(ElementPtr ptr) = 0;
            virtual void innerRemove(ElementPtr ptr) = 0;
            virtual void allocatePtr(ElementPtr ptr) = 0;
            virtual void deAllocatePtr(ElementPtr ptr) = 0;

            class IDSet;

            class iterator {
                template <class T>
                friend class SetDataPolicy;
                template <class T>
                friend class SingletonDataPolicy;
                friend class IDSet;

                protected:
                    size_t m_hash = 0;
                    virtual ElementPtr getCurr() const = 0;
                    virtual void next() = 0;
                    virtual std::unique_ptr<iterator> clone() const = 0;
                public:
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
            class IDSet {
                protected:
                    std::weak_ptr<AbstractSet> m_set;
                    class iterator {
                        private:
                            std::unique_ptr<AbstractSet::iterator> m_it;
                        public:
                            iterator(std::unique_ptr<AbstractSet::iterator> it) : m_it(std::move(it)) {}
                            iterator(const iterator& rhs) : m_it(rhs.m_it->clone()) {}
                            ID operator*() {
                                return m_it->getCurr().id();
                            }
                            ElementPtr operator->() {
                                return m_it->getCurr();
                            }
                            iterator operator++() {
                                m_it->next();
                                return *this;
                            }
                            iterator operator++(int) {
                                m_it->next();
                                return *this;
                            }
                            bool operator==(const iterator& rhs) const {
                                return rhs.m_it != m_it;
                            }
                            bool operator!=(const iterator& rhs) const {
                                return rhs.m_it != m_it;
                            }
                    };
                public:
                    IDSet(const AbstractSet* set) : m_set(set->m_rootRedefinedSet) {}
                    iterator begin() {
                        return iterator(m_set.lock()->beginPtr());
                    }
                    iterator end() {
                        return iterator(m_set.lock()->endPtr());
                    }
            };
            virtual std::unique_ptr<iterator> beginPtr() const = 0;
            virtual std::unique_ptr<iterator> endPtr() const = 0;
        public:
            AbstractSet() : m_rootRedefinedSet(this) {}
            virtual ~AbstractSet() {
                for (auto subSet : m_subSets) {
                    subSet->m_superSets.erase(subSet->m_superSets.find(m_rootRedefinedSet));
                }
                m_subSets.clear();
                for (auto superSet : m_superSets) {
                    superSet->m_subSets.erase(superSet->m_superSets.find(m_rootRedefinedSet));
                }
                m_superSets.clear();
                
                m_redefinedSets.clear();
            }
            virtual void subsets(AbstractSet& superSet) {
                superSet.m_rootRedefinedSet->m_subSets.insert(m_rootRedefinedSet);
                m_rootRedefinedSet->m_subSets.insert(superSet.m_rootRedefinedSet); 
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
            virtual bool contains(ElementPtr ptr) const = 0;
            size_t size() const {
                return m_size;
            }
            bool empty() const {
                return m_size > 0;
            }
            IDSet ids() const {
                return IDSet(this);
            }
            virtual SetType setType() const = 0;
    };

}
