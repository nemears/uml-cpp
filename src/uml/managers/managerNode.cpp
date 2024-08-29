#include "uml/managers/managerNode.h"
#include "uml/managers/abstractManager.h"
#include "uml/umlPtr.h"

namespace UML {

// ManagerNode& ManagerNode::operator=(const ManagerNode& rhs) {
//     m_element = rhs.m_element;
//     m_mountedFlag = rhs.m_mountedFlag;
//     for (auto& ptr : rhs.m_ptrs) {
//         m_ptrs.insert(ptr);
//     }
//     for (auto& pair : rhs.m_references) {
//         m_references[pair.first] = {pair.second.node};
//         NodeReference& reference = m_references[pair.first];
//         for (auto ptr : pair.second.ptrs) {
//             reference.ptrs.insert(ptr);
//         }
//     }
//     return *this;
// }

void ManagerNode::setReference(AbstractElementPtr* ptr) {
    auto reference = m_references.find(ptr->m_id);
    if (reference == m_references.end()) {
        reference = m_references.emplace(ptr->m_id, *ptr->m_node).first;
        reference->second.m_ptrs.insert(ptr);
    } else {
        if (!reference->second.m_ptrs.count(ptr)) {
            reference->second.m_ptrs.insert(ptr);
        }
    }
}

void ManagerNode::setReference(ID id, ManagerNode& node) {
    auto reference = m_references.find(id);
    if (reference == m_references.end()) {
        m_references.emplace(id, node);
    }
}

void ManagerNode::removeReference(AbstractElementPtr* ptr) {
    auto ref = m_references.find(ptr->m_id);
    if (ref == m_references.end()) {
        throw ManagerStateException("tried to remove inexistent reference");
    }
    ref->second.m_ptrs.erase(ptr);
}

// void ManagerNode::releasePtrs() {
//     for (auto& ptr : m_ptrs) {
//         ptr->releasePtr();
//     }
// }
// 
// void ManagerNode::restoreReferences() {
//     for (auto& ptr : m_ptrs) {
//         ptr->reindex(m_element->getID(), m_element);
//     }
//     m_element->restoreReferences();
// }
// 
// void ManagerNode::referenceErased(ID id) {
//     m_element->referenceErased(id);
// }
// 
// void ManagerNode::erase() {
//     for (auto& ptr : m_ptrs) {
//         ptr->erasePtr();
//     }
// }
// 
// void ManagerNode::erasePtrs() {
//     for (auto& ptr : m_ptrs) {
//         ptr->erasePtr();
//     }
// }
// 
// void ManagerNode::reindexPtrs(ID newID) {
//     for (auto& ptr : m_ptrs) {
//         ptr->reindex(newID, m_element);
//     }
// }
// 
// void ManagerNode::assingPtr(AbstractUmlPtr* ptr) {
//     ptr->m_node = this;
// }

}
