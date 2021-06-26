#ifndef CONNECTABLE_ELEMENT_H
#define CONNECTABLE_ELEMENT_H

#include "typedElement.h"
#include "parameterableElement.h"

namespace UML {
    class ConnectableElement : virtual public TypedElement, virtual public ParameterableElement {
        protected:

        public:
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            ConnectableElement& operator=(ConnectableElement&&) {
                return *this;
            };
            static ElementType elementType() {
                return ElementType::CONNECTABLE_ELEMENT;
            };
    };
}

#endif