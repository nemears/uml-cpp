#ifndef EXTENSION_H
#define EXTENSION_H

#include "association.h"

namespace UML {

    class ExtensionEnd;

    class Extension : public Association {
        private:
            /**
             * NOTE: keeping it simple for now, uml suggests dealing with metaclasses as actual Classes stored within
             * every model, but obviously it will be more efficient to just keep track of it by enum right now.
             **/
            ElementType m_metaClass;
            ID m_ownedEndID;
            ExtensionEnd* m_ownedEndPtr;
        public:
            Extension();
            Extension(const Extension& extension);
            virtual ~Extension();
            void setMetaClass(ElementType metaClass);
            ElementType getMetaClass();
            ExtensionEnd* getOwnedEnd();
            void setOwnedEnd(ExtensionEnd* end);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::EXTENSION;
            };
    };
}

#endif