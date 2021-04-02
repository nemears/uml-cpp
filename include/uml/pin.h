#ifndef PIN_H
#define PIN_H

#include "objectNode.h"
#include "multiplicityElement.h"

namespace UML {
    class Pin : public ObjectNode , public MultiplicityElement {
        public:
            ElementType getElementType() override;
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif