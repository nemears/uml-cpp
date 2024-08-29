#pragma once

#include "uml/id.h"

namespace UML {

    template <class T>
    class UmlPtr;
    class AbstractElement;
    typedef UmlPtr<AbstractElement> AbstractElementPtr;
    struct ManagerNode;

    class  AbstractManager {

        friend struct ManagerNode;
        
        public:
            virtual AbstractElementPtr create(std::size_t elementType) = 0;
            virtual AbstractElementPtr get(ID id) = 0;
            virtual void reindex(ID oldID, ID newID) = 0;
            virtual void release (AbstractElement& el) = 0;

        protected:
            virtual void destroyNode(ManagerNode& node) = 0;
            
            // TODO rest of funcionality interface
    };
}
