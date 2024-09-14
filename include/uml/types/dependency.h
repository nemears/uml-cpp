#pragma once
#include "packageableElement.h"
#include "directedRelationship.h"
#include "uml/types/element.h"

namespace UML {
    class Dependency : public DirectedRelationship, public PackageableElement {
        protected:
            Set<NamedElement, Dependency> m_clients = Set<NamedElement, Dependency>(this);
            Set<NamedElement, Dependency> m_suppliers = Set<NamedElement, Dependency>(this);
        public:
            Dependency(std::size_t elementType, AbstractManager& manager);
            Set<NamedElement, Dependency>& getClients();
            Set<NamedElement, Dependency>& getSuppliers();
            typedef TypeInfo<std::tuple<DirectedRelationship, PackageableElement>, Dependency> Info;
    };

    template <>
    struct ElementInfo<Dependency> : public DefaultInfo {
        static const bool abstract = false;
        inline static const std::string name {"Dependency"};
        static SetList sets(Dependency& el) {
            return SetList {
                makeSetPair("clients", el.getClients()),
                makeSetPair("suppliers", el.getSuppliers())
            };
        }
    };
}
