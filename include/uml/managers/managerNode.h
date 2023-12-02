#pragma once

#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include "uml/id.h"

namespace UML {

    class Element;
    class AbstractUmlPtr;

    /**
     * The ManagerNode struct is used as nodes in the internal graphs of the element's manager
     * It is also stored as a pointer from within the element so each element can quickly access
     * its place in the reference graph
    **/
    struct ManagerNode {

        // The element this node is representing
        Element* m_element = 0;
        bool m_mountedFlag = false;
        struct NodeReference {
            // The node of the referenced element
            // whether released or not it should have a node because we are referencing it
            ManagerNode* node = 0;

            // list of ptrs to the reference owned by this element
            std::unordered_set<AbstractUmlPtr*> ptrs;
        };
        std::unordered_map<ID, NodeReference> m_references;

        // list of all of the ptrs representing the element of this node
        std::unordered_set<AbstractUmlPtr*> m_ptrs;
        std::mutex m_mtx;
        std::mutex m_ptrsMtx;
        ManagerNode(){};
        ManagerNode& operator=(const ManagerNode& rhs);

        // methods
        void setReference(AbstractUmlPtr* ptr);
        void setReference(ID id, ManagerNode* node);
        void removeReference(AbstractUmlPtr* ptr);
        void restoreReferences();
        void releasePtrs();
        void referenceErased(ID id);
        void erase();
        void erasePtrs();
        void reindexPtrs(ID newID);
        void assingPtr(AbstractUmlPtr* ptr);
    };
}