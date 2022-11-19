#pragma once

#include "set.h"

namespace UML {

    struct OrderedSetNode : public SetNode {
        OrderedSetNode* m_prev = 0;
        OrderedSetNode* m_next = 0;
    };

    template<class V, class W, class AdditionPolicy, class RemovalPolicy> class CustomOrderedSet;

    template <class T>
    class OrderedSetIterator {

        template<class V, class W, class AdditionPolicy, class RemovalPolicy> friend class CustomOrderedSet;

        private:
            OrderedSetNode* curr = 0;
            std::unordered_set<AbstractSet*> validSets;
        public:
            OrderedSetIterator() {}
            OrderedSetIterator(const OrderedSetIterator& rhs) {
                curr = rhs.curr;
                validSets = rhs.validSets;
            }
            T& operator*() {
                return curr->m_ptr->as<T>();
            }
            UmlPtr<T> operator->() {
                return curr->m_ptr;
            }
            OrderedSetIterator operator++() {
                do {
                    curr = curr->m_next;
                } while (curr && !validSets.count(curr->set));
                return *this;
            }
            friend bool operator== (const OrderedSetIterator& lhs, const OrderedSetIterator& rhs) {
                if (!lhs.curr && !lhs.curr) {
                    return true;
                }
                if (!lhs.curr || !lhs.curr) {
                    return false;
                }
                return lhs.curr == rhs.curr;
            }
            friend bool operator!= (const OrderedSetIterator& lhs, const OrderedSetIterator& rhs) {
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

    template <class T, class U>
    class OrderedSet : virtual public TypedSet<T,U> {
        public:
            virtual UmlPtr<T> front() = 0;
            virtual UmlPtr<T> back() = 0;
            virtual void add(UmlPtr<T> el) = 0;
            virtual void add(T& el) = 0;
            virtual void remove(ID id) = 0;
            virtual void remove(T& el) = 0;
            virtual void remove(UmlPtr<T> el) = 0;
            virtual OrderedSetIterator<T> begin() = 0;
            virtual OrderedSetIterator<T> end() = 0;
    };

    template <
                class T, 
                class U,
                class AdditionPolicy = DoNothing<T, U>,
                class RemovalPolicy = DoNothing<T, U>
            >
    class CustomOrderedSet : public PrivateSet<T, U, AdditionPolicy, RemovalPolicy, OrderedSetNodeAllocationPolicy<T>> , public OrderedSet<T,U> {
        public:
            CustomOrderedSet(U& el) : PrivateSet<T, U, AdditionPolicy, RemovalPolicy, OrderedSetNodeAllocationPolicy<T>>(el) {}
            CustomOrderedSet(U* el) : PrivateSet<T, U, AdditionPolicy, RemovalPolicy, OrderedSetNodeAllocationPolicy<T>>(el) {}
            UmlPtr<T> front() override {
                SetLock myLock = this->m_el.m_manager->lockEl(this->m_el);
                if (!this->m_first) {
                    throw SetStateException("front is null");
                }
                return this->m_first->m_ptr;
            }
            UmlPtr<T> back() override {
                SetLock myLock = this->m_el.m_manager->lockEl(this->m_el);
                if (!this->m_last) {
                    throw SetStateException("last is null");
                }
                return this->m_last->m_ptr;
            }
            void add(UmlPtr<T> el) override {
                add(*el);
            }
            void add(T& el) override {
                this->PrivateSet<T,U, AdditionPolicy, RemovalPolicy, OrderedSetNodeAllocationPolicy<T>>::add(el);
            }
            void remove(ID id) override {
                this->PrivateSet<T,U, AdditionPolicy, RemovalPolicy, OrderedSetNodeAllocationPolicy<T>>::remove(id);
            }
            void remove(T& el) override {
                remove(el.getID());
            }
            void remove (UmlPtr<T> el) override {
                remove(el.id());
            }

            OrderedSetIterator<T> begin() override {
                OrderedSetIterator<T> ret;
                ret.curr = this->m_first;
                ret.validSets = this->getAllSuperSets();
                ret.validSets.insert(this);
                return ret;
            }

            OrderedSetIterator<T> end() override {
                return OrderedSetIterator<T>();
            }
    };
}