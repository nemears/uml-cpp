#pragma once

#include "egm/egm-basic.h"

namespace UML {
    template <class>
    class NamedElement;

    template <class>
    class PackageableElement;

    template <class>
    class DirectedRelationship;

    template <class ManagerPolicy>
    class Dependency : public ManagerPolicy {
        friend struct EGM::ElementInfo<Dependency>;
        public:
            using Info = EGM::TypeInfo<Dependency, EGM::TemplateTypeList<PackageableElement, DirectedRelationship>>;
        protected:
            using NamedElementSet = EGM::Set<NamedElement, Dependency>;
            NamedElementSet m_suppliers = NamedElementSet(this);
            NamedElementSet m_clients = NamedElementSet(this);
        private:
            void init() {
                m_suppliers.subsets(ManagerPolicy::m_targets);
                m_clients.subsets(ManagerPolicy::m_sources);
                m_clients.opposite(&NamedElementSet::ManagedType::getClientDependencies);
            }
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(Dependency);
            NamedElementSet& getSuppliers() { return m_suppliers; }
            NamedElementSet& getClients() { return m_clients; }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::Dependency> {
        static std::string name() { return "Dependency"; }
        template <class Policy>
        static SetList sets(UML::Dependency<Policy>& el) {
            return SetList {
                make_set_pair("suppliers", el.m_suppliers),
                make_set_pair("clients", el.m_clients)
            };
        }
    };
}
