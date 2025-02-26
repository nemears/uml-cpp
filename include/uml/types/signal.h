#pragma once

#include "egm/egm-basic.h"
#include "uml/util/indexableSet.h"

namespace UML {
    template <class>
    class Classifier;

    template <class>
    class Property;

    template <class>
    class Reception;

    template <class ManagerPolicy>
    class Signal : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<Signal, EGM::TemplateTypeList<Classifier>>;
        protected:
            using OwnedAttributes = IndexableOrderedSet<Property, Signal>;
            using Receptions = IndexableSet<Reception, Signal>;
            OwnedAttributes m_ownedAttributes = OwnedAttributes(this);
            Receptions m_receptions = Receptions(this);
        private:
            void init() {
                m_ownedAttributes.subsets(ManagerPolicy::m_attributes);
                m_ownedAttributes.subsets(ManagerPolicy::m_ownedMembers);
                m_receptions.opposite(&Receptions::ManagedType::getSignalSingleton);
            }
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(Signal);

            OwnedAttributes& getOwnedAttributes() { return m_ownedAttributes; }
            Receptions& getReceptions() { return m_receptions; }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::Signal> {
        static std::string name() { return "Signal"; }
        template <class Policy>
        static SetList sets(UML::Signal<Policy>& el) {
            return SetList {
                make_set_pair("ownedAttributes", el.getOwnedAttributes()),
                make_set_pair("receptions", el.getReceptions())
            };
        }
    };
}
