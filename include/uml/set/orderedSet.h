#pragma once

#include "set.h"

namespace UML {

    struct OrderedSetNode;

    class OrderedSetNodeAllocationPolicyInterface : virtual public AbstractSet {

        friend class OrderedSetNodeAllocationPolicy;
        template <class T> 
        friend class OrderedID_Set;
        template <class T>
        friend class OrderedPtrSet;
        friend struct OrderedSetNode;

        protected:
            virtual OrderedSetNode* getFront() const = 0;
            virtual OrderedSetNode* getBack() const = 0;
            virtual void setFront(OrderedSetNode* front) = 0;
            virtual void setBack(OrderedSetNode* back) = 0;
    };

    class OrderedSetNodeAllocationPolicy;
    template <class T>
    class OrderedPtrSet;
    template <class T>
    class OrderedSetIterator; 

    struct OrderedSetNode : public SetNode {

        friend class OrderedSetNodeAllocationPolicy;
        template <class T>
        friend class OrderedPtrSet;
        template <class T>
        friend class OrderedSetIterator;

        OrderedSetNode* m_prev = 0;
        OrderedSetNode* m_next = 0;
        virtual ~OrderedSetNode() {
            // find root orderedSet
            OrderedSetNodeAllocationPolicyInterface* orderedSet = dynamic_cast<OrderedSetNodeAllocationPolicyInterface*>(set);
            if (!orderedSet) {
                for (auto& redefinedSet : set->m_redefines) {
                    orderedSet = dynamic_cast<OrderedSetNodeAllocationPolicyInterface*>(redefinedSet);
                    if (orderedSet) {
                        break;
                    }
                }
            }
            if (!orderedSet) {
                for (auto& superSet : set->getAllSuperSets()) {
                    orderedSet = dynamic_cast<OrderedSetNodeAllocationPolicyInterface*>(superSet);
                    if (orderedSet) {
                        break;
                    }
                }
            }

            // TODO understand this edge case (orderedSetNode is not part of an ordered set)
            // happens with ClassTest.addOwnedAttributeAsStructuredClassifierTest
            if (!orderedSet) {
                // throw SetStateException("bad state, non orderedSetNode tried to add to orderedSet!");
                return;
            }

            if (orderedSet->getFront() == this) {
                orderedSet->setFront(m_next);
            }
            if (orderedSet->getBack() == this) {
                orderedSet->setBack(m_prev);
            }
            
            // check if supersets are orderedSets to clear adjust their front and back
            for (auto& superSet : set->getAllSuperSets()) {
                OrderedSetNodeAllocationPolicyInterface* orderedSuperSet = dynamic_cast<OrderedSetNodeAllocationPolicyInterface*>(superSet);
                    if (orderedSuperSet) {
                        if (orderedSuperSet->getFront() == this) {
                            orderedSuperSet->setFront(m_next);
                        }
                        if (orderedSuperSet->getBack() == this) {
                            orderedSuperSet->setBack(m_prev);
                        }
                    }
            }

            if (m_prev) {
                m_prev->m_next = m_next;
            }
            if (m_next) {
                m_next->m_prev = m_prev;
            }
        }
    };

    template<class V, class W, class AdditionPolicy, class RemovalPolicy> class CustomOrderedSet;
    template <class T> class OrderedPtrSet;

    template <class T>
    class OrderedSetIterator {

        template<class V, class W, class AdditionPolicy, class RemovalPolicy> friend class CustomOrderedSet;
        template <class V> friend class OrderedPtrSet;

        protected:
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
                if (!lhs.curr && !rhs.curr) {
                    return true;
                }
                if (!lhs.curr || !rhs.curr) {
                    return false;
                }
                return lhs.curr == rhs.curr;
            }
            friend bool operator!= (const OrderedSetIterator& lhs, const OrderedSetIterator& rhs) {
                if (!lhs.curr && !rhs.curr) {
                    return false;
                }
                if (!lhs.curr || !rhs.curr) {
                    return true;
                }
                return lhs.curr != rhs.curr;
            }
    };

    template <class T>
    class OrderedID_Set;

    template <class T>
    class OrderedSetID_Iterator : public OrderedSetIterator<T> {
        friend class OrderedID_Set<T>;
        public:
            ID operator*() {
                return this->curr->m_ptr.id();
            }
            UmlPtr<T> operator->() = delete;
    };

    template <class T>
    class OreredPtrSet;

    template <class T>
    class OrderedID_Set {

        template <class V, class W, class AdditionPolicy, class RemovalPolicy> friend class CustomReadOnlySet;
        template <class V, class W, class AdditionPolicy, class RemovalPolicy> friend class CustomOrderedSet;

        private:
            OrderedSetNodeAllocationPolicyInterface* m_me = 0;
        public:
            OrderedSetID_Iterator<T> begin() {
                OrderedSetID_Iterator<T> ret;
                ret.curr = m_me->getFront();
                ret.validSets = m_me->getAllSubSets();
                ret.validSets.insert(m_me);
                for (auto& redefinedSet : m_me->m_redefines) {
                    ret.validSets.insert(redefinedSet);
                }
                return ret;
            };
            OrderedSetID_Iterator<T> end() {
                return OrderedSetID_Iterator<T>();
            };
            ID front() const {
                SetNode* curr = m_me->getFront();
                if (!curr) {
                    throw SetStateException("Cannot get front element because set is empty!");
                }

                return curr->m_ptr.id();
            }
            ID back() const {
                SetNode* curr = m_me->getBack();
                if (!curr) {
                    throw SetStateException("Cannot get back element because set is empty!");
                }

                return curr->m_ptr.id();
            }
    };

    template <class T>
    class OrderedSetPtrIterator : public OrderedSetIterator<T> {
        public:
            UmlPtr<T> operator*() {
                return this->curr->m_ptr;
            }
    };

    template <class T>
    class OrderedPtrSet {

        template <class V, class W, class AdditionPolicy, class RemovalPolicy> friend class CustomReadOnlySet;
        template <class V, class W, class AdditionPolicy, class RemovalPolicy> friend class CustomOrderedSet;

        private:
            OrderedSetNodeAllocationPolicyInterface* m_me = 0;
        public:
            OrderedSetPtrIterator<T> begin() {
                OrderedSetPtrIterator<T> ret;
                ret.curr = m_me->getFront();
                ret.validSets = m_me->getAllSubSets();
                ret.validSets.insert(m_me);
                for (auto& redefinedSet : m_me->m_redefines) {
                    ret.validSets.insert(redefinedSet);
                }
                return ret;
            };
            OrderedSetPtrIterator<T> end() {
                return OrderedSetPtrIterator<T>();
            };
            UmlPtr<T> front() const {
                SetNode* curr = m_me->getFront();
                if (!curr) {
                    throw SetStateException("Cannot get front element because set is empty!");
                }

                return curr->m_ptr;
            }
            UmlPtr<T> back() const {
                SetNode* curr = m_me->getBack();
                if (!curr) {
                    throw SetStateException("Cannot get back element because set is empty!");
                }

                return curr->m_ptr;
            }
    };

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
            SetNode* create(Element& el) {
                OrderedSetNode* ret = new OrderedSetNode();
                ret->m_ptr = &el;
                return ret;
            }
            template <class T>
            SetNode* create(UmlPtr<T> el) {
                OrderedSetNode* ret = new OrderedSetNode();
                ret->m_ptr = el;
                return ret;
            }
            void adjustSuperSets(SetNode* node, std::unordered_set<AbstractSet*>& allSuperSetsAndMe) override {
                OrderedSetNode* orderedSetNode = static_cast<OrderedSetNode*>(node);
                // todo maybe do this bfs instead of just giving the list
                for (AbstractSet* set: allSuperSetsAndMe) {
                    if (set->setType() != SetType::ORDERED_SET) {
                        continue;
                    }
                    OrderedSetNodeAllocationPolicyInterface* orderedSet = dynamic_cast<OrderedSetNodeAllocationPolicyInterface*>(set);

                    if (!orderedSet->getFront()) {
                        orderedSet->setFront(orderedSetNode);
                    }

                    if (orderedSet->getBack() && orderedSetNode->m_prev != orderedSet->getBack() && orderedSetNode != orderedSet->getBack()) {
                        orderedSetNode->m_prev = orderedSet->getBack();
                        orderedSetNode->m_prev->m_next = orderedSetNode;                        
                    }
                    orderedSet->setBack(orderedSetNode);
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
            virtual OrderedID_Set<T> ids() = 0;
            virtual OrderedPtrSet<T> ptrs() = 0; 
    };

    template <
                class T, 
                class U,
                class AdditionPolicy = DoNothingAdd<T, U>,
                class RemovalPolicy = DoNothingRemove<T, U>
            >
    class CustomOrderedSet : public PrivateSet<T, U, AdditionPolicy, RemovalPolicy, OrderedSetNodeAllocationPolicy> , public OrderedSet<T,U> {
        public:
            CustomOrderedSet(U& el) : PrivateSet<T, U, AdditionPolicy, RemovalPolicy, OrderedSetNodeAllocationPolicy>(el) {}
            CustomOrderedSet(U* el) : PrivateSet<T, U, AdditionPolicy, RemovalPolicy, OrderedSetNodeAllocationPolicy>(el) {}
            bool contains(ID id) const override {
                return PrivateSet<T,U, AdditionPolicy, RemovalPolicy, OrderedSetNodeAllocationPolicy>::contains(id);
            }
            bool contains(T& el) const override {
                return PrivateSet<T,U, AdditionPolicy, RemovalPolicy, OrderedSetNodeAllocationPolicy>::contains(el);
            }
            bool contains(UmlPtr<T> el) const override {
                return PrivateSet<T,U, AdditionPolicy, RemovalPolicy, OrderedSetNodeAllocationPolicy>::contains(el);
            }
            bool contains(std::string name) const override {
                return PrivateSet<T,U, AdditionPolicy, RemovalPolicy, OrderedSetNodeAllocationPolicy>::contains(name);
            }
            T& get(ID id) const override {
                return PrivateSet<T,U, AdditionPolicy, RemovalPolicy, OrderedSetNodeAllocationPolicy>::get(id);
            }
            T& get(std::string name) const override {
                return PrivateSet<T,U, AdditionPolicy, RemovalPolicy, OrderedSetNodeAllocationPolicy>::get(name);
            }
            bool empty() const override {
                return PrivateSet<T,U, AdditionPolicy, RemovalPolicy, OrderedSetNodeAllocationPolicy>::empty();
            }
            size_t size() const override {
                return PrivateSet<T,U, AdditionPolicy, RemovalPolicy, OrderedSetNodeAllocationPolicy>::size();
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
                this->PrivateSet<T,U, AdditionPolicy, RemovalPolicy, OrderedSetNodeAllocationPolicy>::add(el);
            }
            void add(ID id) override {
                PrivateSet<T,U, AdditionPolicy, RemovalPolicy, OrderedSetNodeAllocationPolicy>::add(id);
            }
            void remove(ID id) override {
                this->PrivateSet<T,U, AdditionPolicy, RemovalPolicy, OrderedSetNodeAllocationPolicy>::remove(id);
            }
            void remove(T& el) override {
                remove(el.getID());
            }
            void remove (UmlPtr<T> el) override {
                remove(el.id());
            }
            void clear() override {
                PrivateSet<T,U, AdditionPolicy, RemovalPolicy, OrderedSetNodeAllocationPolicy>::clear();
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

            OrderedID_Set<T> ids() override {
                OrderedID_Set<T> ret;
                ret.m_me = this;
                return ret;
            };

            OrderedPtrSet<T> ptrs() override {
                OrderedPtrSet<T> ret;
                ret.m_me = this;
                return ret;
            }
    };
}
