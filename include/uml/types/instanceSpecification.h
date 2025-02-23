#pragma once

#include "egm/egm-basic.h"
#include "egm/managedPtr.h"
#include "uml/types/valueSpecification.h"
#include "uml/util/indexableSet.h"

namespace UML {
    template <class>
    class PackageableElement;

    template <class>
    class Classifier;

    template <class>
    class Slot;

    template <class ManagerPolicy>
    class InstanceSpecification : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<InstanceSpecification, EGM::TemplateTypeList<PackageableElement>>;
        protected:
            using Classifiers = IndexableSet<Classifier, InstanceSpecification>;
            using Slots = EGM::Set<Slot, InstanceSpecification>;
            using Specification = EGM::Singleton<ValueSpecification, InstanceSpecification>;
            Classifiers m_classifiers = Classifiers(this);
            Slots m_slots = Slots(this);
            Specification m_specification = Specification(this);
        private:
            void init() {
                m_slots.subsets(ManagerPolicy::m_ownedElements);
                m_slots.opposite(&Slots::ManagedType::getOwningInstanceSingleton);
                m_specification.subsets(ManagerPolicy::m_ownedElements);
            }
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(InstanceSpecification);
            Classifiers& getClassifiers() { return m_classifiers; }
            Slots& getSlots() { return m_slots; }

            Specification& getSpecificationSingleton() { return m_specification; }
            using ValueImpl = typename Specification::ManagedType;
            using ValuePtr = EGM::ManagedPtr<ValueImpl>;
            ValuePtr getSpecification() const { return m_specification.get(); }
            void setSpecification(ValueImpl& el) { m_specification.set(el); }
            void setSpecification(ValuePtr el) { m_specification.set(el); }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::InstanceSpecification> {
        static std::string name() { return "InstanceSpecification"; }
        template <class Policy>
        static SetList sets(UML::InstanceSpecification<Policy>& el) {
            return SetList {
                make_set_pair("classifiers", el.getClassifiers()),
                make_set_pair("slots", el.getSlots()),
                make_set_pair("specification", el.getSpecificationSingleton())
            };
        }
    };
}
