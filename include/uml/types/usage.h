#pragma once

#include "dependency.h"

namespace UML {
    /**
     * A Usage is a Dependency in which one NamedElement requires another NamedElement (or set of NamedElements) for 
     * its full implementation or operation.
     **/
    class Usage : public Dependency {

        friend struct ElementInfo<Usage>;

        protected:
            struct ClientPolicy {
                void elementAdded(NamedElement& el, Usage& me);
                void elementRemoved(NamedElement& el, Usage& me);
            };
            Set<NamedElement, Usage, ClientPolicy> m_usageClients = Set<NamedElement, Usage, ClientPolicy>(this);
        public:
            Usage(std::size_t elementType, AbstractManager& manager);
            typedef TypeInfo<std::tuple<Dependency>, Usage> Info;
    };

    template <>
    struct ElementInfo<Usage> : public DefaultInfo {
        static const bool abstract = false;
        inline static const std::string name {"Usage"};
        static SetList sets(Usage& el) {
            return SetList {
                makeSetPair("clients", el.m_usageClients)
            };
        }
    };
}
