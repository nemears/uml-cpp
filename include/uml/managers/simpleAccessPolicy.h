#ifndef _UML_MANAGERS_SIMPLE_ACCESS_POLICY_H_
#define _UML_MANAGERS_SIMPLE_ACCESS_POLICY_H_

#include "abstractAccessPolicy.h"

namespace UML {
    class SimpleAccessPolicy : public AbstractAccessPolicy {
        protected:
            std::unordered_map<ID, ManagerNode> m_graph;

            void clear() {
                for (auto& pair : m_graph) {
                    pair.second.erasePtrs();
                    delete pair.second.m_managerElementMemory;
                    pair.second.erase();
                }
                m_graph.clear();
            }
            
            ManagerNode& assignNode(Element* newElement) {
                ManagerNode& node = m_graph[newElement->getID()];
                node.m_managerElementMemory = newElement;
                return node;
            }

            ElementPtr get(ID id) {
                std::unordered_map<ID, ManagerNode>::const_iterator result = m_graph.find(id);
                if (result != m_graph.end() && (*result).second.m_managerElementMemory) {
                    return ElementPtr(result->second.m_managerElementMemory);
                }
                return ElementPtr(0);
            }

            bool loaded(ID id) {
                std::unordered_map<ID, ManagerNode>::const_iterator result = m_graph.find(id);
                return result != m_graph.end() && (*result).second.m_managerElementMemory;
            }

            bool exists(ID id) {
                std::unordered_map<ID, ManagerNode>::const_iterator result = m_graph.find(id);
                return result != m_graph.end();
            }

            void restoreNode(ManagerNode* restoredNode) {
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
                std::vector<ManagerNode*> nodesToErase(node->m_references.size());
                {
                    std::vector<ID> idsToAquire(node->m_references.size());
                    size_t i = 0;
                    for (auto& pair : node->m_references) {
                        if (!pair.second.node || !pair.second.node->m_managerElementMemory) {
                            // element has been released, aquire
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
                    refNode->referenceErased(id);
                    // refNode->removeReference(*node->m_managerElementMemory);
                    if (refNode->m_references.count(id)) {
                        refNode->m_references.erase(id);
                    }
                }
                node->erase();
                delete node->m_managerElementMemory;
                m_graph.erase(id);
            }

            void releaseNode(ManagerNode* node) {
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
                delete node->m_managerElementMemory;
                node->m_managerElementMemory = 0;
                if (node->m_ptrs.empty()) {
                    m_graph.erase(id);
                }
            }

            // Element with this ID already exists, overwrite it with new one
            // This logic should only be called when it is loading from disk
            // and will overwrite the existing element in memory with one from disk
            // during a UmlManager::open() or UmlManager::aquire(id) invoke
            void overwrite(ID oldID, ID newID) {
                std::unordered_map<ID, ManagerNode>::iterator nodeToBeOverwrittenIterator = m_graph.find(newID);
                ManagerNode& nodeToBeOverwritten = nodeToBeOverwrittenIterator->second;
                ManagerNode& newNode = m_graph[oldID];
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
                
                // reindex
                ManagerNode& discRef = m_graph[oldID];
                ManagerNode& newDisc = m_graph[newID] = discRef;
                reindexNoReplace(oldID, newID, &newDisc);
                m_graph.erase(oldID);
            }

            void removeNode(ID id) override {
                m_graph.erase(id);
            }
    };
}

#endif