#ifndef _UML_MANAGERS_ABSTRACT_ACCESS_POLICY_H_
#define _UML_MANAGERS_ABSTRACT_ACCESS_POLICY_H_

#include "uml/element.h"

namespace UML {
    class AbstractAccessPolicy {
        protected:
            void setNode(ManagerNode* node) {
                node->m_managerElementMemory->m_node = node;
            }

            virtual ElementPtr get(ID id) = 0;

            void restore(Element& el) {
                el.m_node->m_managerElementMemory = &el;
                for (auto& pair : el.m_node->m_references) {
                    ManagerNode* node = pair.second.node;
                    if (!node || !node->m_managerElementMemory) {
                        // element has been released, possibly there are no pointers
                        continue;
                    }
                    node->m_managerElementMemory->restoreReference(&el);
                    el.restoreReference(node->m_managerElementMemory);
                }
                for (auto& ptr : el.m_node->m_ptrs) {
                    ptr->reindex(el.getID(), &el);
                }
                el.restoreReferences();
            }

            ManagerNode* releaseNode(Element& el) {
                ManagerNode* node = el.m_node;
                ID id = el.getID();
                for (auto& e : node->m_references) {
                    if (!e.second.node) {
                        // el has been released there are no pointers
                        continue;
                    }
                    if (e.second.node->m_managerElementMemory) {
                        e.second.node->m_managerElementMemory->referencingReleased(id);
                    }
                }
                for (auto& ptr : node->m_ptrs) {
                    ptr->releasePtr();
                }
                node->m_managerElementMemory = 0;
                delete &el;
                return node;
            }
            
            void reindexNoReplace(ID oldID, ID newID, ManagerNode* newDisc) {
                for (auto& ref : newDisc->m_references) {
                    if (!ref.second.node) {
                        // reference is relased currently with no ptrs
                        throw ManagerStateException("Bad state in reindex, reference released! TODO maybe aquire released el");
                    } else if (ref.second.node->m_references.count(oldID)) {
                        size_t numRefs = ref.second.node->m_references[oldID].numRefs;
                        ref.second.node->m_references.erase(oldID);
                        ref.second.node->m_references[newID] = ManagerNode::NodeReference{newDisc, numRefs};
                    }
                    if (!ref.second.node->m_managerElementMemory) {
                        get(ref.first);
                    }
                    ref.second.node->m_managerElementMemory->referenceReindexed(oldID, newID);
                }
                newDisc->m_managerElementMemory->m_node = newDisc;
                for (auto& ptr : newDisc->m_ptrs) {
                    ptr->reindex(newID, newDisc->m_managerElementMemory);
                }
            }

            void reindexReplace(ID oldID, ID newID, ManagerNode* newNode, ManagerNode& oldNode) {
                for (auto& pair : newNode->m_references) {
                    if (!pair.second.node || !pair.second.node->m_managerElementMemory) {
                        continue;
                    }

                    // Not sure why I am doing these two lines below here TODO investigate
                    pair.second.node->m_managerElementMemory->referencingReleased(newID);
                    pair.second.node->m_references[newID].node = 0;
                }
                for (auto& ptr : newNode->m_ptrs) {
                    ptr->reindex(newID, newNode->m_managerElementMemory);
                }
                for (auto& ptr : oldNode.m_ptrs) {
                    ptr->reindex(newID, newNode->m_managerElementMemory);
                    ptr->m_ptrId = 0;
                    if (newNode->m_ptrs.size() > 0) {
                        ptr->m_ptrId = newNode->m_ptrs.back()->m_ptrId + 1;
                    }
                    newNode->m_ptrs.push_back(ptr);
                }
            }
    };
}

#endif