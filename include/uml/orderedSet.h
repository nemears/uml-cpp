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
            struct OrderedNode : public AbstractSet::SetNode {
                inline OrderedNode(Element& el) : AbstractSet::SetNode(&el) {};
                OrderedNode() {};
                OrderedNode* m_prev = 0;
                OrderedNode* m_next = 0;
            };
            OrderedNode* m_last = 0;
            OrderedNode* m_first = 0;
            inline OrderedNode* createOrderedNode(T& el) {
                return new OrderedNode(el);
            };
            inline OrderedNode* createOrderedNode(ID id) {
                OrderedNode* ret = new OrderedNode;
                ret->m_id = id;
                return ret;
            };
            inline void fillNode(OrderedNode* node) {
                if (!m_first) {
                    m_first = node;
                    for (AbstractSet* subSet : this->m_superSets) {
                        if (subSet->m_upper == -1) { // compare types (ordered set will be two bytes bigger than set and singleton)
                            OrderedSet<T,U>* set = static_cast<OrderedSet<T,U>*>(subSet);
                            if (!set->m_first) {
                                set->m_first = node;
                            }
                        }
                    }
                    for (AbstractSet* redefined : this->m_redefines) {
                        if (redefined->m_upper == -1) {
                            OrderedSet<T,U>* set = static_cast<OrderedSet<T,U>*>(redefined);
                            if (!set->m_first) {
                                set->m_first = node;
                            }
                        }
                    }
                }
                if (m_last) {
                    node->m_prev = m_last;
                    m_last->m_next = node;
                }
                m_last = node;
                for (AbstractSet* subSet : this->m_subSets) {
                    if (subSet->m_upper == -1) {
                        OrderedSet<T,U>* set = static_cast<OrderedSet<T,U>*>(subSet);
                        if (set) {
                            set->m_last = node;
                        } else {
                            throw ManagerStateException("TODO, orderedSet");
                        }
                    }
                }
                for (AbstractSet* owningSet : this->m_superSets) {
                    if (owningSet->m_upper == -1) {
                        OrderedSet<T,U>* set = static_cast<OrderedSet<T,U>*>(owningSet);
                        if (set) {
                            set->m_last = node;
                        } else {
                            // actually i think this is okay
                            throw ManagerStateException("TODO, orderedSet");
                        }
                    }
                }
                for (AbstractSet* redefined : this->m_redefines) {
                    if (redefined->m_upper == -1) {
                        OrderedSet<T,U>* set = static_cast<OrderedSet<T,U>*>(redefined);
                        if (set) {
                            set->m_last = node;
                        } else {
                            throw ManagerStateException("TODO, orderedSet");
                        }
                    }
                }
            };
            inline AbstractSet::SetNode* createNode(T& el) override {
                AbstractSet::SetNode* temp = this->lookForNodeInParents(el.getID());
                if (temp) {
                    // TODO change to orderedNode?
                    return temp;
                }
                OrderedNode* ret = createOrderedNode(el);
                ret->m_guard = this->m_guard;
                fillNode(ret);
                return ret;
            };
            inline AbstractSet::SetNode* createNode(ID id) override {
                AbstractSet::SetNode* temp = this->lookForNodeInParents(id);
                if (temp) {
                    // TODO change to orderedNode?
                    return temp;
                }
                OrderedNode* ret = createOrderedNode(id);
                ret->m_guard = this->m_guard;
                fillNode(ret);
                return ret;
            };
            void deleteNode(AbstractSet::SetNode* node) override {
                OrderedNode* oNode = static_cast<OrderedNode*>(node);
                if (m_first->m_id == oNode->m_id) {
                    if (oNode->m_next) {
                        m_first = oNode->m_next;
                    } else {
                        m_first = 0;
                    }
                }
                if (m_last->m_id == oNode->m_id) {
                    if (oNode->m_prev) {
                        m_last = oNode->m_prev;
                    } else {
                        m_last = 0;
                    }
                }
                if (oNode->m_prev) {
                    if (oNode->m_next) {
                        oNode->m_prev->m_next = oNode->m_next;
                    } else {
                        oNode->m_prev->m_next = 0;
                    }
                }
                if (oNode->m_next) {
                    if (oNode->m_prev) {
                        oNode->m_next->m_prev = oNode->m_prev;
                    } else {
                        oNode->m_next->m_prev = 0;
                    }
                }
                Set<T,U>::deleteNode(node);
            };
        public:
            inline OrderedSet<T,U>(U* el) : Set<T,U>(el) {
                this->m_upper = -1;
            };
            inline OrderedSet<T,U>() : Set<T,U>() {
                this->m_upper = -1;
            };
            T& front() {
                if (this->m_first) {
                    if (!m_first->m_el) {
                        m_first->m_el = &this->m_el->m_manager->get(m_first->m_id);
                    }
                    return *dynamic_cast<T*>(this->m_first->m_el);
                } else {
                    throw ManagerStateException("TODO, OrderedSet");
                }
            };
            T& back() {
                if (m_last) {
                    if (!m_last->m_el) {
                        m_last->m_el = &this->m_el->m_manager->get(m_last->m_id);
                    }
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
                ret.m_el = this->m_el;
                if (m_first) {
                    ret.m_node = static_cast<OrderedNode*>(this->m_first);
                } else {
                    ret.m_node = &ret.m_endNode;
                }
                ret.m_guard = this->m_guard;
                return ret;
            };
            OrderedSetIterator<T,U> end() {
                OrderedSetIterator<T,U> ret;
                ret.m_el = this->m_el;
                ret.m_node = &ret.m_endNode;
                ret.m_guard = this->m_guard;
                return ret;
            };
    };

    template <class T = Element, class U = Element> struct OrderedSetIterator {

        friend class OrderedSet<T,U>;
        using NodeType = typename OrderedSet<T,U>::OrderedNode;

        private:
            Element* m_el = 0;
            NodeType* m_node;
            NodeType m_endNode;
            int m_guard;
        public:
            OrderedSetIterator() {};
            T& operator*() {
                if (!m_node->m_el) {
                    m_node->m_el = &m_el->m_manager->get(m_node->m_id);
                }
                return dynamic_cast<T&>(*m_node->m_el);
            };
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