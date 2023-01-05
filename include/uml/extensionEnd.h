#ifndef _UML_EXTENSION_END_H_
#define _UML_EXTENSION_END_H_

#include "property.h"
#include "stereotype.h"

namespace UML {

    class Extension;

    class ExtensionEnd : public Property {

        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;

        private:
            CustomSingleton<Stereotype, ExtensionEnd> m_extensionType = CustomSingleton<Stereotype, ExtensionEnd>(this);
            TypedSet<Stereotype, ExtensionEnd>& getTypeSingleton();
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