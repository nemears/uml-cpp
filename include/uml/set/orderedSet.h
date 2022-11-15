#pragma once

#include "set.h"

namespace UML {

    struct OrderedSetNode : public SetNode {
        OrderedSetNode* m_prev = 0;
        OrderedSetNode* m_next = 0;
    };

    template <class T>
    class OrderedSetNodeAllocationPolicy {
        // this policy implements a linked list on top of the regular set tree
        protected:
            OrderedSetNode* m_first = 0;
            OrderedSetNode* m_last = 0;
            SetNode* create(T& el) {
                OrderedSetNode* ret = new OrderedSetNode();
                ret->m_ptr = &el;
                ret->m_prev = m_last;
                if (ret->m_prev) {
                    ret->m_prev->m_next = ret;
                }
                m_last = ret;
                if (!m_first) {
                    m_first = ret;
                }
                return ret;
            }
            void deleteNode(SetNode* node) {
                OrderedSetNode* orderedNode = static_cast<OrderedSetNode*>(node);
                if (m_first == node) {
                    m_first = orderedNode->m_next;
                }
                if (m_last == node) {
                    m_last = orderedNode->m_prev;
                }
                if (orderedNode->m_prev) {
                    orderedNode->m_prev->m_next = orderedNode->m_next;
                }
                if (orderedNode->m_next) {
                    orderedNode->m_next->m_prev = orderedNode->m_prev;
                }
                delete node;
            }
    };

    template<class T, class U>
    class OrderedSet2 : public PrivateSet<T, U, OrderedSetNodeAllocationPolicy<T>> {
        public:
            OrderedSet2(U& el) : PrivateSet<T, U, OrderedSetNodeAllocationPolicy<T>>(el) {}
            UmlPtr<T> front() {
                SetLock myLock = this->m_el.m_manager->lockEl(this->m_el);
                if (!this->m_first) {
                    throw SetStateException("front is null");
                }
                return this->m_first->m_ptr;
            }
            UmlPtr<T> back() {
                SetLock myLock = this->m_el.m_manager->lockEl(this->m_el);
                if (!this->m_last) {
                    throw SetStateException("last is null");
                }
                return this->m_last->m_ptr;
            }
            void add(UmlPtr<T> el) {
                add(*el);
            }
            void add(T& el) {
                this->PrivateSet<T,U, OrderedSetNodeAllocationPolicy<T>>::add(el);
            }
            void remove(ID id) {
                this->PrivateSet<T,U, OrderedSetNodeAllocationPolicy<T>>::remove(id);
            }
            void remove(T& el) {
                remove(el.getID());
            }
            void remove (UmlPtr<T> el) {
                remove(el.id());
            }
    };
}