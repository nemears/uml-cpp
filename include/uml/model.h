#ifndef MODELH
#define MODELH

#include "package.h"

namespace UML{
    class Model : public Package {
        public:
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::MODEL;
            };
    };
}
#endif