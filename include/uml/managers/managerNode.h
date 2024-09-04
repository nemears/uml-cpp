#pragma once

#include <memory>
#include <unordered_set>
#include "uml/managers/abstractManager.h"
#include "uml/managers/baseElement.h"

namespace UML {
    struct NodeReference;
}

namespace std {
    template<>
    struct hash<UML::NodeReference> {
        std::size_t operator()(const UML::NodeReference& ref) const;
    };

}

namespace UML {

    class AbstractPtr;

    struct ManagerNode {
        const ID m_id;
        std::shared_ptr<AbstractElement> m_ptr = 0;
        AbstractManager& m_manager;
        std::unordered_set<AbstractPtr*> m_ptrs; // don't know how to not use raw ptr here
        std::unordered_set<NodeReference> m_references;
        ManagerNode(AbstractElement* ptr) : m_id(ptr->m_id), m_ptr(ptr), m_manager(ptr->m_manager) {}
        ManagerNode(std::shared_ptr<AbstractElement> ptr) : m_id(ptr->m_id), m_ptr(ptr), m_manager(ptr->m_manager) {}
        ManagerNode(ID id, AbstractManager& manager) : m_id(id), m_manager(manager) {}
        ManagerNode(const ManagerNode& rhs) = delete;
        ~ManagerNode();
        void addPtr(AbstractPtr* ptr) {
            m_ptrs.insert(ptr);
        }
        void removePtr(AbstractPtr* ptr); 
        void setReference(AbstractPtr* ptr);
        void setReference(std::shared_ptr<ManagerNode> managerNode);
        void removeReference(AbstractPtr* ptr);
    };

    struct NodeReference {
        std::weak_ptr<ManagerNode> m_node;
        std::size_t m_count = 0; // Necessary?
        NodeReference(std::shared_ptr<ManagerNode> node) : m_node(node) {}
        bool operator==(const NodeReference& rhs) const { return m_node.lock()->m_id == rhs.m_node.lock()->m_id; }
    };
}


