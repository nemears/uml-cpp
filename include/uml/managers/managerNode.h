#pragma once

#include <memory>
#include <unordered_set>
#include <unordered_map>
#include "uml/id.h"

namespace UML {

    class Element;
    class AbstractUmlPtr;
    struct NodeReference;

    struct ManagerNode {
        std::shared_ptr<Element> m_ptr = 0;
        std::unordered_set<AbstractUmlPtr*> m_ptrs; // don't know how to not use raw ptr here
        std::unordered_map<ID, std::shared_ptr<NodeReference>> m_references;
        ManagerNode(Element* ptr) : m_ptr(ptr) {}
        ManagerNode(const ManagerNode& rhs) = delete;

    };

    struct NodeReference {
        ManagerNode& m_node;
        NodeReference(ManagerNode& node) : m_node(node) {}
    };
}
