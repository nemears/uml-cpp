#ifndef REALIZATION_H
#define REALIZATION_H

#include "uml/abstraction.h"

namespace UML {
    /**
     * Realization is a specialized Abstraction dependency between two sets of NamedElements, one representing a 
     * specification (the supplier) and the other representing an implementation of that specification (the client).
     **/
    class Realization : public Abstraction {
        public:
            Realization();
            Realization(const Realization& realization);
            virtual ~Realization();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::REALIZATION;
            };
    };
}

#endif