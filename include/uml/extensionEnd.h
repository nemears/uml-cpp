#ifndef EXTENSION_END_H
#define EXTENSION_END_H

#include "property.h"

namespace UML {

    class Extension;

    class ExtensionEnd : public Property {
        private:
            ID m_extensionID;
            Extension* m_extensionPtr;
        public:
            ExtensionEnd();
            ExtensionEnd(const ExtensionEnd& end);
            virtual ~ExtensionEnd();
            Extension* getExtension();
            void setExtension(Extension* extension);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::EXTENSION_END;
            };
    };
}

#endif