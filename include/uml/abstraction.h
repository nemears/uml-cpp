#ifndef ABSTRACTION_H
#define ABSTRACTION_H

#include "dependency.h"

namespace UML {
    class Abstraction : public Dependency {
        // TODO
        public:
            Abstraction();
            Abstraction(const Abstraction& abstraction);
            virtual ~Abstraction();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ABSTRACTION;
            };
    };
}

#endif