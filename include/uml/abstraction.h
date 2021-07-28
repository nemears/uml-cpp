#ifndef ABSTRACTION_H
#define ABSTRACTION_H

#include "dependency.h"

namespace UML {
    /**
     * An Abstraction is a Dependency that relates two NamedElements or sets of NamedElements that represent the same 
     * concept at different levels of abstraction or from different viewpoints.
     **/
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