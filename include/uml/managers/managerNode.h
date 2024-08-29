#pragma once

#include <memory>
#include <unordered_set>
#include <unordered_map>
#include "uml/managers/abstractManager.h"
#include "uml/managers/baseElement.h"

namespace UML {

    class AbstractElement;
    struct NodeReference;
    template <class T>
    class UmlPtr;
    typedef UmlPtr<AbstractElement> AbstractElementPtr;

    struct ManagerNode {
        std::shared_ptr<AbstractElement> m_ptr = 0;
        AbstractManager& m_manager;
        std::unordered_set<AbstractElementPtr*> m_ptrs; // don't know how to not use raw ptr here
        std::unordered_map<ID, std::shared_ptr<NodeReference>> m_references;
        ManagerNode(AbstractElement* ptr) : m_ptr(ptr), m_manager(ptr->m_manager) {}
        ManagerNode(const ManagerNode& rhs) = delete;
        void addPtr(AbstractElementPtr& ptr) {
            m_ptrs.insert(&ptr);
        }
        void removePtr(AbstractElementPtr& ptr) {
            m_ptrs.erase(&ptr);
            if (m_ptrs.empty()) {
                m_manager.destroyNode(*this);
            }
        }
    };

    struct NodeReference {
        ManagerNode& m_node;
        NodeReference(ManagerNode& node) : m_node(node) {}
    };
}
