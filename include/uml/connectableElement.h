#ifndef CONNECTABLE_ELEMENT_H
#define CONNECTABLE_ELEMENT_H

#include "typedElement.h"

namespace UML {
    class ConnectableElement : virtual public TypedElement {
        protected:

        public:
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
            ConnectableElement& operator=(ConnectableElement&&) {
                return *this;
            };
            static ElementType elementType() {
                return ElementType::CONNECTABLE_ELEMENT;
            };
    };
}

#endif