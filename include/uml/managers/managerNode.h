#pragma once

#include <memory>
#include <unordered_set>
#include <unordered_map>
#include "uml/managers/abstractManager.h"
#include "uml/managers/baseElement.h"

namespace UML {
    struct NodeReference;
    struct SetReference;
}

namespace std {
    template<>
    struct hash<UML::SetReference> {
        std::size_t operator()(const UML::SetReference& ref) const;
    };

}

namespace UML {

    class AbstractPtr;
    class SetStructure;

    struct ManagerNode {
        const ID m_id;
        std::shared_ptr<AbstractElement> m_ptr = 0;
        AbstractManager& m_manager;
        std::weak_ptr<ManagerNode> m_myPtr;
        std::unordered_set<AbstractPtr*> m_ptrs; // don't know how to not use raw ptr here
        std::unordered_map<ID, NodeReference> m_references;
        ManagerNode(AbstractElement* ptr) : m_id(ptr->m_id), m_ptr(ptr), m_manager(ptr->m_manager) {}
        ManagerNode(std::shared_ptr<AbstractElement> ptr) : m_id(ptr->m_id), m_ptr(ptr), m_manager(ptr->m_manager) {}
        ManagerNode(ID id, AbstractManager& manager) : m_id(id), m_manager(manager) {}
        ManagerNode(const ManagerNode& rhs) = delete;
        ~ManagerNode();
        void addPtr(AbstractPtr* ptr) {
            m_ptrs.insert(ptr);
        }
        void removePtr(AbstractPtr* ptr); 
        void setReference(std::shared_ptr<SetStructure> set, AbstractPtr& ptr);
        NodeReference& setReference(std::shared_ptr<ManagerNode> managerNode);
        void removeReference(std::shared_ptr<SetStructure> set, AbstractPtr& ptr);
    };

    struct SetReference {
        std::weak_ptr<SetStructure> m_set;
        SetReference(std::shared_ptr<SetStructure> set) : m_set(set) {}
        bool operator==(const SetReference& rhs) const {
            return m_set.lock() == rhs.m_set.lock();
        }
    };

    struct NodeReference {
        std::unordered_set<SetReference> m_sets;
        std::weak_ptr<ManagerNode> m_node;
        NodeReference(std::shared_ptr<ManagerNode> node) : m_node(node) {}
    };
}


