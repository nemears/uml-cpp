#ifndef _UML_MANAGERS_ABSTRACT_ACCESS_POLICY_H_
#define _UML_MANAGERS_ABSTRACT_ACCESS_POLICY_H_

#include "uml/element.h"
#include "uml/managers/abstractManager.h"
#include "uml/umlPtr.h"

namespace UML {
    class AbstractAccessPolicy {
        protected:
            void setNode(ManagerNode* node) {
                node->m_managerElementMemory->m_node = node;
            }

            virtual ElementPtr get(ID id) = 0;

            ManagerNode* getNode(Element& el) {
                return el.m_node;
            }
            
            void reindexNoReplace(ID oldID, ID newID, ManagerNode* newDisc) {
                for (auto& ref : newDisc->m_references) {
                    if (!ref.second.node) {
                        // reference is relased currently with no ptrs
                        throw ManagerStateException("Bad state in reindex, reference released! TODO maybe aquire released el");
                    } else if (ref.second.node->m_references.count(oldID)) {
                        size_t numRefs = ref.second.node->m_references[oldID].count;
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