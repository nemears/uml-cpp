#ifndef UML_ORDERED_SET
#define UML_ORDERED_SET

#include "set.h"

namespace UML {
    template <class T = Element, class U = Element> class OrderedSet : public Set<T,U> {
        protected:
            struct OrderedNode : public AbstractContainer::ContainerNode {
                OrderedNode(Element& el) : AbstractContainer::ContainerNode(el) {};
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
    };
}

#endif