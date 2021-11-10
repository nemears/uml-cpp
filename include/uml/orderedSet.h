#ifndef UML_ORDERED_SET
#define UML_ORDERED_SET

#include "set.h"

namespace UML {

    template <class T, class U> struct OrderedSetIterator;

    /**
     * This class implements a linkedList through the nodes of the weighted tree of the set
     * so that we can index by id, name and order in this container
     **/
    template <class T = Element, class U = Element> class OrderedSet : public Set<T,U> {

        friend struct OrderedSetIterator<T, U>;

        protected:
            struct OrderedNode : public AbstractSet::ContainerNode {
                OrderedNode(Element& el) : AbstractSet::ContainerNode(el) {};
                OrderedNode() {};
                OrderedNode* m_prev = 0;
                OrderedNode* m_next = 0;
            };
            OrderedNode* m_last = 0;
            OrderedNode* m_first = 0;
            OrderedNode* createOrderedNode(T& el) {
                return new OrderedNode(el);
            };
            OrderedNode* createOrderedNode(ID id) {
                OrderedNode* ret = new OrderedNode;
                ret->m_id = id;
                return ret;
            };
            void fillNode(OrderedNode* node) {
                if (!m_first) {
                    m_first = node;
                    for (AbstractSet* subSet : this->m_subsetOf) {
                        OrderedSet<T,U>* set = static_cast<OrderedSet<T,U>*>(subSet);
                        if (!set->m_first) {
                            set->m_first = node;
                        }
                    }
                    for (AbstractSet* redefined : this->m_redefines) {
                        OrderedSet<T,U>* set = static_cast<OrderedSet<T,U>*>(redefined);
                        if (!set->m_first) {
                            set->m_first = node;
                        }
                    }
                }
                if (m_last) {
                    ret->m_prev = m_last;
                    m_last->m_next = node;
                }
                m_last = node;
                for (AbstractSet* subSet : this->m_subsettedContainers) {
                    OrderedSet<T,U>* set = static_cast<OrderedSet<T,U>*>(subSet);
                    if (set) {
                        set->m_last = node;
                    } else {
                        throw ManagerStateException("TODO, orderedSet");
                    }
                }
                for (AbstractSet* owningSet : this->m_subsetOf) {
                    OrderedSet<T,U>* set = static_cast<OrderedSet<T,U>*>(owningSet);
                    if (set) {
                        set->m_last = node;
                    } else {
                        // actually i think this is okay
                        throw ManagerStateException("TODO, orderedSet");
                    }
                }
                for (AbstractSet* redefined : this->m_redefines) {
                    OrderedSet<T,U>* set = static_cast<OrderedSet<T,U>*>(redefined);
                    if (set) {
                        set->m_last = node;
                    } else {
                        throw ManagerStateException("TODO, orderedSet");
                    }
                }
            };
            AbstractSet::ContainerNode* createNode(T& el) override {
                OrderedNode* ret = createOrderedNode(el);
                fillNode(ret);
                return ret;
            };
            AbstractSet::ContainerNode* createNode(ID id) override {
                OrderedNode* ret = createOrderedNode(id);
                fillNode(ret);
                return ret;
            };
        public:
            T& front() {
                if (this->m_first) {
                    return *dynamic_cast<T*>(this->m_first->m_el);
                } else {
                    throw ManagerStateException("TODO, OrderedSet");
                }
            };
            T& back() {
                if (m_last) {
                    return *dynamic_cast<T*>(m_last->m_el);
                } else {
                    throw ManagerStateException("TODO, OrderedSet");
                }
            };
            T& get(int index) {
                /**
                 * TODO: this can be faster if we traverse tree maybe?
                 **/
                OrderedSetIterator it = begin();
                for (int i = 0; i < index; i++) {
                    it++;
                }
                return *it;
            };
            T& get(ID id) {return Set<T, U>::get(id);};
            T& get(std::string name) {return Set<T, U>::get(name);};
            OrderedSetIterator<T,U> begin() {
                OrderedSetIterator<T,U> ret;
                ret.m_node = static_cast<OrderedNode*>(this->m_first);
                ret.m_guard = this->m_guard;
                return ret;
            };
            OrderedSetIterator<T,U> end() {
                OrderedSetIterator<T,U> ret;
                ret.m_node = &ret.m_endNode;
                ret.m_guard = this->m_guard;
                return ret;
            };
    };

    template <class T = Element, class U = Element> struct OrderedSetIterator {

        friend class OrderedSet<T,U>;
        using NodeType = typename OrderedSet<T,U>::OrderedNode;

        private:
            NodeType* m_node;
            NodeType m_endNode;
            int m_guard;
        public:
            OrderedSetIterator() {};
            T& operator*() { return dynamic_cast<T&>(*m_node->m_el); };
            T* operator->() { return dynamic_cast<T*>(m_node->m_el); };
            OrderedSetIterator operator++() {
                if (m_node->m_next) {
                    NodeType* temp = m_node;
                    do {
                        if (m_node->m_next) {
                            m_node = m_node->m_next;
                        } else {
                            m_node = &m_endNode;
                            break;
                        }
                    } while (m_node->m_guard < m_guard);
                } else {
                    m_node = &m_endNode;
                }
                return *this;
            };
            OrderedSetIterator operator++(int) {
                if (m_node->m_next) {
                    NodeType* temp = m_node;
                    do {
                        if (m_node->m_next) {
                            m_node = m_node->m_next;
                        } else {
                            m_node = &m_endNode;
                            break;
                        }
                    } while (m_node->m_guard < m_guard);
                } else {
                    m_node = &m_endNode;
                }
                OrderedSetIterator ret = *this;
                return ret;
            };
            friend bool operator== (const OrderedSetIterator& a, const OrderedSetIterator& b) { return a.m_node->m_id == b.m_node->m_id; };
            friend bool operator!= (const OrderedSetIterator& a, const OrderedSetIterator& b) { return a.m_node->m_id != b.m_node->m_id; };
    };
}

#endif