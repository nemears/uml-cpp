#ifndef INPUTPIN_H
#define INPUTPIN_H

#include "pin.h"
#include "action.h"

namespace UML {
    class InputPin : public Pin {

        friend class UmlManager;

        protected:
            InputPin();
        public:
            InputPin(const InputPin& rhs);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::INPUT_PIN;
            };
    };
}

#endif