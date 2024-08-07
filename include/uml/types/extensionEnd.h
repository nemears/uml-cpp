#pragma once

#include "property.h"

namespace UML {

    class Extension;
    class Stereotype;

    class ExtensionEnd : public Property {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        private:
            Singleton<Stereotype, ExtensionEnd> m_extensionType = Singleton<Stereotype, ExtensionEnd>(this);
            Singleton<Stereotype, ExtensionEnd>& getTypeSingleton();
            ExtensionEnd();
        public:
            virtual ~ExtensionEnd();
            TypePtr getType() const override;
            void setType(Stereotype& stereotype);
            void setType(Stereotype* stereotype);
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::EXTENSION_END;
            };
    };
}
