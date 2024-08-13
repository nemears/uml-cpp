#pragma once

#include "uml/set/abstractSet.h"
#include "uml/set/doNothingPolicy.h"
#include "uml/set/set.h"
#include "uml/types/element.h"
#include "uml/umlPtr.h"
#include <memory>

namespace UML {

    template <class T>
    struct OrderedUmlPtr : public UmlPtr<T> {
        std::shared_ptr<OrderedUmlPtr> m_prev;
        std::shared_ptr<OrderedUmlPtr> m_next;
        OrderedUmlPtr(const OrderedUmlPtr& rhs) : UmlPtr<T>(rhs) {
            m_prev = rhs.m_prev;
            m_next = rhs.m_next;
        }
        void destroy() {
            if (m_prev.get()) {
                m_prev->m_next.reset();
            }
            m_prev.reset();
            if (m_next.get()) {
                m_next->m_prev.reset();
            }
            m_next.reset();
        }
        template <class V>
        OrderedUmlPtr(const UmlPtr<V>& rhs) : UmlPtr<T>(rhs) {}
    };

    template <class T>
    class OrderedSetDataPolicy : virtual public SetDataPolicy<T> {
        protected:
            std::shared_ptr<OrderedUmlPtr<T>> m_first;
            std::shared_ptr<OrderedUmlPtr<T>> m_last;
            // std::unordered_map<ID, OrderedUmlPtr<T>> m_allData;
            void allocatePtr(ElementPtr ptr, SetStructure& set) override {
                std::shared_ptr<OrderedUmlPtr<T>> orderedPtr;
                if (&set == this->m_structure.get()) {
                    orderedPtr = std::make_shared<OrderedUmlPtr<T>>(ptr);
                } else {
                    orderedPtr = dynamic_cast<OrderedSetDataPolicy&>(set.m_set).m_last;
                }
                if (!m_first.get()) {
                    m_first = orderedPtr;
                }
                orderedPtr->m_prev = m_last;
                if (m_last.get()) {
                    m_last->m_next = orderedPtr;
                }
                m_last = orderedPtr;
            }
            void deAllocatePtr(ElementPtr ptr) override {
                auto orderedPtr = m_first;
                while (orderedPtr && orderedPtr->id() != ptr.id()) {
                    orderedPtr = orderedPtr->m_next;
                }
                if (orderedPtr->m_prev.get()) {
                    orderedPtr->m_prev->m_next = orderedPtr->m_next;
                }
                if (orderedPtr->m_next.get()) {
                    orderedPtr->m_next->m_prev = orderedPtr->m_prev;
                }
                if (m_last == orderedPtr) {
                    m_last = orderedPtr->m_prev;
                }
                if (m_first == orderedPtr) {
                    m_first = orderedPtr->m_next;
                }
            }
            class iterator : public AbstractSet::iterator {
                friend class OrderedSetDataPolicy;
                const OrderedSetDataPolicy& m_set;
                std::shared_ptr<OrderedUmlPtr<T>> m_curr;
                protected:
                    ElementPtr getCurr() const override {
                        if (m_curr.get()) {
                            return *m_curr;
                        }
                        return ElementPtr();
                    }
                    void next() override {
                        if (m_curr) {
                            if ((*m_curr).id() == (*m_set.m_last).id()) {
                                m_curr = nullptr;
                            } else {
                                m_curr = m_curr->m_next;
                            }
                        }
                        if (m_curr.get()) {
                            std::hash<ID> hasher;
                            m_hash = hasher((*m_curr).id());
                        } else {
                            m_hash = 0;
                        }
                    }
                    std::unique_ptr<AbstractSet::iterator> clone() const override {
                        return std::unique_ptr<iterator>(new iterator(*this));
                    }
                public:
                    iterator(const iterator& rhs) : AbstractSet::iterator(rhs) , m_set(rhs.m_set) {
                        m_curr = rhs.m_curr;
                    }
                    iterator(const OrderedSetDataPolicy& set) : m_set(set) {};
                    T& operator*() {
                        return getCurr()->template as<T>();
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
            std::unique_ptr<AbstractSet::iterator> beginPtr() const override {
                return std::make_unique<iterator>(begin());
            };
            std::unique_ptr<AbstractSet::iterator> endPtr() const override {
                return std::unique_ptr<iterator>(new iterator(end()));
            }
        public:
            ~OrderedSetDataPolicy() {
                if (m_first.get()) {
                    m_first->destroy();
                }
                if (m_last.get()) {
                    m_last->destroy();
                }
            }
            UmlPtr<T> front() const {
                if (m_first.get()) {
                    return *m_first;
                }
                return UmlPtr<T>();
            }
            UmlPtr<T> back() const {
                if (m_last.get()) {
                    return *m_last;
                }
                return UmlPtr<T>();
            }
            iterator begin() const {
                iterator it(*this);
                it.m_curr = m_first;
                if (it.m_curr.get()) {
                    std::hash<ID> hasher;
                    it.m_hash = hasher((*it.m_curr).id());
                }
                return it;
            }
            iterator end() const {
                return iterator(*this);
            }
            SetType setType() const override {
                return SetType::ORDERED_SET;
            }
            UmlPtr<T> get(ID id) const {
                return SetDataPolicy<T>::get(id);
            }
            UmlPtr<T> get(size_t index) const {
                size_t currIndex = 0;
                auto currPtr = m_first;
                while (currIndex != index && currPtr) {
                    currPtr = currPtr->m_next;
                    currIndex++;
                }
                if (currIndex != index) {
                    throw SetStateException("index larger than set size!");
                }
                return *currPtr;
            }
    };

    template <class T, class U, class ApiPolicy = DoNothingPolicy>
    using ReadOnlyOrderedSet = PrivateSet<T, U, OrderedSetDataPolicy<T>, ApiPolicy>;
    
    template <class T, class U, class ApiPolicy = DoNothingPolicy>
    class OrderedSet : public ReadOnlyOrderedSet<T, U, ApiPolicy> {
        public:
            OrderedSet(U* me) : ReadOnlyOrderedSet<T, U, ApiPolicy>(me) {}
            void add(UmlPtr<T> ptr) {
                this->innerAdd(ptr);
            }
            void add(ID& id) {
                this->innerAdd(this->m_el.m_manager->createPtr(id));
            }
            void add(T& el) {
                this->innerAdd(UmlPtr<T>(&el));
            }
            template <class ... Ts>
            void add(T& el, Ts& ... els) { 
                add(el);
                add(els...);
            }
            void remove(UmlPtr<T> ptr) {
                this->innerRemove(ptr);
            }
            void remove(ID& id) {
                this->innerRemove(this->m_el.m_manager->createPtr(id));
            }
            void remove(T& el) {
                this->innerRemove(UmlPtr<T>(&el));
            }
            void clear() {
                while (this->front()) {
                    remove(this->front());
                }
            }

    };
}
