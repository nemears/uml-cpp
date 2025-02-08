#pragma once

#include "egm/egm-basic.h"

namespace UML {
    template <class>
    class ValueSpecification;

    template <class ManagerPolicy>
    class LiteralSpecification : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<LiteralSpecification, EGM::TemplateTypeList<ValueSpecification>>;
        private:
            void init() {}
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(LiteralSpecification);
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::LiteralSpecification> {
        static const bool abstract = true;
        static std::string name() { return "LiteralSpecification"; }
    };
}
