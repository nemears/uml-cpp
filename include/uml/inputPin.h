#ifndef _UML_INPUT_PIN_H_
#define _UML_INPUT_PIN_H_

#include "pin.h"
#include "action.h"

namespace UML {
    class InputPin : public Pin {

        friend class UmlManager;

        protected:
            InputPin();
        public:
            virtual ~InputPin();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::INPUT_PIN;
            };
    };
}

#endif