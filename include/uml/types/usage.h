#pragma once

#include "egm/egm-basic.h"

namespace UML {
    template <class>
    class Dependency;

    template <class ManagerPolicy>
    class Usage : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<Usage, EGM::TemplateTypeList<Dependency>>;
        private:
            void init() {}
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(Usage);
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::Usage> {
        static std::string name() { return "Usage"; }
    };
}
