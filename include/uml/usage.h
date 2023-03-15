#ifndef _UML_USAGE_H_
#define _UML_USAGE_H_

#include "dependency.h"

namespace UML {
    /**
     * A Usage is a Dependency in which one NamedElement requires another NamedElement (or set of NamedElements) for 
     * its full implementation or operation.
     **/
    class Usage : public Dependency {

        template <typename AccessPolicy, typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            class AddClientPolicy {
                public:
                    void apply(NamedElement& el, Usage& me);
            };
            class RemoveClientPolicy {
                public:
                    void apply(NamedElement& el, Usage& me);
            };
            CustomSet<NamedElement, Usage, AddClientPolicy, RemoveClientPolicy> m_usageClients = CustomSet<NamedElement, Usage, AddClientPolicy, RemoveClientPolicy>(this);
            Usage();
        public:
            virtual ~Usage();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::USAGE;
            };
    };
}

#endif