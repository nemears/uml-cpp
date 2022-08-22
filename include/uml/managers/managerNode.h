#ifndef _UML_MANAGERS_MANAGER_NODE_H_
#define _UML_MANAGERS_MANAGER_NODE_H_

#include <unordered_map>
#include <list>
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
        Element* m_managerElementMemory = 0;
        // std::string m_path;
        bool m_mountedFlag = false;
        struct NodeReference {
            ManagerNode* node = 0;
            size_t count = 0;
        };
        std::unordered_map<ID, NodeReference> m_references;
        std::list<AbstractUmlPtr*> m_ptrs; // list to UmlPtr*'s
        void setReference(Element& el);
        void setReference(ID id);
        void removeReference(Element& el);
        void restoreReference(Element* el);
        void restoreReferences();
        void releaseReference(ID id);
        void releasePtrs();
        void referenceErased(ID id);
        void erase();
    };
}

#endif