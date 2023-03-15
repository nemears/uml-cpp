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
        ThreadSafeManagerNode& operator=(const ThreadSafeManagerNode& rhs) {
            return static_cast<ThreadSafeManagerNode&>(ManagerNode::operator=(rhs));
        };
        std::mutex m_mtx;
        std::mutex m_ptrsMtx;
    };

    struct ThreadSafeSetLock : public SetLock {

        std::unique_lock<std::mutex> m_lck;

        ThreadSafeSetLock(ThreadSafeManagerNode& node) : m_lck(node.m_mtx) {
            
        }
    };

    class ThreadSafeAccessPolicy : public AbstractAccessPolicy {
        protected:
            std::unordered_map<ID, ThreadSafeManagerNode> m_graph;
            std::mutex m_graphMtx;

            void clear() {
                // std::lock_guard<std::mutex> graphLock(m_graphMtx);
                for (auto& pair : m_graph) {
                    delete pair.second.m_managerElementMemory;
                    pair.second.erase();
                }
                m_graph.clear();
            }

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
                    return ElementPtr(result->second.m_managerElementMemory);
                }
                return ElementPtr(0);
            }

            bool exists(ID id) {
                // lock graph for access
                std::lock_guard<std::mutex> graphLock(m_graphMtx);
                return m_graph.count(id);
            }

            bool loaded(ID id) {
                // lock graph for access
                std::lock_guard<std::mutex> graphLock(m_graphMtx);
                std::unordered_map<ID, ThreadSafeManagerNode>::const_iterator result = m_graph.find(id);
                return result != m_graph.end() && (*result).second.m_managerElementMemory;
            }

            void restoreNode(ManagerNode* restoredNode) {
                // lock just node access
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
                        // refNode->removeReference(*node->m_managerElementMemory);
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
                ID id = node->m_managerElementMemory->getID();
                for (auto& e : node->m_references) {
                    if (!e.second.node) {
                        // el has been released there are no pointers
                        continue;
                    }
                    // if (e.second.node->m_managerElementMemory) {
                    //     e.second.node->releaseReference(id);
                    // }
                }
                node->releasePtrs();
                delete node->m_managerElementMemory;
                node->m_managerElementMemory = 0;
                if (node->m_ptrs.empty()) {
                    std::lock_guard<std::mutex> graphLck(m_graphMtx);
                    m_graph.erase(id);
                }
            }

            void overwrite(ID oldID, ID newID) {
                // lock graph for edit
                std::lock_guard<std::mutex> graphLock(m_graphMtx);

                // overwrite
                std::unordered_map<ID, ThreadSafeManagerNode>::iterator nodeToBeOverwrittenIterator = m_graph.find(newID);
                ThreadSafeManagerNode& nodeToBeOverwritten = nodeToBeOverwrittenIterator->second;
                ThreadSafeManagerNode& newNode = m_graph[oldID];
                if (nodeToBeOverwritten.m_managerElementMemory) {
                    delete nodeToBeOverwritten.m_managerElementMemory;
                }
                nodeToBeOverwritten.m_managerElementMemory = newNode.m_managerElementMemory;
                setNode(&nodeToBeOverwritten);
                nodeToBeOverwritten.m_references.clear();
                nodeToBeOverwritten.reindexPtrs(newID);
                for (auto& ptr : newNode.m_ptrs) {
                    nodeToBeOverwritten.assingPtr(ptr);
                    assignPtr(*ptr);
                }
                m_graph.erase(oldID);
            }

            void reindex(ID oldID, ID newID) {
                // lock graph for edit
                std::lock_guard<std::mutex> graphLock(m_graphMtx);

                // reindex
                ThreadSafeManagerNode& discRef = m_graph[oldID];
                ThreadSafeManagerNode& newDisc = m_graph[newID] = discRef;
                reindexNoReplace(oldID, newID, &newDisc);
                m_graph.erase(oldID);
            }

            void removeNode(ID id) {
                std::lock_guard<std::mutex> graphLck(m_graphMtx);
                m_graph.erase(id);
            }
            
            virtual void removePtr(AbstractUmlPtr& ptr) {
                std::lock_guard<std::mutex> ptrsLck(static_cast<ThreadSafeManagerNode*>(getNode(ptr))->m_ptrsMtx);
                AbstractAccessPolicy::removePtr(ptr);
            }

            virtual void assignPtr(AbstractUmlPtr& ptr) {
                std::lock_guard<std::mutex> ptrsLck(static_cast<ThreadSafeManagerNode*>(getNode(ptr))->m_ptrsMtx);
                AbstractAccessPolicy::assignPtr(ptr);
            }

            virtual void restorePtr(AbstractUmlPtr& ptr) {
                std::lock_guard<std::mutex> ptrsLck(static_cast<ThreadSafeManagerNode*>(getNode(ptr))->m_ptrsMtx);
                AbstractAccessPolicy::restorePtr(ptr);
            }

            // std::vector<std::unique_lock<std::mutex>> accessSet(AbstractSet& set, Element& el) {
            //     std::vector<std::unique_lock<std::mutex>> ret;
            //     std::unordered_map<ID, bool> referenceIdsMap;
            //     for (auto& pair : getNode(el)->m_references) {
            //         referenceIdsMap[pair.first] = false;
            //     }
            //     for (auto& pair : getNode(getEl(set))->m_references) {
            //         if (referenceIdsMap.count(pair.first)) {
            //             continue;
            //         }
            //         referenceIdsMap[pair.first] = false;
            //     }
            //     ret.reserve(getNode(el)->m_references.size());
            //     for (auto& referencePair : getNode(el)->m_references) {
            //         if (!referencePair.second.node) {
            //             continue;
            //         }
            //         if (referenceIdsMap.at(referencePair.first)) {
            //             continue;
            //         }
            //         ret.push_back(std::unique_lock<std::mutex>(static_cast<ThreadSafeManagerNode*>(referencePair.second.node)->m_mtx));
            //         referenceIdsMap[referencePair.first] = true;
            //     }
            //     for (auto& referencePair : getNode(getEl(set))->m_references) {
            //         if (!referencePair.second.node) {
            //             continue;
            //         }
            //         if (referenceIdsMap.at(referencePair.first)) {
            //             continue;
            //         }
            //         ret.push_back(std::unique_lock<std::mutex>(static_cast<ThreadSafeManagerNode*>(referencePair.second.node)->m_mtx));
            //         referenceIdsMap[referencePair.first] = true;
            //     }
            //     return ret;
            // }

            // std::vector<std::unique_lock<std::mutex>> accessSet(AbstractSet& set, ID id) {
                
            // }
            ElementPtr createPtr(AbstractManager* manager, ID id) {
                std::unordered_map<ID, ThreadSafeManagerNode>::const_iterator nodeIt = m_graph.find(id);
                const ThreadSafeManagerNode* node = 0;
                if (nodeIt == m_graph.end()) {
                    node = &m_graph[id];
                } else {
                    node = &nodeIt->second;
                }
                ElementPtr ret;
                setPtr(ret, id, manager, node);
                assignPtr(ret);
                return ret;
            }
        public:
            SetLock lockEl(Element& el) {
                return ThreadSafeSetLock(*static_cast<ThreadSafeManagerNode*>(getNode(el)));
            }
    };

}

#endif