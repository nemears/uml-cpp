#ifndef EXTENSION_H
#define EXTENSION_H

#include "association.h"

namespace UML {
    class Extension : public Association {
        private:
        public:
            Extension();
            Extension(const Extension& extension);
            virtual ~Extension();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::EXTENSION;
            };
    };
}

#endif