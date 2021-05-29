#ifndef OUTPUTPIN_H
#define OUTPUTPIN_H

#include "pin.h"
#include "action.h"

namespace UML {
    class OutputPin : public Pin {
        protected:
            void reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) override;
            void reindexName(std::string oldName, std::string newName) override;
        public:
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
            static ElementType elementType() {
                return ElementType::OUTPUT_PIN;
            };
    };
}

#endif