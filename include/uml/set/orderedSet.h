#pragma once

#include "set.h"

namespace UML {

    struct OrderedSetNode : public SetNode {
        OrderedSetNode* m_prev = 0;
        OrderedSetNode* m_next = 0;
    };

    template<class T, class U> class OrderedSet2;

    template <class T>
    class OrderedSet2Iterator {

        template<class V, class W> friend class OrderedSet2;

        private:
            OrderedSetNode* curr = 0;
            std::unordered_set<AbstractSet2*> validSets;
        public:
            OrderedSet2Iterator() {}
            OrderedSet2Iterator(const OrderedSet2Iterator& rhs) {
                curr = rhs.curr;
                validSets = rhs.validSets;
            }
            T& operator*() {
                return curr->m_ptr->as<T>();
            }
            UmlPtr<T> operator->() {
                return curr->m_ptr;
            }
            OrderedSet2Iterator operator++() {
                do {
                    curr = curr->m_next;
                } while (curr && !validSets.count(curr->set));
                return *this;
            }
            friend bool operator== (const OrderedSet2Iterator& lhs, const OrderedSet2Iterator& rhs) {
                if (!lhs.curr && !lhs.curr) {
                    return true;
                }
                if (!lhs.curr || !lhs.curr) {
                    return false;
                }
                return lhs.curr == rhs.curr;
            }
            friend bool operator!= (const OrderedSet2Iterator& lhs, const OrderedSet2Iterator& rhs) {
                if (!lhs.curr && !lhs.curr) {
                    return false;
                }
                if (!lhs.curr || !lhs.curr) {
                    return true;
                }
                return lhs.curr != rhs.curr;
            }
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

            OrderedSet2Iterator<T> begin() {
                OrderedSet2Iterator<T> ret;
                ret.curr = this->m_first;
                ret.validSets = this->getAllSuperSets();
                ret.validSets.insert(this);
                return ret;
            }

            OrderedSet2Iterator<T> end() {
                return OrderedSet2Iterator<T>();
            }
    };
}