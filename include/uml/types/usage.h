#pragma once

#include "dependency.h"

namespace UML {
    /**
     * A Usage is a Dependency in which one NamedElement requires another NamedElement (or set of NamedElements) for 
     * its full implementation or operation.
     **/
    class Usage : public Dependency {
        protected:
            struct ClientPolicy {
                void elementAdded(NamedElement& el, Usage& me);
                void elementRemoved(NamedElement& el, Usage& me);
            };
            Set<NamedElement, Usage, ClientPolicy> m_usageClients = Set<NamedElement, Usage, ClientPolicy>(this);
            Usage(std::size_t elementType, AbstractManager& manager);
        public:
            typedef TypeInfo<std::tuple<Dependency>, Usage> Info;
    };

    template <>
    struct ElementInfo<Usage> {
        static const bool abstract = false;
        inline static const std::string name {"Usage"};
        static SetList sets(__attribute__((unused)) Usage& el) {
            return SetList {};
        }
    };
}
