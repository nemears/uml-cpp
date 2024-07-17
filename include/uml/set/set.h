#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "../umlPtr.h"
#include "privateSet.h"
#include "uml/set/abstractSet.h"
#include "uml/set/doNothingPolicy.h"

namespace UML {

    template <class T>
    class SetDataPolicy : public AbstractSet {
        private:        
            std::unordered_map<ID, UmlPtr<T>> m_data;
            class iterator : public AbstractSet::iterator {
                private:
                    std::weak_ptr<SetDataPolicy<T>> m_me;
                    typename std::unordered_map<ID, UmlPtr<T>>::iterator m_dataIt;
                    std::unordered_set<std::shared_ptr<AbstractSet>>::iterator m_subSetWithDataIt;
                    std::unique_ptr<AbstractSet::iterator> m_currSetIt;
                    std::unique_ptr<AbstractSet::iterator> clone() const override {
                        return new iterator(*this);
                    }
                    UmlPtr<T> getCurr() const override {
                        if (m_dataIt != m_me->m_data.end()) {
                            return *(m_dataIt->second);
                        } else {
                            return m_currSetIt->getCurr();
                        }
                    }
                    void next() override {
                        if (m_dataIt != m_me->m_data.end()) {
                            m_dataIt++;
                        } else {
                            std::shared_ptr<AbstractSet> currentSet = *m_subSetWithDataIt;
                            if (!m_currSetIt) {
                                m_currSetIt = currentSet->begin().clone();
                            } else if (m_currSetIt != currentSet->end().clone()) {
                                m_currSetIt->next();
                            } else {
                                throw SetStateException("iterating past end!");
                            }
                        }
                    }
                public:
                    T& operator*() {
                        return *getCurr();
                    }
                    UmlPtr<T> operator->() {
                        return getCurr(); 
                    }
                    iterator operator++() {
                        next();
                        return *this;                        
                    }
                    iterator operator++(int) {
                        return ++(*this);
                    }
                    bool operator==(const AbstractSet::iterator& right) const {
                        return &right == this;
                    }
                    bool operator!=(const AbstractSet::iterator& right) const {
                        return &right != this;
                    }
            };
     protected:
            bool hasData() const {
                return !m_data.empty();
            }
            bool containsData(UmlPtr<T> ptr) const {
                return m_data.count(ptr.id()) > 0;
            }
            void addData(UmlPtr<T> ptr) {
                m_data.insert(ptr.id, ptr);
            }
            void removeData(UmlPtr<T> ptr) {
                m_data.erase(ptr.id());
            }
        public:
            virtual ~SetDataPolicy() {}
            SetType setType() const override {
                return SetType::SET;
            }
            UmlPtr<T> front() const {
                if (m_data.size() > 0) {
                    return m_data.begin()->second;
                }
                return UmlPtr<T>();
            }
            std::unique_ptr<AbstractSet::iterator> begin() const override {
                std::unique_ptr<iterator> ret(new iterator);
                ret->m_me = m_rootRedefinedSet;
                ret->m_dataIt = m_data.begin();
                ret->m_subSetsWithDataIt = m_subSetsWithData.begin();
                return ret;
            }
            std::unique_ptr<AbstractSet::iterator> end() const override {
                return std::unique_ptr<iterator>(new iterator);
            }
    };

    template <class T, class U, class ApiPolicy = DoNothingPolicy>
    using ReadOnlySet = PrivateSet<T, U, SetDataPolicy<T>, ApiPolicy>;

    template <class T, class U, class ApiPolicy>
    class Set : ReadOnlySet<T, U, ApiPolicy> {
        public:
            void add(UmlPtr<T> ptr) {
                innerAdd(ptr);
            }
            void add(ID& id) {
                innerAdd(UmlPtr<T>(id));
            }
            void add(T& el) {
                innerAdd(UmlPtr<T>(el));
            }
            void remove(UmlPtr<T> ptr) {
                innerRemove(ptr);
            }
            void remove(ID& id) {
                innerRemove(UmlPtr<T>(id));
            }
            void remove(T& el) {
                innerRemove(UmlPtr<T>(el));
            }
    };
}
