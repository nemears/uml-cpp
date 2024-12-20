#pragma once

#include "property.h"
#include "uml/managers/typeInfo.h"
#include "uml/types/element.h"

namespace UML {

    class Extension;
    class Stereotype;
    typedef UmlPtr<Stereotype> StereotypePtr;

    class ExtensionEnd : public Property {

        friend struct ElementInfo<ExtensionEnd>;

        private:
            Singleton<Stereotype, ExtensionEnd> m_extensionType = Singleton<Stereotype, ExtensionEnd>(this);
            Singleton<Stereotype, ExtensionEnd>& getTypeSingleton();
        public:
            ExtensionEnd(std::size_t elementType, AbstractManager& manager);
            StereotypePtr getType() const;
            void setType(Stereotype& stereotype);
            void setType(StereotypePtr stereotype);
            typedef TypeInfo<std::tuple<Property>, ExtensionEnd> Info;
    };

    template <>
    struct ElementInfo<ExtensionEnd> : public DefaultInfo {
        static const bool abstract = false;
        static std::string name(__attribute__((unused)) AbstractElement& el) { return "ExtensionEnd"; }
        static SetList sets(ExtensionEnd& el) {
            return SetList {
                makeSetPair("type", el.m_extensionType)
            };
        }
    };
}
