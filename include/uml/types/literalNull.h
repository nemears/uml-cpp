#pragma once

#include "egm/egm-basic.h"

namespace UML {
    template <class>
    class LiteralSpecification;

    template <class ManagerPolicy>
    class LiteralNull : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<LiteralNull, EGM::TemplateTypeList<LiteralSpecification>>;
        private:
            void init() {}
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(LiteralNull);
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::LiteralNull> {
        static std::string name() { return "LiteralNull"; }
    };
}
