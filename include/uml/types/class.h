#pragma once

#include "egm/egm-basic.h"
#include "uml/util/indexableSet.h"

namespace UML {
    template <class>
    class Classifier;

    template <class>
    class Property;

    template <class ManagerPolicy>
    class Class : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<Class, EGM::TemplateTypeList<Classifier>>;
        protected:
            using OwnedAttributesSet = IndexableSet<Property, Class>;
            OwnedAttributesSet m_classOwnedAttributes = OwnedAttributesSet(this);
        private:
            void init() {
                m_classOwnedAttributes.subsets(ManagerPolicy::m_attributes);
                m_classOwnedAttributes.subsets(ManagerPolicy::m_ownedMembers);
                m_classOwnedAttributes.opposite(&OwnedAttributesSet::ManagedType::getClassSingleton);
            }
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(Class);
            OwnedAttributesSet& getOwnedAttributes() { return m_classOwnedAttributes; }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::Class> {
        static std::string name() { return "Class"; }
        template <class Policy>
        static SetList sets(UML::Class<Policy>& el) {
            return SetList {
                make_set_pair("ownedAttributes", el.getOwnedAttributes())
            };
        }
    };
}
