#include "uml/managers/managerNode.h"
#include "uml/types/element.h"
#include "uml/managers/abstractManager.h"
#include "uml/umlPtr.h"

namespace UML {

ManagerNode& ManagerNode::operator=(const ManagerNode& rhs) {
    m_managerElementMemory = rhs.m_managerElementMemory;
    m_mountedFlag = rhs.m_mountedFlag;
    for (auto& ptr : rhs.m_ptrs) {
        m_ptrs.push_back(ptr);
    }
    for (auto& pair : rhs.m_references) {
        m_references[pair.first] = {pair.second.node, pair.second.count};
    }
    return *this;
}

void ManagerNode::setReference(Element& el) {
    auto reference = m_references.find(el.getID());
    if (reference == m_references.end()) {
        m_references[el.getID()] = {el.m_node, 1};
    } else {
        reference->second.count++;
    }
}

void ManagerNode::setReference(ID id) {
    auto reference = m_references.find(id);
    if (reference == m_references.end()) {
        m_references[id] = {0, 1};
    } else {
        reference->second.count++;
    }
}

void ManagerNode::removeReference(Element& el) {
    std::unordered_map<ID, NodeReference>::iterator ref = m_references.find(el.getID());
    if (ref == m_references.end()) {
        throw ManagerStateException("tried to remove inexistent reference");
    }
    if (ref->second.count == 1) {
        m_references.erase(ref);
    } else {
        ref->second.count--;
    }
}

void ManagerNode::restoreReference(Element* el) {
    if (!el) {
        throw ManagerStateException("reference is null when trying to restore!");
    }
    m_managerElementMemory->restoreReference(el);
}

void ManagerNode::releasePtrs() {
    for (auto& ptr : m_ptrs) {
        ptr->releasePtr();
    }
}

void ManagerNode::restoreReferences() {
    for (auto& ptr : m_ptrs) {
        ptr->reindex(m_managerElementMemory->getID(), m_managerElementMemory);
    }
    m_managerElementMemory->restoreReferences();
}

void ManagerNode::referenceErased(ID id) {
    m_managerElementMemory->referenceErased(id);
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
        ptr->reindex(newID, m_managerElementMemory);
    }
}

void ManagerNode::assingPtr(AbstractUmlPtr* ptr) {
    ptr->m_node = this;
}

}