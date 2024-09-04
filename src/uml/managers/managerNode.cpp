#include "uml/managers/managerNode.h"
#include "uml/managers/abstractManager.h"
#include "uml/umlPtr.h"
#include <unordered_set>

namespace UML {

ManagerNode::~ManagerNode() {
    for (auto& reference : m_references) {
        auto referenceNode = reference.m_node.lock(); 
        if (!referenceNode) {
            continue;
        }
        auto& referenceReferences = referenceNode->m_references;
        auto referenceReferenceIt = referenceReferences.begin();
        auto referenceReferenceEnd = referenceReferences.end();
        while (referenceReferenceIt != referenceReferenceEnd) {
            auto referenceReferenceNode = referenceReferenceIt->m_node.lock();
            if (referenceReferenceNode && referenceReferenceNode->m_id == m_id) {
                break;
            }
            referenceReferenceIt++;
        }
        if (referenceReferenceIt != referenceReferenceEnd) {
            referenceReferences.erase(referenceReferenceIt);
        }
    }
    m_ptr = 0;
}
    
void ManagerNode::setReference(AbstractPtr* ptr) {
    std::unordered_set<NodeReference>::iterator referenceIt = m_references.begin();
    auto referenceEnd = m_references.end();
    while (referenceIt != referenceEnd) {
        if (referenceIt->m_node.lock()->m_id == m_id) {
            break;
        }
        referenceIt++;
    }
    if (referenceIt == referenceEnd) {
        auto emplaceResult = m_references.emplace(ptr->m_node.lock());
        const_cast<NodeReference&>(*emplaceResult.first).m_count++;
    } else {
        const_cast<NodeReference&>(*referenceIt).m_count++;
    }
}

void ManagerNode::setReference(std::shared_ptr<ManagerNode> node) {
    auto referenceIt = m_references.begin();
    auto referenceEnd = m_references.end();
    while (referenceIt != referenceEnd) {
        if (referenceIt->m_node.lock()->m_id == node->m_id) {
            break;
        }
        referenceIt++;
    }

    if (referenceIt == referenceEnd) {
        m_references.emplace(node);
    }
}

void ManagerNode::removeReference(AbstractPtr* ptr) {
    auto referenceIt = m_references.begin();
    auto referenceEnd = m_references.end();
    while (referenceIt != referenceEnd) {
        if (referenceIt->m_node.lock()->m_id == ptr->m_id) {
            break;
        }
        referenceIt++;
    }

    if (referenceIt == referenceEnd) {
        throw ManagerStateException("tried to remove inexistent reference");
    }

    const_cast<NodeReference&>(*referenceIt).m_count--;
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

std::size_t hash<UML::NodeReference>::operator()(const UML::NodeReference& ref) const {
    hash<string> hasher;
    return hasher(ref.m_node.lock()->m_id.string());
}
}
