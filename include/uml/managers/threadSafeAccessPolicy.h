#ifndef _UML_MANAGERS_THREAD_SAFE_ACCESS_POLICY_H_
#define _UML_MANAGERS_THREAD_SAFE_ACCESS_POLICY_H_

#include <mutex>
#include <uml/element.h>
#include "managerNode.h"
#include "abstractAccessPolicy.h"
#include "uml/umlPtr.h"

namespace UML {
    struct ThreadSafeManagerNode : public ManagerNode {
        ThreadSafeManagerNode(){};
        std::mutex m_mtx;
    };

    class ThreadSafeAccessPolicy : public AbstractAccessPolicy {
        protected:
            std::unordered_map<ID, ThreadSafeManagerNode> m_graph;
            std::mutex m_graphMtx;
            ManagerNode& assignNode(Element* newElement) {
                // lock graph for access
                std::lock_guard<std::mutex> graphLock(m_graphMtx);
                ThreadSafeManagerNode& node = m_graph[newElement->getID()];
                node.m_managerElementMemory = newElement;
                return node;
            }

            ElementPtr get(ID id) {
                // lock graph for access
                std::lock_guard<std::mutex> graphLock(m_graphMtx);
                std::unordered_map<ID, ThreadSafeManagerNode>::iterator result = m_graph.find(id);
                if (result != m_graph.end() && (*result).second.m_managerElementMemory) {
                    // lock node for creation
                    // std::lock_guard<std::mutex> nodeLock(static_cast<ThreadSafeManagerNode&>(result->second).m_mtx);
                    return ElementPtr(result->second.m_managerElementMemory);
                }
                return ElementPtr(0);
            }

            bool loaded(ID id) {
                // lock graph for access
                std::lock_guard<std::mutex> graphLock(m_graphMtx);
                std::unordered_map<ID, ThreadSafeManagerNode>::const_iterator result = m_graph.find(id);
                return result != m_graph.end() && (*result).second.m_managerElementMemory;
            }

            void restoreNode(ManagerNode* restoredNode) {
                // lock just node access
                // std::lock_guard<std::mutex> nodeLock(static_cast<ThreadSafeManagerNode*>(restoredNode)->m_mtx);
                for (auto& pair : restoredNode->m_references) {
                    ManagerNode* node = pair.second.node;
                    if (!node || !node->m_managerElementMemory) {
                        // element has been released, possibly there are no pointers
                        continue;
                    }
                    node->restoreReference(restoredNode->m_managerElementMemory);
                    restoredNode->restoreReference(node->m_managerElementMemory);
                }
                restoredNode->restoreReferences();
            }

            void eraseNode(ManagerNode* node, AbstractManager* me) {
                ID id = node->m_managerElementMemory->getID();
                {
                    // lock node
                    // std::lock_guard<std::mutex> nodeLck(static_cast<ThreadSafeManagerNode*>(node)->m_mtx);
                    std::vector<ManagerNode*> nodesToErase(node->m_references.size());
                    {
                        std::vector<ID> idsToAquire(node->m_references.size());
                        size_t i = 0;
                        for (auto& pair : node->m_references) {
                            if (!pair.second.node || !pair.second.node->m_managerElementMemory) {
                                // element has been released, aquire later
                                nodesToErase[i] = 0;
                                idsToAquire[i] = pair.first;
                            } else {
                                nodesToErase[i] = pair.second.node;
                            }
                            i++;
                        }
                        i = 0;
                        for (auto& refNode : nodesToErase) {
                            if (!refNode) {
                                refNode = getNode(*me->get(idsToAquire[i]));
                            }
                            i++;
                        }
                    }
                    for (auto& refNode : nodesToErase) {
                        if (!refNode) {
                            continue;
                        }
                        refNode->removeReference(*node->m_managerElementMemory);
                        refNode->referenceErased(id);
                        if (refNode->m_references.count(id)) {
                            refNode->m_references.erase(id);
                        }
                    }
                    node->erase();
                    delete node->m_managerElementMemory;
                }
                // lock graph for deletion
                std::lock_guard<std::mutex> graphLock(m_graphMtx);
                m_graph.erase(id);
            }

            void releaseNode(ManagerNode* node) {
                // std::lock_guard<std::mutex> nodeLck(static_cast<ThreadSafeManagerNode*>(node)->m_mtx);
                ID id = node->m_managerElementMemory->getID();
                for (auto& e : node->m_references) {
                    if (!e.second.node) {
                        // el has been released there are no pointers
                        continue;
                    }
                    if (e.second.node->m_managerElementMemory) {
                        e.second.node->releaseReference(id);
                    }
                }
                node->releasePtrs();
                node->m_managerElementMemory = 0;
                delete node->m_managerElementMemory;
                if (node->m_ptrs.empty()) {
                    std::lock_guard<std::mutex> graphLck(m_graphMtx);
                    m_graph.erase(id);
                }
            }

            void reindex(ID oldID, ID newID) {
                if (oldID == newID) {
                    return;
                }
                std::lock_guard<std::mutex> graphLck(m_graphMtx);
                if (m_graph.count(newID)) {
                    // Element with this ID already exists, overwrite it with new one
                    // This logic should only be called when it is loading from disk
                    // and will overwrite the existing element in memory with one from disk
                    // during a UmlManager::open() or UmlManager::aquire(id) invoke

                    ThreadSafeManagerNode* node = &m_graph[newID];
                    // std::lock_guard<std::mutex> nodeLck(node->m_mtx);
                    delete node->m_managerElementMemory;
                    ThreadSafeManagerNode& oldNode = m_graph[oldID];
                    node->m_managerElementMemory = oldNode.m_managerElementMemory;
                    setNode(node);
                    reindexReplace(oldID, newID, node, oldNode);
                    m_graph.erase(oldID);
                } else  {
                    // reindex
                    ThreadSafeManagerNode& discRef = m_graph[oldID];
                    // std::lock_guard<std::mutex> oldLock(discRef.m_mtx);

                    // copy over node
                    ThreadSafeManagerNode& newDisc = m_graph[newID];
                    newDisc.m_managerElementMemory = discRef.m_managerElementMemory;
                    for (auto& ptr: discRef.m_ptrs) {
                        newDisc.m_ptrs.push_back(ptr);
                    }
                    for (auto& refNode : discRef.m_references) {
                        newDisc.m_references[refNode.first] = refNode.second;
                    }
                    // std::lock_guard<std::mutex> newLock(newDisc.m_mtx);
                    reindexNoReplace(oldID, newID, &newDisc);
                    m_graph.erase(oldID);
                }
            }

            void removeNode(ID id) {
                std::lock_guard<std::mutex> graphLck(m_graphMtx);
                m_graph.erase(id);
            }
    };

}

#endif