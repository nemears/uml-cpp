#ifndef _UML_MANAGERS_SIMPLE_ACCESS_POLICY_H_
#define _UML_MANAGERS_SIMPLE_ACCESS_POLICY_H_

#include "abstractAccessPolicy.h"

namespace UML {
    class SimpleAccessPolicy : public AbstractAccessPolicy {
        private:
            std::unordered_map<ID, ManagerNode> m_graph;
        protected:
            template <class T = Element>
            UmlPtr<T> create() {
                T* ptr = createPtr<T>();
                m_graph[ptr->getID()] = {ptr};
                setNode(&m_graph[ptr->getID()]);
                UmlPtr<T> ret(ptr);
                return ret;
            }

            void assignNode(Element* newElement) {
                m_graph[newElement->getID()] = {newElement};
                setNode(&m_graph[newElement->getID()]);
            }

            ElementPtr get(ID id) {
                std::unordered_map<ID, ManagerNode>::const_iterator result = m_graph.find(id);
                if (result != m_graph.end() && (*result).second.m_managerElementMemory) {
                    return ElementPtr(result->second.m_managerElementMemory);
                }
                return ElementPtr(0);
            }

            void restoreNode(Element& el) {
                restore(el);
            }

            void releaseNode(Element& el) {
                ID elID = el.getID();
                ManagerNode* node = AbstractAccessPolicy::releaseNode(el);
                if (node->m_ptrs.empty()) {
                    m_graph.erase(elID);
                }
            }

            void reindex(ID oldID, ID newID) {
                if (oldID == newID) {
                    return;
                }
                if (m_graph.count(newID)) {
                    // Element with this ID already exists, overwrite it with new one
                    // This logic should only be called when it is loading from disk
                    // and will overwrite the existing element in memory with one from disk
                    // during a UmlManager::open() or UmlManager::aquire(id) invoke

                    ManagerNode* m_node = &m_graph[newID];
                    delete m_node->m_managerElementMemory;
                    ManagerNode& oldNode = m_graph[oldID];
                    m_node->m_managerElementMemory = oldNode.m_managerElementMemory;
                    setNode(m_node);
                    // m_node->m_managerElementMemory->m_node = m_node;
                    reindexReplace(oldID, newID, m_node, oldNode);
                    m_graph.erase(oldID);
                } else  {
                    // reindex
                    ManagerNode& discRef = m_graph[oldID];
                    m_graph[newID] = std::move(discRef);
                    ManagerNode* newDisc = &m_graph[newID];
                    reindexNoReplace(oldID, newID, newDisc);
                    m_graph.erase(oldID);
                    // if (!m_mountBase.empty()) {
                    //     std::filesystem::remove(m_mountBase / "mount" / (oldID.string() + ".yml"));
                    //     /** Should we write to new file now?**/
                    // }
                }
            }

            void removeNode(ID id) {
                m_graph.erase(id);
            }
    };
}

#endif