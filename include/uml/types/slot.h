#pragma once

#include "egm/egm-basic.h"
#include "uml/util/indexableSet.h"

namespace UML {
    template <class>
    class Element;

    template <class>
    class InstanceSpecification;

    template <class>
    class StructuralFeature;

    template <class>
    class ValueSpecification;

    template <class ManagerPolicy>
    class Slot: public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<Slot, EGM::TemplateTypeList<Element>>;
        protected:
            using OwningInstance = EGM::Singleton<InstanceSpecification, Slot>;
            using DefiningFeature = EGM::Singleton<StructuralFeature, Slot>;
            using Values = IndexableSet<ValueSpecification, Slot>;
            OwningInstance m_owningInstance = OwningInstance(this);
            DefiningFeature m_definingFeature = DefiningFeature(this);
            Values m_values = Values(this);
        private:
            void init() {
                m_owningInstance.subsets(ManagerPolicy::m_owner);
                m_owningInstance.opposite(&OwningInstance::ManagedType::getSlots);
                m_values.subsets(ManagerPolicy::m_ownedElements);
            }
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(Slot);

            using InstanceImpl = typename OwningInstance::ManagedType;
            using InstancePtr = EGM::ManagedPtr<InstanceImpl>;
            OwningInstance& getOwningInstanceSingleton() { return m_owningInstance; }
            InstancePtr getOwningInstance() const { return m_owningInstance.get(); }
            void setOwningInstance(InstanceImpl& el) { m_owningInstance.set(el); }
            void setOwningInstance(InstancePtr el) { m_owningInstance.set(el); }

            using StructuralFeatureImpl = typename DefiningFeature::ManagedType;
            using StructuralFeaturePtr = EGM::ManagedPtr<StructuralFeatureImpl>;
            DefiningFeature& getDefiningFeatureSingleton() { return m_definingFeature; }
            StructuralFeaturePtr getDefiningFeature() { return m_definingFeature.get(); }
            void setDefiningFeature(StructuralFeatureImpl& el) { m_definingFeature.set(el); }
            void setDefiningFeature(StructuralFeaturePtr el) { m_definingFeature.set(el); }

            Values& getValues() { return m_values; }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::Slot> {
        static std::string name() { return "Slot"; }
        template <class Policy>
        static SetList sets(UML::Slot<Policy>& el) {
            return SetList {
                make_set_pair("owningInstance", el.getOwningInstanceSingleton()),
                make_set_pair("definingFeature", el.getDefiningFeatureSingleton()),
                make_set_pair("values", el.getValues())
            };
        }
    };
}
