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
            OrderedSetIterator& operator++() {
                do {
                    curr = curr->m_next;
                } while (curr && !validSets.count(curr->set));
                return *this;
            }
            OrderedSetIterator operator++(int) {
                return ++(*this);
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

    class OrderedSetNodeAllocationPolicyInterface : virtual public AbstractSet {

        template <class T> friend class OrderedSetNodeAllocationPolicy;

        protected:
            virtual OrderedSetNode* getFront() const = 0;
            virtual OrderedSetNode* getBack() const = 0;
            virtual void setFront(OrderedSetNode* front) = 0;
            virtual void setBack(OrderedSetNode* back) = 0;
    };

    template <class T>
    class OrderedSetNodeAllocationPolicy : public OrderedSetNodeAllocationPolicyInterface {
        // this policy implements a linked list on top of the regular set tree
        protected:
            OrderedSetNode* m_first = 0;
            OrderedSetNode* m_last = 0;

            OrderedSetNode* getFront() const override {
                return m_first;
            }

            OrderedSetNode* getBack() const override {
                return m_last;
            }

            void setFront(OrderedSetNode* front) override {
                m_first = front;
            }

            void setBack(OrderedSetNode* back) override {
                m_last = back;
            }

            SetType setType() const override {
                return SetType::ORDERED_SET;
            }
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
            SetNode* create(UmlPtr<T> el) {
                OrderedSetNode* ret = new OrderedSetNode();
                ret->m_ptr = el;
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
            void adjustSuperSets(SetNode* node, std::unordered_set<AbstractSet*> allSuperSetsAndMe) override {
                for (AbstractSet* set: allSuperSetsAndMe) {
                    if (set->setType() == SetType::ORDERED_SET) {
                        OrderedSetNodeAllocationPolicyInterface* orderedSet = dynamic_cast<OrderedSetNodeAllocationPolicyInterface*>(set);
                        if (!orderedSet->getFront()) {
                            orderedSet->setFront(static_cast<OrderedSetNode*>(node));
                        }
                        if (!orderedSet->getBack()) {
                            orderedSet->setBack(static_cast<OrderedSetNode*>(node));
                        }
                    }
                }
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
            virtual bool contains(ID id) const = 0;
            virtual bool contains(T& el) const = 0;
            virtual bool contains(UmlPtr<T> el) const = 0;
            virtual bool contains(std::string name) const = 0;
            virtual T& get(ID id) const = 0;
            virtual T& get(std::string name) const = 0;
            virtual bool empty() const = 0;
            virtual size_t size() const = 0;
            virtual T& get(size_t index) const = 0;
            virtual T& front() = 0;
            virtual T& back() = 0;
            virtual void add(UmlPtr<T> el) = 0;
            virtual void add(T& el) = 0;
            template <class ... Ts>
            void add(T& el, Ts&... els) {
                add(el);
                add(els...);
            }
            virtual void add(ID id) = 0;
            virtual void remove(ID id) = 0;
            virtual void remove(T& el) = 0;
            virtual void remove(UmlPtr<T> el) = 0;
            virtual void clear() = 0;
            virtual OrderedSetIterator<T> begin() const = 0;
            virtual OrderedSetIterator<T> end() const = 0;
            virtual ID_Set<T> ids() = 0;
    };

    template <
                class T, 
                class U,
                class AdditionPolicy = DoNothingAdd<T, U>,
                class RemovalPolicy = DoNothingRemove<T, U>
            >
    class CustomOrderedSet : public PrivateSet<T, U, AdditionPolicy, RemovalPolicy, OrderedSetNodeAllocationPolicy<T>> , public OrderedSet<T,U> {
        public:
            CustomOrderedSet(U& el) : PrivateSet<T, U, AdditionPolicy, RemovalPolicy, OrderedSetNodeAllocationPolicy<T>>(el) {}
            CustomOrderedSet(U* el) : PrivateSet<T, U, AdditionPolicy, RemovalPolicy, OrderedSetNodeAllocationPolicy<T>>(el) {}
            bool contains(ID id) const override {
                return PrivateSet<T,U, AdditionPolicy, RemovalPolicy, OrderedSetNodeAllocationPolicy<T>>::contains(id);
            }
            bool contains(T& el) const override {
                return PrivateSet<T,U, AdditionPolicy, RemovalPolicy, OrderedSetNodeAllocationPolicy<T>>::contains(el);
            }
            bool contains(UmlPtr<T> el) const override {
                return PrivateSet<T,U, AdditionPolicy, RemovalPolicy, OrderedSetNodeAllocationPolicy<T>>::contains(el);
            }
            bool contains(std::string name) const override {
                return PrivateSet<T,U, AdditionPolicy, RemovalPolicy, OrderedSetNodeAllocationPolicy<T>>::contains(name);
            }
            T& get(ID id) const override {
                return PrivateSet<T,U, AdditionPolicy, RemovalPolicy, OrderedSetNodeAllocationPolicy<T>>::get(id);
            }
            T& get(std::string name) const override {
                return PrivateSet<T,U, AdditionPolicy, RemovalPolicy, OrderedSetNodeAllocationPolicy<T>>::get(name);
            }
            bool empty() const override {
                return PrivateSet<T,U, AdditionPolicy, RemovalPolicy, OrderedSetNodeAllocationPolicy<T>>::empty();
            }
            size_t size() const override {
                return PrivateSet<T,U, AdditionPolicy, RemovalPolicy, OrderedSetNodeAllocationPolicy<T>>::size();
            }
            T& get(size_t index) const override {
                OrderedSetIterator it = this->begin();
                for (size_t i = 0; i < index; i++) {
                    it++;
                }
                return *it;
            }
            T& front() override {
                [[maybe_unused]] SetLock myLock = this->m_el.m_manager->lockEl(this->m_el);
                if (!this->m_first) {
                    throw SetStateException("front is null");
                }
                return this->m_first->m_ptr->template as<T>();
            }
            T& back() override {
                [[maybe_unused]] SetLock myLock = this->m_el.m_manager->lockEl(this->m_el);
                if (!this->m_last) {
                    throw SetStateException("last is null");
                }
                return this->m_last->m_ptr->template as<T>();
            }
            void add(UmlPtr<T> el) override {
                add(*el);
            }
            void add(T& el) override {
                this->PrivateSet<T,U, AdditionPolicy, RemovalPolicy, OrderedSetNodeAllocationPolicy<T>>::add(el);
            }
            void add(ID id) override {
                PrivateSet<T,U, AdditionPolicy, RemovalPolicy, OrderedSetNodeAllocationPolicy<T>>::add(id);
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
            void clear() override {
                PrivateSet<T,U, AdditionPolicy, RemovalPolicy, OrderedSetNodeAllocationPolicy<T>>::clear();
            }
            OrderedSetIterator<T> begin() const override {
                OrderedSetIterator<T> ret;
                ret.curr = this->m_first;
                ret.validSets = this->getAllSubSets();
                ret.validSets.insert(const_cast<AbstractSet*>(static_cast<const AbstractSet*>(this))); // I <3 c++
                return ret;
            }

            OrderedSetIterator<T> end() const override {
                return OrderedSetIterator<T>();
            }

            ID_Set<T> ids() override {
                ID_Set<T> ret;
                ret.root = this->m_root;
                ret.validSets = this->getAllSubSets();
                ret.validSets.insert(this);
                return ret;
            };
    };
}