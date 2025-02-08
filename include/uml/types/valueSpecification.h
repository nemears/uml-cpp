#pragma once

#include "egm/egm-basic.h"

namespace UML {
    template <class>
    class PackageableElement;

    template <class>
    class TypedElement;

    template <class ManagerPolicy>
    class ValueSpecification : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<ValueSpecification, EGM::TemplateTypeList<PackageableElement, TypedElement>>;
        private:
            void init() {}
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(ValueSpecification);
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::ValueSpecification> {
        static const bool abstract = true;
        static std::string name() { return "ValueSpecification"; }
    };
}
