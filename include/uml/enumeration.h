#ifndef ENUMERATION_H
#define ENUMERATION_H

#include "dataType.h"

namespace UML {
    class Enumeration : public DataType {
        protected:

        public:
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif