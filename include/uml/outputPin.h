#ifndef OUTPUTPIN_H
#define OUTPUTPIN_H

#include "pin.h"
#include "action.h"

namespace UML {
    class OutputPin : public Pin {
        protected:
            void reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) override;
            void reindexName(string oldName, string newName) override;
        public:
            ElementType getElementType() override;
    };
}

#endif