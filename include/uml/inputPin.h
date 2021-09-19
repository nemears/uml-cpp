#ifndef INPUTPIN_H
#define INPUTPIN_H

#include "pin.h"
#include "action.h"

namespace UML {
    class InputPin : public Pin {
        protected:
            // void reindexName(std::string oldName, std::string newName) override;
        public:
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::INPUT_PIN;
            };
    };
}

#endif