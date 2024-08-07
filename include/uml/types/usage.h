#pragma once

#include "dependency.h"

namespace UML {
    /**
     * A Usage is a Dependency in which one NamedElement requires another NamedElement (or set of NamedElements) for 
     * its full implementation or operation.
     **/
    class Usage : public Dependency {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            struct ClientPolicy {
                void elementAdded(NamedElement& el, Usage& me);
                void elementRemoved(NamedElement& el, Usage& me);
            };
            Set<NamedElement, Usage, ClientPolicy> m_usageClients = Set<NamedElement, Usage, ClientPolicy>(this);
            Usage();
        public:
            virtual ~Usage();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::USAGE;
            };
    };
}
