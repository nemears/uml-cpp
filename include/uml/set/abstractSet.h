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
    std::ostream& operator<<(std::ostream& stream, const SetType& setType);

    class SetStructure;

    class AbstractSet {
        template <class T, class U, class DataTypePolicy, class ApiPolicy>
        friend class PrivateSet;
        template <class T>
        friend class SetDataPolicy;
        template <class T>
        friend class SingletonDataPolicy;
        friend class Element;
        protected:
            std::shared_ptr<SetStructure> m_structure;
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
                    virtual ElementPtr getCurr() const = 0;
                    virtual void next() = 0;
                    virtual std::unique_ptr<iterator> clone() const = 0;
                public:
                    size_t m_hash = 0;
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
                    std::weak_ptr<SetStructure> m_set;
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
                    IDSet(const AbstractSet* set) : m_set(set->m_structure) {}
                    iterator begin();
                    iterator end();
            };
            virtual std::unique_ptr<iterator> beginPtr() const = 0;
            virtual std::unique_ptr<iterator> endPtr() const = 0;
        public:
            AbstractSet();
            virtual ~AbstractSet();
            virtual void subsets(AbstractSet& superSet);
            virtual void redefines(AbstractSet& redefinedSet);
            virtual bool contains(ElementPtr ptr) const = 0;
            size_t size() const;
            bool empty() const;
            IDSet ids() const {
                return IDSet(this);
            }
            virtual SetType setType() const = 0;

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
            SetStructure(AbstractSet& set) : m_set(set) {}
    };
    
}
