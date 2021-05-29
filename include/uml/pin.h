#ifndef PIN_H
#define PIN_H

#include "objectNode.h"
#include "multiplicityElement.h"

namespace UML {
    class Pin : public ObjectNode , public MultiplicityElement {
        public:
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
            static ElementType elementType() {
                return ElementType::PIN;
            };
    };
}

#endif