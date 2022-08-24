#ifndef _UML_MANAGERS_SIMPLE_ACCESS_POLICY_H_
#define _UML_MANAGERS_SIMPLE_ACCESS_POLICY_H_

#include "abstractAccessPolicy.h"

namespace UML {
    class SimpleAccessPolicy : public AbstractAccessPolicy {
        protected:
            std::unordered_map<ID, ManagerNode> m_graph;
            
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

            void restoreNode(ManagerNode* restoredNode) {
                // el.m_node->m_managerElementMemory = &el;
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
                            // try {
                            //     me->get(pair.first);
                            //     nodesToErase[i] = pair.second.node;
                            // } catch (ID_doesNotExistException& idException) {
                            //     nodesToErase[i] = 0;
                            // }
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
                node->m_managerElementMemory = 0;
                delete node->m_managerElementMemory;
                if (node->m_ptrs.empty()) {
                    m_graph.erase(id);
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