#ifndef _UML_MANAGERS_ABSTRACT_ACCESS_POLICY_H_
#define _UML_MANAGERS_ABSTRACT_ACCESS_POLICY_H_

#include "uml/element.h"
#include "uml/managers/abstractManager.h"
#include "uml/umlPtr.h"
#include "uml/set.h"

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

            virtual void removeNode(ID id) = 0;

            virtual void removePtr(AbstractUmlPtr& ptr) {
                ptr.m_node->m_ptrs.remove_if([&](const AbstractUmlPtr* ptrEntry) {
                    return ptrEntry->m_ptrId == ptr.m_ptrId;
                });
            }

            virtual void destroyPtr(AbstractUmlPtr& ptr) {
                if (ptr.m_node->m_ptrs.empty() && !ptr.m_node->m_managerElementMemory) {
                    removeNode(ptr.m_id);
                }
            }

            virtual void assignPtr(AbstractUmlPtr& ptr) {
                if (ptr.m_node->m_ptrs.size() > 0) {
                    ptr.m_ptrId = ptr.m_node->m_ptrs.back()->m_ptrId + 1;
                }
               ptr.m_node->m_ptrs.push_back(const_cast<AbstractUmlPtr*>(&ptr));
            }

            virtual void restorePtr(AbstractUmlPtr& ptr) {
                ptr.m_node->m_ptrs.push_back(const_cast<AbstractUmlPtr*>(&ptr));
            }

            ManagerNode* getNode(AbstractUmlPtr& ptr) {
                return ptr.m_node;
            }

            Element& getEl(AbstractSet& set) {
                return set.getEl();
            }
    };
}

#endif