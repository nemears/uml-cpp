#pragma once

#include "egm/egm-basic.h"
#include "uml/util/boolDataPolicy.h"

namespace UML {
    template <class>
    class Element;

    template <class>
    class ValueSpecification;

    template <class ManagerPolicy>
    class MultiplicityElement : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<MultiplicityElement, EGM::TemplateTypeList<Element>>;
        protected:
            bool m_isOrdered = false;
            bool m_isUnique = true;
            using ValueSingleton = EGM::Singleton<ValueSpecification, MultiplicityElement>;
            ValueSingleton m_lowerValue = ValueSingleton(this);
            ValueSingleton m_upperValue = ValueSingleton(this);
        private:
            void init() {
                m_lowerValue.subsets(ManagerPolicy::m_ownedElements);
                m_upperValue.subsets(ManagerPolicy::m_ownedElements);
            }
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(MultiplicityElement);
            ValueSingleton& getLowerValueSingleton() { return m_lowerValue; }
            ValueSingleton& getUpperValueSingleton() { return m_upperValue; }
            using ValueSpecificationImpl = typename ValueSingleton::ManagedType;
            using ValueSpecificationPtr = EGM::ManagedPtr<ValueSpecificationImpl>;
            ValueSpecificationPtr getLowerValue() { return m_lowerValue.get(); }
            void setLowerValue(ValueSpecificationImpl& lowerVal) { m_lowerValue.set(lowerVal); }
            void setLowerValue(ValueSpecificationPtr ptr) { m_lowerValue.set(ptr); }
            ValueSpecificationPtr getUpperValue() { return m_upperValue.get(); }
            void setUpperValue(ValueSpecificationImpl& el) { m_upperValue.set(el); }
            void setUpperValue(ValueSpecificationPtr ptr) { m_upperValue.set(ptr); }
            bool isOrdered() { return m_isOrdered; }
            void setIsOrdered(bool val) { m_isOrdered = val; }
            bool isUnique() { return m_isUnique; }
            void setIsUnique(bool val) { m_isUnique = val; }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::MultiplicityElement> {
        static const bool abstract = true;
        template <class Policy>
        SetList sets(UML::MultiplicityElement<Policy>& el) {
            return SetList {
                make_set_pair("lowerValue", el.m_lowerVal),
                make_set_pair("upperValue", el.m_upperVal)   
            };
        }
        template <class Policy>
        struct OrderedPolicy : public UML::BooleanDataPolicy {
            EGM::ManagedPtr<UML::MultiplicityElement<Policy>> el;
            OrderedPolicy(UML::MultiplicityElement<Policy>& ref) : el(&ref) {}
            bool getBool() override { return el->isOrdered(); }
            void setBool(bool b) override { el->setIsOrdered(b); }
        };
        template <class Policy>
        struct UniquePolicy : public UML::BooleanDataPolicy {
            EGM::ManagedPtr<UML::MultiplicityElement<Policy>> el;
            UniquePolicy(UML::MultiplicityElement<Policy>& ref) : el(&ref) {}
            bool getBool() override { return el->isUniqe(); }
            void setBool(bool b) override { el->setIsUnique(b); }
        };
        template <class Policy>
        DataList data(UML::MultiplicityElement<Policy>& el) {
            return DataList {
                createDataPair<OrderedPolicy<Policy>>("ordered", el),
                createDataPair<UniquePolicy<Policy>>("unique", el)
            };
        }
    };
}
