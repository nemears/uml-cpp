#ifndef STEREOTYPE_H
#define STEREOTYPE_H

#include "class.h"

namespace UML {
    class Stereotype : public Class {
        private:
        public:
            Stereotype();
            Stereotype(const Stereotype& stereotype);
            virtual ~Stereotype();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::STEREOTYPE;
            };
    };
}

#endif