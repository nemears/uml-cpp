#ifndef EXTENSION_END_H
#define EXTENSION_END_H

#include "property.h"
#include "stereotype.h"

namespace UML {

    class Extension;

    class ExtensionEnd : public Property {
        private:
            ID m_extensionID;
            Extension* m_extensionPtr;
            ID m_extensionTypeID;
            Stereotype* m_extensionTypePtr;
        public:
            ExtensionEnd();
            ExtensionEnd(const ExtensionEnd& end);
            virtual ~ExtensionEnd();
            Extension* getExtension();
            void setExtension(Extension* extension);
            Stereotype* getType() override;
            void setType(Stereotype* stereotype);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::EXTENSION_END;
            };
    };
}

#endif