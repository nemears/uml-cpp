#ifndef EXTENSION_END_H
#define EXTENSION_END_H

#include "property.h"
#include "stereotype.h"

namespace UML {

    class Extension;
    namespace Parsers {
        class SetExtension;
    }

    class ExtensionEnd : public Property {

        friend class UmlManager;
        friend class Parsers::SetExtension;

        private:
            Singleton<Stereotype, ExtensionEnd> m_extensionType = Singleton<Stereotype, ExtensionEnd>(this);
            Set<Stereotype, ExtensionEnd>& getTypeSingleton();
            void init();
            void copy(const ExtensionEnd& rhs);
            ExtensionEnd();
        public:
            ExtensionEnd(const ExtensionEnd& end);
            virtual ~ExtensionEnd();
            Stereotype* getType() override;
            Stereotype& getTypeRef();
            bool hasType() const;
            void setType(Stereotype& stereotype);
            void setType(Stereotype* stereotype);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::EXTENSION_END;
            };
    };
}

#endif