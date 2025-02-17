#pragma once

#include "egm/egm-basic.h"
#include "uml/util/boolDataPolicy.h"

namespace UML {
    template <class>
    class Feature;

    template <class>
    class TypedElement;

    template <class>
    class MultiplicityElement;

    template <class ManagerPolicy>
    class StructuralFeature : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<StructuralFeature, EGM::TemplateTypeList<Feature, TypedElement, MultiplicityElement>>;
        protected:
            bool m_readonly = false;
        private:
            void init() {}
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(StructuralFeature);
            bool isReadOnly() const { return m_readonly; }
            void setReadOnly(bool b) { m_readonly = b; }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::StructuralFeature> {
        static const bool abstract = true;
        static std::string name() { return "StructuralFeature"; }
        template <class Policy>
        struct ReadOnlyPolicy : public UML::BooleanDataPolicy {
            EGM::ManagedPtr<UML::StructuralFeature<Policy>> el;
            ReadOnlyPolicy(UML::StructuralFeature<Policy>& ref) : el(&ref) {}
            bool getBool() override { return el->isReadOnly(); }
            void setBool(bool b) override { el->setReadOnly(b); }
        };
    };
}
