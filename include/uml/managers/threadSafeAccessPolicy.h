#ifndef _UML_MANAGERS_THREAD_SAFE_ACCESS_POLICY_H_
#define _UML_MANAGERS_THREAD_SAFE_ACCESS_POLICY_H_

#include <mutex>
#include <uml/element.h>

namespace UML {
    struct ThreadSafeManagerNode : public ManagerNode {
        std::mutex m_mtx;
    };

    class ThreadSafeAccessPolicy : public AbstractAccessPolicy {
        private:
            std::unordered_set<ID> m_elements;
            std::unordered_map<ID, ThreadSafeManagerNode> m_graph;
            std::mutex m_accessMtx;
        protected:
            template <class T = Element>
            UmlPtr<T> create() {
                std::lock_guard<std::mutex> accessLck(m_accessMtx);
                // T* ptr = createPtr<T>();
                m_elements.insert(ptr->getID());
                m_graph[ptr->getID()] = {ptr};
                setNode(&m_graph[ptr->getID()]);
                UmlPtr<T> ret(ptr);
                return ret;
            }
    };

}

#endif