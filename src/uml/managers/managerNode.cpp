#include "uml/managers/managerNode.h"
#include "uml/managers/abstractManager.h"
#include "uml/managers/umlPtr.h"
#include "uml/set/abstractSet.h"
#include <unordered_set>

namespace UML {

ManagerNode::~ManagerNode() {
    for (auto& pair : m_references) {
        auto& referenceReferences = pair.second.m_node.lock()->m_references;
        auto referenceMatch = referenceReferences.find(m_id);
        if (referenceMatch != referenceReferences.end()) {
            referenceReferences.erase(referenceMatch);
        }
    }
}
    
void ManagerNode::setReference(std::shared_ptr<SetStructure> set, AbstractPtr& ptr) {
    auto referenceMatch = m_references.find(ptr.m_id);
    if (referenceMatch == m_references.end()) {
        auto& reference = setReference(ptr.m_node.lock());
        reference.m_sets.emplace(set);
    } else {
        referenceMatch->second.m_sets.emplace(set);
    }
}

NodeReference& ManagerNode::setReference(std::shared_ptr<ManagerNode> node) {
    auto referenceMatch = m_references.find(node->m_id);
    if (referenceMatch == m_references.end()) {
        auto& reference = m_references.emplace(node->m_id, node).first->second;
        node->setReference(m_myPtr.lock());
        return reference;
    }
    return referenceMatch->second;
}

void ManagerNode::removeReference(std::shared_ptr<SetStructure> set, AbstractPtr& ptr) {
    auto referenceMatch = m_references.find(ptr.m_id);
    referenceMatch->second.m_sets.erase(set);
    if (referenceMatch->second.m_sets.empty()) {
        // check to remove the other side
        auto& ptrReferences = ptr.m_node.lock()->m_references;
        auto ptrReferenceMatch = ptrReferences.find(m_id);
        if (ptrReferenceMatch != ptrReferences.end() && ptrReferenceMatch->second.m_sets.empty()) {
            ptrReferences.erase(ptrReferenceMatch);
            m_references.erase(referenceMatch);
        }
    }
}

void ManagerNode::removePtr(AbstractPtr* ptr) {
    auto id = ptr->m_id;
    m_ptrs.erase(ptr);
    if (m_ptrs.empty() && !m_ptr) {
        m_manager.destroy(id);
    }
}

}

namespace std {
    std::size_t hash<UML::SetReference>::operator()(const UML::SetReference& ref) const {
        return reinterpret_cast<std::size_t>(ref.m_set.lock().get());
    }
}
