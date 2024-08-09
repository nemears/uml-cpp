#pragma once

#include "uml/set/abstractSet.h"

namespace UML {
    template <class S, class WrapperPolicy>
    class WrapperSet : public WrapperPolicy{
        protected:
            std::weak_ptr<SetStructure> m_set;
            class iterator : public WrapperPolicy {
                private:
                    std::unique_ptr<AbstractSet::iterator> m_it;
                public:
                    iterator(std::unique_ptr<AbstractSet::iterator> it) : m_it(std::move(it)) {}
                    iterator(const iterator& rhs) : m_it(rhs.m_it->clone()) {}
                    S operator*() {
                        return WrapperPolicy::get(m_it->getCurr());
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
                        return *rhs.m_it == *m_it;
                    }
                    bool operator!=(const iterator& rhs) const {
                        return *rhs.m_it != *m_it;
                    }
            };
        public:
            WrapperSet(const AbstractSet* set) : m_set(set->m_structure) {}
            iterator begin() {
                return iterator(m_set.lock()->m_set.beginPtr());
            }
            iterator end() {
                return iterator(m_set.lock()->m_set.endPtr());
            }
    };

    struct IDPolicy {
        ID get(ElementPtr ptr) {
            return ptr.id();
        }
    };
    typedef WrapperSet<ID, IDPolicy> IDSet;
}
