#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "../umlPtr.h"
#include "privateSet2.h"
#include "uml/set/abstractSet2.h"

namespace UML {

    template <class T>
    class SetIterator;

    template <class T>
    class SetDataPolicy : public AbstractSet2 {
        private:    
            std::unordered_map<ID, UmlPtr<T>> m_data;
            class iterator : public AbstractSet2::iterator {
                private:
                    std::weak_ptr<SetDataPolicy<T>> m_me;
                    typename std::unordered_map<ID, UmlPtr<T>>::iterator m_dataIt;
                    std::unordered_set<std::shared_ptr<AbstractSet2>>::iterator m_subSetWithDataIt;
                    std::unique_ptr<AbstractSet2::iterator> m_currSetIt;
                    UmlPtr<T> getCurr() const override {
                        if (m_dataIt != m_me->m_data.end()) {
                            return *(m_dataIt->second);
                        } else {
                            // TODO

                        }
                    }
                    iterator next() override {
                        if (m_dataIt != m_me->m_data.end()) {
                            m_dataIt++;
                        } else {
                            std::shared_ptr<AbstractSet2> currentSet = *m_subSetWithDataIt;
                            if (!m_currSetIt) {
                                m_currSetIt = std::unique_ptr<AbstractSet2::iterator>(currentSet->begin());
                            } else {
                                if (m_currSetIt != currentSet->end()) {
                                    (*m_currSetIt)++;
                                }
                            }
                        }
                    }
                public:
                    T& operator*() {
                        return *getCurr();
                    }
                    UmlPtr<T> operator->() override {
                        return getCurr(); 
                    }
                    iterator operator++() override {
                    
                    }
                    iterator operator++(int) {
                        return ++(*this);
                    }
                    friend bool operator==(iterator& left, AbstractSet2::iterator& right) {
                    
                    }
                    friend bool operator!=(iterator& left, AbstractSet2::iterator& right) {
                    
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
            SetType setType() const override {
                return SetType::SET;
            }
            UmlPtr<T> front() const {
                if (m_data.size() > 0) {
                    return m_data.begin()->second;
                }
                return UmlPtr<T>();
            }
            SetIterator<T> begin() const {
                // TODO
            }
            SetIterator<T> end() const {
                // TODO
            }
    };

    template <class T, class ApiPolicy, class U>
    using ReadOnlySet2 = PrivateSet2<T, SetDataPolicy<T>, ApiPolicy, U>;

    template <class T, class ApiPolicy, class U>
    class Set2 : ReadOnlySet2<T, ApiPolicy, U> {
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
