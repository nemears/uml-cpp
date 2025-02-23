#pragma once

#include "egm/egm-basic.h"
#include "uml/util/indexableSet.h"

namespace UML {
    template <class>
    class Classifier;

    template <class>
    class ConnectableElement;

    template <class>
    class Property;

    template <class ManagerPolicy>
    class StructuredClassifier : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<StructuredClassifier, EGM::TemplateTypeList<Classifier>>;
        protected:
            using Roles = ReadOnlyIndexableSet<ConnectableElement, StructuredClassifier>;
            using Parts = ReadOnlyIndexableSet<Property, StructuredClassifier>;
            struct OwnedAttributesPolicy {
                using PropertyImpl = typename Parts::ManagedType;
                void elementAdded(PropertyImpl& el, StructuredClassifier& me) {
                    if (el.isComposite() && !me.getParts().contains(el)) {
                        me.getParts().innerAdd(&el);
                    }
                }
                void elementRemoved(PropertyImpl& el, StructuredClassifier& me) {
                    if (el.isComposite() && me.getParts().contains(el)) {
                        me.getParts().innerRemove(&el);
                    }
                }
            };
            using OwnedAttributes = IndexableSet<Property, StructuredClassifier, OwnedAttributesPolicy>;
            Roles m_roles = Roles(this);
            OwnedAttributes m_ownedAttributes = OwnedAttributes(this);
            Parts m_parts = Parts(this);
        private:
            void init() {
                m_roles.subsets(ManagerPolicy::m_members);
                m_ownedAttributes.subsets(ManagerPolicy::m_attributes);
                m_ownedAttributes.subsets(m_roles);
                m_ownedAttributes.subsets(ManagerPolicy::m_ownedMembers);
                m_parts.subsets(m_ownedAttributes);
            }
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(StructuredClassifier);

            Roles& getRoles() { return m_roles; }
            OwnedAttributes& getOwnedAttributes() { return m_ownedAttributes; }
            Parts& getParts() { return m_parts; }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::StructuredClassifier> {
        static const bool abstract = true;
        static std::string name() { return "StructuredClassifier"; }
        template <class Policy>
        static SetList sets(UML::StructuredClassifier<Policy>& el) {
            return SetList {
                make_set_pair("roles", el.getRoles()),
                make_set_pair("ownedAttributes", el.getOwnedAttributes()),
                make_set_pair("parts", el.getParts())
            };
        }
    };
}
