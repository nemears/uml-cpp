#pragma once

#include "typedElement.h"
#include "parameterableElement.h"
#include "uml/managers/typeInfo.h"
#include "uml/set/set.h"
#include "uml/types/element.h"

namespace UML {

    class ConnectorEnd;

    class ConnectableElement : virtual public TypedElement, virtual public ParameterableElement {

        friend struct ElementInfo<ConnectableElement>;

        protected:
            ReadOnlySet<ConnectorEnd, ConnectableElement> m_ends = ReadOnlySet<ConnectorEnd, ConnectableElement>(this);
            ConnectableElement(std::size_t elementType, AbstractManager& manager);
        public:
            ReadOnlySet<ConnectorEnd, ConnectableElement>& getEnds();
            typedef TypeInfo<std::tuple<TypedElement, ParameterableElement>, ConnectableElement> Info;
    };

    template <>
    struct ElementInfo<ConnectableElement> : public DefaultInfo {
        static std::string name(__attribute__((unused)) AbstractElement& el) { return "ConnectableElement"; }
        static SetList sets(ConnectableElement& el) {
            return SetList {
                makeSetPair("ends", el.m_ends)
            };
        }
    };
}
