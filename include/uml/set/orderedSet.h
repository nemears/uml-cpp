#pragma once

#include "abstractSet.h"
#include "uml/set/doNothingPolicy.h"
#include "uml/set/privateSet.h"
#include <memory>
#include <unordered_map>

namespace UML {

    template <class T>
    class OrderedUmlPtr : public UmlPtr<T> {
        OrderedUmlPtr<T> m_prev;
        OrderedUmlPtr<T> m_next;
    };

    template <class T>
    class OrderedSetDataPolicy : public AbstractSet {
        protected:
            OrderedUmlPtr<T> m_first;
            OrderedUmlPtr<T> m_last;
            std::unordered_map<ID, OrderedUmlPtr<T>> m_data;
            std::unordered_map<ID, OrderedUmlPtr<T>> m_allData;
            bool hasData() const {
                return m_data.size() > 0;
            }
            bool containsData(UmlPtr<T> ptr) {
                return m_data.count(ptr);
            }
            void allocatePtr(UmlPtr<T> ptr) {
                OrderedUmlPtr<T> orderedPtr(ptr);
                if (!m_first) {
                    m_first = orderedPtr;
                }
                orderedPtr.m_prev = m_last;
                m_last.m_next = orderedPtr;
                m_last = orderedPtr;
                m_allData.insert(orderedPtr.id(), orderedPtr);
            }
            void deAllocatePtr(UmlPtr<T> ptr) {
                auto orderedPtr = m_allData.at(ptr.id());
                if (orderedPtr.m_prev) {
                    orderedPtr.m_prev.m_next = orderedPtr.m_next;
                }
                if (orderedPtr.m_next) {
                    orderedPtr.m_next.m_prev = orderedPtr.m_prev;
                }
                if (m_last == orderedPtr) {
                    m_last = orderedPtr.m_prev;
                }
                if (m_first == orderedPtr) {
                    m_first = orderedPtr.m_next;
                }
                m_allData.erase(ptr.id());
            }
            void addData(UmlPtr<T> ptr) {
                OrderedUmlPtr<T> orderedPtr = m_allData.get(ptr.id);
                m_data.insert(ptr.id(), orderedPtr);
            }
            void removeData(UmlPtr<T> ptr) {
                m_data.erase(ptr.id());
            }
            class iterator : public AbstractSet::iterator {
                OrderedUmlPtr<T> m_curr;
                protected:
                    UmlPtr<T> getCurr() {
                        return m_curr;
                    }
                    void next() {
                        if (m_curr) {
                            m_curr = m_curr.m_next;
                        }
                    }
                    std::unique_ptr<iterator> clone() {
                        return std::unique_ptr<iterator>(new iterator(*this));
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
            };
        public:
            UmlPtr<T> front() const {
                return m_first;
            }
            UmlPtr<T> back() const {
                return m_last;
            }
            iterator begin() const override {
                iterator it;
                it.m_curr = m_first;
            }
            iterator end() const override {
                return iterator();
            }
            SetType setType() const override {
                return SetType::ORDERED_SET;
            }
    };

    template <class T, class U, class ApiPolicy = DoNothingPolicy>
    using ReadOnlyOrderedSet = PrivateSet<T, U, OrderedSetDataPolicy<T>, ApiPolicy>;
    
    template <class T, class U, class ApiPolicy = DoNothingPolicy>
    class OrderedSet : ReadOnlyOrderedSet<T, U, ApiPolicy> {
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
