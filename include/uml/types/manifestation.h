#pragma once

#include "abstraction.h"
#include "uml/managers/typeInfo.h"
#include "uml/types/element.h"

namespace UML {

    typedef UmlPtr<PackageableElement> PackageableElementPtr;

    class Manifestation : public Abstraction {

        friend struct ElementInfo<Manifestation>;

        protected:
            Singleton<PackageableElement, Manifestation> m_utilizedElement = Singleton<PackageableElement, Manifestation>(this);
            Singleton<PackageableElement, Manifestation>& getUtilizedElementSingleton();
        public:
            Manifestation(std::size_t elementType, AbstractManager& manager);
            PackageableElementPtr getUtilizedElement() const;
            void setUtilizedElement(PackageableElement& utilizedElement);
            void setUtilizedElement(PackageableElementPtr utilizedElement);
            void setUtilizedElement(ID id);
            typedef TypeInfo<std::tuple<Abstraction>, Manifestation> Info;
    };

    template <>
    struct ElementInfo<Manifestation> : public DefaultInfo {
        static const bool abstract = false;
        static std::string name(__attribute__((unused)) AbstractElement& el) { return "Manifestation"; }
        static SetList sets(Manifestation& el) {
            return SetList {
                makeSetPair("utilizedElement", el.m_utilizedElement)
            };
        }
    };
}
