#pragma once

#include "set.h"

namespace UML {

    struct OrderedSetNode : public SetNode {
        OrderedSetNode* m_prev = 0;
        OrderedSetNode* m_next = 0;
    };

    template <class T>
    class OrderedSetNodeCreationPolicy {
        protected:
            OrderedSetNode* m_first = 0;
            OrderedSetNode* m_last = 0;
            SetNode* create(T& el) {
                OrderedSetNode* ret = new OrderedSetNode();
                ret->m_ptr = &el;
                ret->m_prev = m_last;
                m_last = ret;
                if (!m_first) {
                    m_first = ret;
                }
                return ret;
            }
    };

    template<class T, class U>
    class OrderedSet2 : public Set2<T, U, OrderedSetNodeCreationPolicy<T>> {
        public:
            OrderedSet2(U& el) : Set2<T, U, OrderedSetNodeCreationPolicy<T>>(el) {}
            UmlPtr<T> front() {
                SetLock myLock = this->m_el.m_manager->lockEl(this->m_el);
                if (!this->m_first) {
                    // TODO throw error
                }
                return this->m_first->m_ptr;
            }
            UmlPtr<T> back() {
                SetLock myLock = this->m_el.m_manager->lockEl(this->m_el);
                if (!this->m_last) {
                    // TODO throw error
                }
                return this->m_last->m_ptr;
            }
    };
}