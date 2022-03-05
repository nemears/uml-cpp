#ifndef _UML_EXTENSION_END_H_
#define _UML_EXTENSION_END_H_

#include "property.h"
#include "stereotype.h"

namespace UML {

    class Extension;

    class ExtensionEnd : public Property {

        friend class UmlManager;

        private:
            Singleton<Stereotype, ExtensionEnd> m_extensionType = Singleton<Stereotype, ExtensionEnd>(this);
            Set<Stereotype, ExtensionEnd>& getTypeSingleton();
            void init();
            ExtensionEnd();
        public:
            virtual ~ExtensionEnd();
            TypePtr getType() const override;
            void setType(Stereotype& stereotype);
            void setType(Stereotype* stereotype);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::EXTENSION_END;
            };
    };
}

#endif