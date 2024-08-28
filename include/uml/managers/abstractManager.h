#pragma once

#include "uml/id.h"

namespace UML {

    template <class T>
    class UmlPtr;
    class AbstractElement;
    typedef UmlPtr<AbstractElement> AbstractElementPtr;

    class  AbstractManager {
        public:
            virtual AbstractElementPtr create(std::size_t elementType) = 0;
            virtual AbstractElementPtr get(ID id) = 0;
            virtual void reindex(ID oldID, ID newID) = 0;

            // TODO rest of funcionality interface
    };
}
