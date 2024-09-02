#pragma once

#include <memory>
#include <unordered_set>
#include <unordered_map>
#include "uml/managers/abstractManager.h"
#include "uml/managers/baseElement.h"

namespace UML {

    struct NodeReference;
    class AbstractPtr;

    struct ManagerNode {
        std::shared_ptr<AbstractElement> m_ptr = 0;
        AbstractManager& m_manager;
        std::unordered_set<AbstractPtr*> m_ptrs; // don't know how to not use raw ptr here
        std::unordered_map<ID, NodeReference> m_references;
        ManagerNode(AbstractElement* ptr) : m_ptr(ptr), m_manager(ptr->m_manager) {}
        ManagerNode(AbstractManager& manager) : m_manager(manager) {}
        ManagerNode(const ManagerNode& rhs) = delete;
        void addPtr(AbstractPtr* ptr) {
            m_ptrs.insert(ptr);
        }
        void removePtr(AbstractPtr* ptr); 
        void setReference(AbstractPtr* ptr);
        void setReference(ID id, ManagerNode& managerNode);
        void removeReference(AbstractPtr* ptr);
    };

    struct NodeReference {
        ManagerNode& m_node;
        std::unordered_set<AbstractPtr*> m_ptrs;
        NodeReference(ManagerNode& node) : m_node(node) {}
    };
}
