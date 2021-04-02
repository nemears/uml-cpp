#ifndef INPUTPIN_H
#define INPUTPIN_H

#include "pin.h"
#include "action.h"

namespace UML {
    class InputPin : public Pin {
        protected:
            void reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) override;
            void reindexName(string oldName, string newName) override;
        public:
            ElementType getElementType() override;
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif