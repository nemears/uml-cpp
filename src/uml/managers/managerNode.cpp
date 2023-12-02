#include "uml/managers/managerNode.h"
#include "uml/types/element.h"
#include "uml/managers/abstractManager.h"
#include "uml/umlPtr.h"

namespace UML {

ManagerNode& ManagerNode::operator=(const ManagerNode& rhs) {
    m_element = rhs.m_element;
    m_mountedFlag = rhs.m_mountedFlag;
    for (auto& ptr : rhs.m_ptrs) {
        m_ptrs.insert(ptr);
    }
    for (auto& pair : rhs.m_references) {
        m_references[pair.first] = {pair.second.node};
        NodeReference& reference = m_references[pair.first];
        for (auto ptr : pair.second.ptrs) {
            reference.ptrs.insert(ptr);
        }
    }
    return *this;
}

void ManagerNode::setReference(AbstractUmlPtr* ptr) {
    auto reference = m_references.find(ptr->m_id);
    if (reference == m_references.end()) {
        m_references[ptr->m_id] = {ptr->m_node};
        m_references.find(ptr->m_id)->second.ptrs.insert(ptr);
    } else {
        if (!reference->second.ptrs.count(ptr)) {
            reference->second.ptrs.insert(ptr);
        }
    }
}

void ManagerNode::setReference(ID id, ManagerNode* node) {
    auto reference = m_references.find(id);
    if (reference == m_references.end()) {
        m_references[id] = {node};
    }
}

void ManagerNode::removeReference(AbstractUmlPtr* ptr) {
    std::unordered_map<ID, NodeReference>::iterator ref = m_references.find(ptr->m_id);
    if (ref == m_references.end()) {
        throw ManagerStateException("tried to remove inexistent reference");
    }
    ref->second.ptrs.erase(ptr);
}

void ManagerNode::releasePtrs() {
    for (auto& ptr : m_ptrs) {
        ptr->releasePtr();
    }
}

void ManagerNode::restoreReferences() {
    for (auto& ptr : m_ptrs) {
        ptr->reindex(m_element->getID(), m_element);
    }
    m_element->restoreReferences();
}

void ManagerNode::referenceErased(ID id) {
    m_element->referenceErased(id);
}

void ManagerNode::erase() {
    for (auto& ptr : m_ptrs) {
        ptr->erasePtr();
    }
}

void ManagerNode::erasePtrs() {
    for (auto& ptr : m_ptrs) {
        ptr->erasePtr();
    }
}

void ManagerNode::reindexPtrs(ID newID) {
    for (auto& ptr : m_ptrs) {
        ptr->reindex(newID, m_element);
    }
}

void ManagerNode::assingPtr(AbstractUmlPtr* ptr) {
    ptr->m_node = this;
}

}