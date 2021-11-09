#ifndef UML_ORDERED_SET
#define UML_ORDERED_SET

#include "set.h"

namespace UML {

    template <class T, class U> struct OrderedSetIterator;

    template <class T = Element, class U = Element> class OrderedSet : public Set<T,U> {

        friend struct OrderedSetIterator<T, U>;

        protected:
            struct OrderedNode : public AbstractContainer::ContainerNode {
                OrderedNode(Element& el) : AbstractContainer::ContainerNode(el) {};
                OrderedNode() {};
                OrderedNode* m_prev = 0;
                OrderedNode* m_next = 0;
            };
            OrderedNode* m_last = 0;
            AbstractContainer::ContainerNode* createNode(T& el) override {
                OrderedNode* ret = new OrderedNode(el);
                if (m_last) {
                    ret->m_prev = m_last;
                    m_last->m_next = ret;
                }
                m_last = ret;
                return ret;
            };
        public:
            T& front() {
                if (this->m_root) {
                    return *dynamic_cast<T*>(this->m_root->m_el);
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
                ret.m_node = static_cast<OrderedNode*>(this->m_root);
                return ret;
            };
            OrderedSetIterator<T,U> end() {
                OrderedSetIterator<T,U> ret;
                ret.m_node = &ret.m_endNode;
                return ret;
            };
    };

    template <class T = Element, class U = Element> struct OrderedSetIterator {

        friend class OrderedSet<T,U>;
        using NodeType = typename OrderedSet<T,U>::OrderedNode;

        private:
            NodeType* m_node;
            NodeType m_endNode;
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
                    } while (m_node->m_guard < temp->m_guard);
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
                    } while (m_node->m_guard < temp->m_guard);
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