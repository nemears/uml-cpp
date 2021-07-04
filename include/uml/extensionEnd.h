#ifndef EXTENSION_END_H
#define EXTENSION_END_H

#include "property.h"

namespace UML {
    class ExtensionEnd : public Property {
        private:
        public:
            ExtensionEnd();
            ExtensionEnd(const ExtensionEnd& end);
            virtual ~ExtensionEnd();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::EXTENSION_END;
            };
    };
}

#endif