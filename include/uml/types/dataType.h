#pragma once

#include "egm/egm-basic.h"
#include "uml/util/indexableSet.h"

namespace UML {
    template <class>
    class Classifier;

    template <class>
    class Property;

    template <class>
    class Operation;

    template <class ManagerPolicy>
    class DataType : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<DataType, EGM::TemplateTypeList<Classifier>>;
        protected:
            using OwnedAttributes = IndexableSet<Property, DataType>;
            using OwnedOperations = IndexableSet<Operation, DataType>;
            OwnedAttributes m_ownedAttributes = OwnedAttributes(this);
            OwnedOperations m_ownedOperations = OwnedOperations(this);
        private:
            void init() {
                m_ownedAttributes.subsets(ManagerPolicy::m_attributes);
                m_ownedAttributes.subsets(ManagerPolicy::m_ownedMembers);
                m_ownedAttributes.opposite(&OwnedAttributes::ManagedType::getDataTypeSingleton);
                m_ownedOperations.subsets(ManagerPolicy::m_features);
                m_ownedOperations.subsets(ManagerPolicy::m_ownedMembers);
                m_ownedOperations.opposite(&OwnedOperations::ManagedType::getDataTypeSingleton);
            }
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(DataType);
            OwnedAttributes& getOwnedAttributes() { return m_ownedAttributes; }
            OwnedOperations& getOwnedOperations() { return m_ownedOperations; }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::DataType> {
        static std::string name() { return "DataType"; }
        template <class Policy>
        static SetList sets(UML::DataType<Policy>& el) {
            return SetList {
                make_set_pair("ownedAttributes", el.getOwnedAttributes()),
                make_set_pair("ownedOperations", el.getOwnedOperations())
            };
        }
    };
}
