#pragma once

#include "egm/egm-basic.h"
#include "uml/util/indexableSet.h"

namespace UML {
    template <class>
    class EncapsulatedClassifier;

    template <class>
    class Property;

    template <class>
    class Operation;

    template <class>
    class Reception;

    template <class ManagerPolicy>
    class Class : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<Class, EGM::TemplateTypeList<EncapsulatedClassifier>>;
        protected:
            using OwnedAttributesSet = IndexableSet<Property, Class>;
            using OwnedOperations = IndexableOrderedSet<Operation, Class>;
            using OwnedReceptions = IndexableSet<Reception, Class>;
            OwnedAttributesSet m_classOwnedAttributes = OwnedAttributesSet(this);
            OwnedOperations m_ownedOperations = OwnedOperations(this);
            OwnedReceptions m_ownedReceptions = OwnedReceptions(this);
        private:
            void init() {
                m_classOwnedAttributes.redefines(ManagerPolicy::m_ownedAttributes);
                m_classOwnedAttributes.opposite(&OwnedAttributesSet::ManagedType::getClassSingleton);
                m_ownedOperations.subsets(ManagerPolicy::m_features);
                m_ownedOperations.subsets(ManagerPolicy::m_ownedMembers);
                m_ownedReceptions.subsets(ManagerPolicy::m_features);
                m_ownedReceptions.subsets(ManagerPolicy::m_ownedMembers);
            }
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(Class);
            OwnedAttributesSet& getOwnedAttributes() { return m_classOwnedAttributes; }
            OwnedOperations& getOwnedOperations() { return m_ownedOperations; }
            OwnedReceptions& getOwnedReceptions() { return m_ownedReceptions; }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::Class> {
        static std::string name() { return "Class"; }
        template <class Policy>
        static SetList sets(UML::Class<Policy>& el) {
            return SetList {
                make_set_pair("ownedAttributes", el.getOwnedAttributes()),
                make_set_pair("ownedOperations", el.getOwnedOperations()),
                make_set_pair("ownedReceptions", el.getOwnedReceptions())
            };
        }
    };
}
